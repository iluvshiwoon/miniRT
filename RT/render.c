/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:17:58 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/25 16:02:04 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../miniRT.h"
#include <sys/time.h>

static void	put_pixel_gamma_corrected(t_rt *rt, int x, int y, t_vec intensity)
{
    int r;
    int g;
    int b;

    r = fmin(255, fmax(0, pow(intensity.x, 1 / 2.2)));
    g = fmin(255, fmax(0, pow(intensity.y, 1 / 2.2)));
    b = fmin(255, fmax(0, pow(intensity.z, 1 / 2.2)));
    my_mlx_put_pixel(&rt->image, x, y, create_trgb(255, r, g, b));
}

static void	accumulate_pixel_intensity(t_rt *rt, t_worker *worker, \
    t_render *r)
{
    while (r->k < r->nrays && \
        !atomic_load(&worker->shared->should_exit) && \
        !atomic_load(&worker->shared->work_paused))
    {
        r->pixel_intensity = vec_plus(r->pixel_intensity, \
            vec_mult(1.0 / r->nrays, \
                get_color(r->ray, rt, 5, &worker->data.rng)));
        r->k++;
    }
}

void    fill_chunk_index(t_rt * rt, t_chunk * chunks, int num_threads)
{
    int chunk_size;
    int i;

    chunk_size = rt->total_pixels / num_threads;
    i = 0;
    while (i < num_threads)
    {
        chunks[i].start_pixel = i * chunk_size; 
        chunks[i].end_pixel = (i + 1) * chunk_size;

        if (i == num_threads - 1)
            chunks[i].end_pixel += rt->total_pixels % num_threads;
        i++;
    }
}

static void	process_one_pixel(t_rt *rt, t_worker *worker, t_render *r)
{
    r->k = 0;
    r->ray = rt->state.rays[r->index];
    r->pixel_intensity = (t_vec){};
    accumulate_pixel_intensity(rt, worker, r);
    if (!atomic_load(&worker->shared->should_exit) && \
        !atomic_load(&worker->shared->work_paused))
        put_pixel_gamma_corrected(rt, r->x, rt->h - 1 - r->y, \
            r->pixel_intensity);
}

void render_chunk(t_rt *rt, t_worker * worker, int pass)
{
    t_render r; 
    int start_pixel;
    int end_pixel;
    int skip;
    
    start_pixel = worker->data.start_pixel;
    end_pixel = worker->data.end_pixel;
    skip = rt->config[pass].skip;
    r.nrays = rt->config[pass].bounces;
    
    while (start_pixel < end_pixel && \
           !atomic_load(&worker->shared->should_exit) && \
           !atomic_load(&worker->shared->work_paused))
    {
        r.index = rt->state.shuffled_pixels[start_pixel];
        r.x = r.index % rt->w;
        r.y = rt->h - 1 - (r.index / rt->w);
        process_one_pixel(rt, worker, &r);
        start_pixel += skip;
    }
}

static void	wait_for_work_available(t_shared *shared)
{
    pthread_mutex_lock(&shared->work_mutex);
    while (!atomic_load(&shared->work_ready) && \
        !atomic_load(&shared->should_exit))
        pthread_cond_wait(&shared->work_available, &shared->work_mutex);
    pthread_mutex_unlock(&shared->work_mutex);
}

static void	handle_pause_state(t_shared *shared, int *pass)
{
    if (atomic_load(&shared->work_paused))
    {
        *pass = 0;
        atomic_fetch_add(&shared->paused_threads, 1);
        while (atomic_load(&shared->work_paused) && \
            !atomic_load(&shared->should_exit))
            usleep(1000);
        atomic_fetch_sub(&shared->paused_threads, 1);
    }
}

static void	process_work(t_shared *shared, t_worker *worker, int *pass)
{
    while (*pass < 3 && !atomic_load(&shared->work_paused) && \
           !atomic_load(&shared->should_exit))
    {
        render_chunk(shared->rt, worker, *pass);
        (*pass)++;
    }
}

void * worker_thread_loop(void * arg)
{
    t_worker * worker;
    t_shared * shared;
    int pass;
    
    worker = arg;
    shared = worker->shared;
    pass = 0;
    while (!atomic_load(&shared->should_exit))
    {
        wait_for_work_available(shared);
        if (atomic_load(&shared->should_exit))
            break;
        handle_pause_state(shared, &pass);
        if (atomic_load(&shared->should_exit))
            break;
        process_work(shared, worker, &pass);
    }
    return NULL;
}

static void	pause_threads_for_rerender(t_rt *rt, bool first_render)
{
    if (!first_render)
    {
        pthread_mutex_lock(&rt->shared->work_mutex);
        atomic_store(&rt->shared->work_paused, true);
        pthread_mutex_unlock(&rt->shared->work_mutex);
        while (atomic_load(&rt->shared->paused_threads) < \
            rt->shared->num_threads)
            usleep(1000);
        key_events(0, rt);
    }
}

static void	start_or_resume_workers(t_rt *rt)
{
    pthread_mutex_lock(&rt->shared->work_mutex);
    atomic_store(&rt->shared->work_paused, false);
    atomic_store(&rt->shared->work_ready, true);
    pthread_cond_broadcast(&rt->shared->work_available);
    pthread_mutex_unlock(&rt->shared->work_mutex);
}

int render(t_rt *rt)
{
    static bool first_render = true;

    if (atomic_load(&rt->state.re_render_scene))
    {
        pause_threads_for_rerender(rt, first_render);
        init_render(rt);
        start_or_resume_workers(rt);
        rt->state.pass = 3;
        first_render = false;
    }
    if (rt->state.pass >= 3)
    {
        mlx_put_image_to_window(rt->mlx, rt->win, rt->image.img, 0, 0);
        if (rt->state.display_string == true)
            display_string(rt, rt->state.display_id);
        return (1);
    }
    return (0);
}

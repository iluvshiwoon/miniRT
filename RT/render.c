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
    
    while (start_pixel < end_pixel 
           && !atomic_load(&worker->shared->should_exit)
           && !atomic_load(&worker->shared->work_paused))
    {
        r.index = rt->state.shuffled_pixels[start_pixel];
        r.x = r.index % rt->w;
        r.y = rt->h - 1 - (r.index / rt->w);
        r.k = 0;
        r.ray = rt->state.rays[r.index];
        r.pixel_intensity = (t_vec){};
        
        while (r.k < r.nrays 
               && !atomic_load(&worker->shared->should_exit)
               && !atomic_load(&worker->shared->work_paused))
        {
            r.pixel_intensity = vec_plus(r.pixel_intensity, vec_mult(1.0
                                                                     / r.nrays, get_color(r.ray, rt, 5, &worker->data.rng)));
            r.k++;
        }
        
        if (!atomic_load(&worker->shared->should_exit) 
            && !atomic_load(&worker->shared->work_paused))
        {
            my_mlx_put_pixel(&rt->image, r.x, rt->h - 1 - r.y, create_trgb(255, \
                                                                           fmin(255, fmax(0, pow(r.pixel_intensity.x, 1 / 2.2))), \
                                                                           fmin(255, fmax(0, pow(r.pixel_intensity.y, 1 / 2.2))), \
                                                                           fmin(255, fmax(0, pow(r.pixel_intensity.z, 1 / 2.2)))));
        }
        start_pixel += skip;
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
        pthread_mutex_lock(&shared->work_mutex);
        while (!atomic_load(&shared->work_ready) && !atomic_load(&shared->should_exit))
            pthread_cond_wait(&shared->work_available, &shared->work_mutex);
        pthread_mutex_unlock(&shared->work_mutex);
        
        if (atomic_load(&shared->should_exit))
            break;
            
        // Check if we should pause BEFORE starting work
        if (atomic_load(&shared->work_paused)) {
            pass = 0;
            atomic_fetch_add(&shared->paused_threads, 1);
            // Wait until unpaused
            while (atomic_load(&shared->work_paused) 
                   && !atomic_load(&shared->should_exit)) {
                usleep(1000); // 1ms
            }
            atomic_fetch_sub(&shared->paused_threads, 1);
            if (atomic_load(&shared->should_exit))
                break;
        }
        
        while (pass < 3 && !atomic_load(&shared->work_paused) 
               && !atomic_load(&shared->should_exit))
        {
            render_chunk(shared->rt, worker, pass);
            pass++;
        }
    }
    return NULL;
}

void    init_thread(t_rt *rt)
{
    int i;
    t_shared * shared;
    t_worker * workers;
    int seeds[2];

    shared = rt->shared;
    i = 0;
    workers = wrap_malloc(rt, sizeof(*workers) * shared->num_threads);
    rt->workers = workers;
    while (i < shared->num_threads)
    {
        workers[i].shared = shared;
        workers[i].data.thread_id = i;
        workers[i].data.start_pixel = shared->chunks[i].start_pixel;
        workers[i].data.end_pixel = shared->chunks[i].end_pixel;
        entropy_getbytes((void *)seeds, sizeof(seeds));
        pcg_setseq_64_srandom_r(&workers[i].data.rng, seeds[0], seeds[1]);
        pthread_create(&workers[i].thread, NULL, worker_thread_loop, &workers[i]);
        i++;
    }
}

void    init_multi_threading(t_rt * rt)
{
    rt->shared = wrap_malloc(rt, sizeof(*rt->shared));
    *rt->shared = (t_shared){};
    rt->shared->rt = rt;
    // Then explicitly initialize atomics:
    atomic_store(&rt->shared->work_paused, false);
    atomic_store(&rt->shared->paused_threads, 0);
    atomic_store(&rt->shared->should_exit, false);
    atomic_store(&rt->shared->work_ready, false);
    atomic_store(&rt->shared->current_pass, 0);
    if (pthread_mutex_init(&rt->shared->display_mutex, NULL) != 0)
        exit_error(rt, "Failed to initialize mutex");
    if (pthread_mutex_init(&rt->shared->work_mutex, NULL) != 0)
        exit_error(rt, "Failed to initialize mutex");
    if (pthread_cond_init(&rt->shared->work_available, NULL) != 0)
        exit_error(rt, "Failed to initialize condition");
    if (pthread_cond_init(&rt->shared->to_display, NULL) != 0)
        exit_error(rt, "Failed to initialize condition");
    rt->shared->num_threads = sysconf(_SC_NPROCESSORS_ONLN) * 16;
    // rt->shared->num_threads = 2;
    if (rt->shared->num_threads <= 0)
        exit_error(rt, "Failed to retrieve number of available threads");
    rt->shared->chunks = wrap_malloc(rt, sizeof(*rt->shared->chunks)*rt->shared->num_threads);
    fill_chunk_index(rt, rt->shared->chunks, rt->shared->num_threads);
    init_thread(rt);
}

int render(t_rt *rt)
{
    static bool first_render = true;
    
    if (atomic_load(&rt->state.re_render_scene)) {
        // Only pause threads if they're already running (not first render)
        if (!first_render) {
            // printf("Pausing threads for re-render...\n");
            pthread_mutex_lock(&rt->shared->work_mutex);
            atomic_store(&rt->shared->work_paused, true);
            pthread_mutex_unlock(&rt->shared->work_mutex);
            
            // Wait for threads to pause
            while (atomic_load(&rt->shared->paused_threads) < rt->shared->num_threads) {
                usleep(1000);
            }
            // printf("All threads paused\n");
            key_events(0,rt);
            // atomic_store(&rt->state.render_paused, true);
            // while (atomic_load(&rt->state.render_paused) == true)
            //     usleep(1000);
            // printf("Re render now\n");
        }
        
        init_render(rt);
        
        // Start/resume workers
        pthread_mutex_lock(&rt->shared->work_mutex);
        atomic_store(&rt->shared->work_paused, false);
        atomic_store(&rt->shared->work_ready, true);
        pthread_cond_broadcast(&rt->shared->work_available);
        pthread_mutex_unlock(&rt->shared->work_mutex);
        
        rt->state.pass = 3;
        first_render = false;
    }
    
    if (rt->state.pass >= 3) {
        mlx_put_image_to_window(rt->mlx, rt->win, rt->image.img, 0, 0);
        if (rt->state.display_string == true)
            display_string(rt, rt->state.display_id);
        return (1);
    }
    
    return (0);
}

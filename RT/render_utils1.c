/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:30:24 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/14 20:24:44 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	put_pixel_gamma_corrected(t_rt *rt, int x, int y, t_vec intensity)
{
	int	r;
	int	g;
	int	b;

	r = fmin(255, fmax(0, pow(intensity.x, 1 / 2.2)));
	g = fmin(255, fmax(0, pow(intensity.y, 1 / 2.2)));
	b = fmin(255, fmax(0, pow(intensity.z, 1 / 2.2)));
	my_mlx_put_pixel(&rt->image, x, y, create_trgb(255, r, g, b));
}

void	accumulate_pixel_intensity(t_rt *rt, t_worker *worker, t_render *r)
{
	while (r->k < r->nrays && !atomic_load(&worker->shared->should_exit)
		&& !atomic_load(&worker->shared->work_paused))
	{
		r->pixel_intensity = vec_plus(r->pixel_intensity, vec_mult(1.0
					/ r->nrays, get_color(r->ray, rt, 5, &worker->data.rng)));
		r->k++;
	}
}

void	fill_chunk_index(t_rt *rt, t_chunk *chunks, int num_threads)
{
	int	chunk_size;
	int	i;

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

void	process_one_pixel(t_rt *rt, t_worker *worker, t_render *r)
{
	r->k = 0;
	r->ray = rt->state.rays[r->index];
	r->pixel_intensity = (t_vec){};
	accumulate_pixel_intensity(rt, worker, r);
	if (!atomic_load(&worker->shared->should_exit)
		&& !atomic_load(&worker->shared->work_paused))
		put_pixel_gamma_corrected(rt, r->x, rt->h - 1 - r->y,
			r->pixel_intensity);
}

void	render_chunk(t_rt *rt, t_worker *worker, int pass)
{
	t_render	r;
	int			start_pixel;
	int			end_pixel;
	int			skip;

	start_pixel = worker->data.start_pixel;
	end_pixel = worker->data.end_pixel;
	skip = rt->config[pass].skip;
	r.nrays = rt->config[pass].bounces;
	while (start_pixel < end_pixel && !atomic_load(&worker->shared->should_exit)
		&& !atomic_load(&worker->shared->work_paused))
	{
		r.index = rt->state.shuffled_pixels[start_pixel];
		r.x = r.index % rt->w;
		r.y = rt->h - 1 - (r.index / rt->w);
		process_one_pixel(rt, worker, &r);
		start_pixel += skip;
	}
}

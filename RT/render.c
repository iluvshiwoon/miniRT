/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:17:58 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/18 17:23:03 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../miniRT.h"

bool	render_pixels(t_rt *rt, t_render *r)
{
	r->index = rt->state.shuffled_pixels[rt->state.pixel_index];
	r->x = r->index % rt->W;
	r->y = rt->H - 1 - (r->index / rt->W);
	r->k = -1;
	r->ray = rt->state.rays[r->index];
	r->pixel_intensity = (t_vec){0., 0., 0.};
	while (++(r->k) < r->nrays)
		r->pixel_intensity = vec_plus(r->pixel_intensity, vec_mult(1.0
					/ r->nrays, get_color(r->ray, rt, 5)));
	my_mlx_put_pixel(&rt->image, r->x, rt->H - 1 - r->y, create_trgb(255, \
		fmin(255, fmax(0, pow(r->pixel_intensity.x, 1 / 2.2))), \
		fmin(255, fmax(0, pow(r->pixel_intensity.y, 1 / 2.2))), \
		fmin(255, fmax(0, pow(r->pixel_intensity.z, 1 / 2.2)))));
	if (rt->state.pixel_index % rt->config[rt->state.pass].update_freq == 0)
	{
		mlx_put_image_to_window(rt->mlx, rt->win, rt->image.img, 0, 0);
		rt->state.pixel_index += rt->config[rt->state.pass].skip;
		if (rt->state.display_string == true)
			display_string(rt, rt->state.display_id);
		return (false);
	}
	rt->state.pixel_index += rt->config[rt->state.pass].skip;
	return (true);
}

int	render(t_rt *rt)
{
	t_render	r;

	if (rt->state.re_render_scene)
		init_render(rt);
	else if (rt->state.pass == 3)
	{
		mlx_put_image_to_window(rt->mlx, rt->win, rt->image.img, 0, 0);
		if (rt->state.display_string == true)
			display_string(rt, rt->state.display_id);
		return (1);
	}
	r.nrays = rt->config[rt->state.pass].bounces;
	while (rt->state.pixel_index < rt->total_pixels)
	{
		if (render_pixels(rt, &r) == false)
			break ;
	}
	if (rt->state.pixel_index >= rt->total_pixels)
	{
		rt->state.pass++;
		rt->state.pixel_index = 0;
	}
	return (0);
}

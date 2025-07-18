/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:30:24 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/18 16:57:30 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"

void	gen_shuffled_pixels(t_rt *rt, int *array)
{
	int	i;
	int	j;
	int	temp;

	i = -1;
	while (++i < rt->total_pixels)
		array[i] = i;
	while (--i + 1 > 0)
	{
		j = pcg32_boundedrand_r(&rt->rng, rt->total_pixels);
		temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}

void	gen_rays(t_rt *rt)
{
	t_gen_rays	gr;
	int			pixel_index;

	gr.cam = rt->scene.camera;
	gr.focal_length = (rt->W / 2.0) / tan(gr.cam.fov / 2.0);
	gr.cam_z = vec_mult(-1, gr.cam.direction);
	gr.cam_y = normalize(gr.cam.up);
	gr.cam_x = normalize(cross(gr.cam_y, gr.cam_z));
	gr.cam_y = normalize(cross(gr.cam_z, gr.cam_x));
	if (rt->state.rays == NULL)
		rt->state.rays = wrap_malloc(rt, sizeof(*(rt->state.rays))
				* rt->total_pixels);
	pixel_index = 0;
	while (pixel_index < rt->total_pixels)
	{
		gr.x = pixel_index % rt->W;
		gr.y = rt->H - 1 - (pixel_index / rt->W);
		gr.u = -(gr.x - rt->W / 2.0);
		gr.v = gr.y - rt->H / 2.0;
		gr.direction = vec_plus(vec_plus(vec_mult(gr.u, gr.cam_x) \
		, vec_mult(gr.v, gr.cam_y)), vec_mult(gr.focal_length, gr.cam_z));
		gr.direction = normalize(gr.direction);
		rt->state.rays[pixel_index] = (t_ray){gr.cam.origin, gr.direction};
		pixel_index++;
	}
}

void	init_render(t_rt *rt)
{
	rt->state.re_render_scene = false;
	if (rt->image.img)
		mlx_destroy_image(rt->mlx, rt->image.img);
	rt->image.img = mlx_new_image(rt->mlx, rt->W, rt->H);
	if (!rt->image.img)
		close_win(rt);
	rt->image.addr = mlx_get_data_addr(rt->image.img, &rt->image.bits_per_pixel,
			&rt->image.line_length, &rt->image.endian);
	rt->state.pass = 0;
	rt->state.pixel_index = 0;
	gen_rays(rt);
}

void	display_string(t_rt *rt, int id)
{
	bool	black;

	black = (rt->state.color_black != 1);
	mlx_string_put(rt->mlx, rt->win, 7, 15, create_trgb(255, 255 * black, 255
			* black, 255 * black), rt->scene.objects[id].string);
}

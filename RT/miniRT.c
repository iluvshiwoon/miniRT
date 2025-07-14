/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:19:53 by kgriset           #+#    #+#             */
/*   Updated: 2025/04/13 16:29:18 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <sys/resource.h>

void gen_shuffled_pixels(t_rt * rt, int * array)
{
    int i = -1;
    
    while (++i < rt->total_pixels)
        array[i] = i;

    while (--i+1 > 0)
    {
        int j = pcg32_boundedrand_r(&rt->rng, rt->total_pixels);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void gen_rays(t_rt * rt)
{
	t_camera cam = rt->scene.camera;
	double focal_length = (rt->W / 2.0) / tan(cam.fov / 2.0);
	t_vec cam_z = vec_mult(-1, cam.direction);
	t_vec temp_up = {0, 1, 0};
	if (fabs(cam_z.y) > 0.999)
		temp_up = (t_vec){1, 0, 0};
	t_vec cam_x = normalize(cross(temp_up, cam_z));
	t_vec cam_y = cross(cam_z, cam_x);

	rt->state.rays = wrap_malloc(rt, sizeof(*(rt->state.rays))*rt->total_pixels);
	int pixel_index = 0;
    	while (pixel_index < rt->total_pixels)
	{
		int x = pixel_index % rt->W;    
		int y = rt->H - 1 -(pixel_index / rt->W);
		double u = -(x - rt->W / 2.0);
		double v = y - rt->H / 2.0;
		t_vec direction = vec_plus(vec_plus(vec_mult(u, cam_x), vec_mult(v, cam_y)), vec_mult(focal_length, cam_z));
		direction = normalize(direction);
		rt->state.rays[pixel_index] = (t_ray){cam.origin,direction};
		pixel_index++;
	}
}

void init_render(t_rt * rt)
{
    rt->state.re_render_scene = false;
    if (rt->image.img)
        mlx_destroy_image(rt->mlx,rt->image.img);
    rt->image.img = mlx_new_image(rt->mlx, rt->W, rt->H ) ;
    if (!rt->image.img)
        close_win(rt);
    rt->image.addr = mlx_get_data_addr(rt->image.img, &rt->image.bits_per_pixel, &rt->image.line_length,
                                       &rt->image.endian);
    rt->state.pass = 0;
    rt->state.pixel_index = 0;
    gen_rays(rt);
}



int render (t_rt * rt)
{
    if (rt->state.re_render_scene)
        init_render(rt);
    else if (rt->state.pass == 3)
            return 1;

    int nrays;
    int k;
    t_pass_config passes[] = {
    {4,   2, rt->W},       // Medium speed, more frequent updates
    {10,  1, rt->W/2},     // Slower pixels, frequent updates
    {80,  1, rt->W/8}      // Slowest pixels, most frequent updates
    };
;
    int * shuffled_pixels = rt->state.shuffled_pixels;
    nrays = passes[rt->state.pass].bounces;
    while (rt->state.pixel_index < rt->total_pixels)
    {
        int index = shuffled_pixels[rt->state.pixel_index];
        int x = index % rt->W;    
        int y = rt->H - 1 -(index / rt->W);
        k = -1;
        t_ray ray = rt->state.rays[index];
        t_vec pixel_intensity = (t_vec){0.,0.,0.};
        while (++k < nrays)
            pixel_intensity = vec_plus(pixel_intensity,vec_mult(1.0/nrays,get_color(ray, rt, 5)));
        my_mlx_put_pixel(&rt->image, x, rt->H - 1 - y, \
                         create_trgb(255, fmin(255, fmax(0, pow(pixel_intensity.x,1/2.2))),\
                                     fmin(255, fmax(0, pow(pixel_intensity.y,1/2.2))),\
                                     fmin(255, fmax(0, pow(pixel_intensity.z,1/2.2)))));
        if (rt->state.pixel_index %passes[rt->state.pass].update_freq == 0)
        {
            mlx_put_image_to_window(rt->mlx, rt->win, rt->image.img, 0, 0);
            rt->state.pixel_index += passes[rt->state.pass].skip;
            break;
        }
        rt->state.pixel_index += passes[rt->state.pass].skip;
    }
    if (rt->state.pixel_index >= rt->total_pixels)
    {
        printf("Pass: %d, bounce: %d, skip: %d, update every %d pixels\n", rt->state.pass, passes[rt->state.pass].bounces, passes[rt->state.pass].skip, passes[rt->state.pass].update_freq);
        rt->state.pass++;
        rt->state.pixel_index = 0;
    }
    return 0;
}

int key_events(int keycode, t_rt *rt)
{
    if (keycode == KEY_ESC)
        close_win(rt);
    return (0);
}

int main (int ac,char ** av)
{
    t_rt rt;
    uint64_t seeds[2];

    if (check_args(ac, av))
		return (1);

    rt = (t_rt){};
    rt.state.re_render_scene = true;
    rt.graphic_heap = init_alloc(&rt.graphic_heap); 
    rt.parsing_heap = init_alloc(&rt.parsing_heap);  
    entropy_getbytes((void*)seeds,sizeof(seeds));
    pcg32_srandom_r(&rt.rng,seeds[0],seeds[1]);


    rt.current_heap = rt.parsing_heap;
    parsing_minirt(&rt,av[1]);

    rt.W = 700;
    rt.H = 700;
    rt.total_pixels = rt.W * rt.H;
    rt.current_heap = rt.graphic_heap;
    rt.mlx = mlx_init();
    if (!rt.mlx)
        printf("malloc error mlx_init\n");
    rt.win = mlx_new_window(rt.mlx, rt.W, rt.H, "miniRT");
    if (!rt.win)
        printf("malloc error mlx_init\n");

    rt.state.shuffled_pixels = wrap_malloc(&rt, rt.W * rt.H * sizeof(*(rt.state.shuffled_pixels)));
    gen_shuffled_pixels(&rt, rt.state.shuffled_pixels);

    mlx_hook(rt.win, KeyPress, KeyPressMask, &key_events, &rt);
    mlx_hook(rt.win, DestroyNotify, 0, &close_win, &rt);
    mlx_loop_hook(rt.mlx, render, &rt);
    mlx_loop(rt.mlx);
    return 0;
}

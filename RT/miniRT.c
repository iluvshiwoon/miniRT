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
    int total_pixels = rt->H * rt->W;
    int i = -1;
    
    while (++i < total_pixels)
        array[i] = i;

    while (--i+1 > 0)
    {
        int j = pcg32_boundedrand_r(&rt->rng, total_pixels);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

unsigned char * render (t_rt * rt)
{
    unsigned char * image;
    t_data img;
    int i;
    int nrays;
    int j;
    int k;
    int *shuffled_pixels;
    shuffled_pixels = wrap_malloc(rt, rt->W * rt->H * sizeof(*shuffled_pixels));

     struct pass_config passes[] = {
    {1,  16, rt->W*4},     // Fast pixels, less frequent updates
    {1,   4, rt->W*2},     // Still fast, moderate updates  
    {4,   2, rt->W},       // Medium speed, more frequent updates
    {10,  1, rt->W/2},     // Slower pixels, frequent updates
    {25,  1, rt->W/4},     // Much slower, very frequent updates
    {80,  1, rt->W/8}      // Slowest pixels, most frequent updates
    };

    int pass = -1;
    img.img = mlx_new_image(rt->mlx, rt->W, rt->H ) ;
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
                                 &img.endian);
    gen_shuffled_pixels(rt, shuffled_pixels);
    int total_pixels = rt->H * rt->W;
    while (++pass < 6)
    {
        nrays = passes[pass].bounces;
        int l = 0;
        printf("Pass: %d, bounce: %d, skip: %d, update every %d pixels\n", pass, passes[pass].bounces, passes[pass].skip, passes[pass].update_freq);
        while (l < total_pixels)
        {
            int index = shuffled_pixels[l];
            int x = index % rt->W;    
            int y = rt->H - 1 -(index / rt->W);
            k = -1;
            t_vec direction = {x - rt->W / 2.0 - rt->scene.camera.origin.x, y - rt->H /2.0 -rt->scene.camera.origin.y, - rt->W / (2 * tan((rt->scene.camera.fov)/2))};
            direction = normalize(direction);
            t_ray ray = {{0.,0.,0.},direction};
            t_vec pixel_intensity = (t_vec){0.,0.,0.};
            while (++k < nrays)
                pixel_intensity = vec_plus(pixel_intensity,vec_mult(1.0/nrays,get_color(ray, rt, 5)));
            my_mlx_put_pixel(&img, x, rt->H - 1 - y, \
                             create_trgb(255, fmin(255, fmax(0, pow(pixel_intensity.x,1/2.2))),\
                                         fmin(255, fmax(0, pow(pixel_intensity.y,1/2.2))),\
                                         fmin(255, fmax(0, pow(pixel_intensity.z,1/2.2)))));
            if (l %passes[pass].update_freq == 0)
                mlx_put_image_to_window(rt->mlx, rt->win, img.img, 0, 0);
            l += passes[pass].skip;
        }
    }
    return image;
}

int main (int ac,char ** av)
{
    t_rt rt;
    if (check_args(ac, av))
		return (1);
    rt = (t_rt){};
    rt.graphic_heap = init_alloc(&rt.graphic_heap); 
    rt.parsing_heap = init_alloc(&rt.parsing_heap);  
    uint64_t seeds[2];
    entropy_getbytes((void*)seeds,sizeof(seeds));
    pcg32_srandom_r(&rt.rng,seeds[0],seeds[1]);
    rt.current_heap = rt.parsing_heap;
    parsing_minirt(&rt,av[1]);
    rt.W = 1500;
    rt.H = 1500;
    rt.current_heap = rt.graphic_heap;
    rt.mlx = mlx_init();
    if (!rt.mlx)
        printf("malloc error mlx_init\n");
    rt.win = mlx_new_window(rt.mlx, rt.W, rt.H, "miniRT");
    render(&rt);
    mlx_loop(rt.mlx);
    close(rt.fd_file);
    free_heap(&rt);
    return 0;
}

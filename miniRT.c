/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:19:53 by kgriset           #+#    #+#             */
/*   Updated: 2025/01/20 23:29:51 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

bool intersection(const t_ray ray, const t_sphere sphere, t_vec * P, t_vec * N)
{
    // res a*t^2 + b*t + c = 0
    double a;
    double b;
    double c;
    double delta; 
    double t2;
    double t1;
    double t;
    
    a = 1;
    b = 2 * vec_scal(ray.direction,vec_minus(ray.origin,sphere.origin));
    c = norm2(vec_minus(ray.origin,sphere.origin)) - sphere.radius * sphere.radius;
    delta = b * b - 4 * a * c;
    if (delta < 0)
        return false;
    t2 = (-b + sqrt(delta)) / (2 * a);
    if (t2 < 0)
        return false;
    t1 = (-b - sqrt(delta)) / (2 * a);
    if (t1 > 0)
        t = t1;
    else 
        t = t2;
    *P = vec_plus(ray.origin, vec_mult(t,ray.direction)); 
    *N = normalize(vec_minus(*P, sphere.origin));
    return true;
}

unsigned char * render (t_rt * rt)
{
    unsigned char * image;
    t_sphere sphere;
    int i;
    int j;

    sphere = (t_sphere){{0,0,-55},20};
    t_vec light = {15, 60, -40};
    double intensity = 1000000;
    image = wrap_malloc(rt, sizeof(unsigned char)*rt->W*rt->H*3);
    i = -1;
    while (++i < rt->H)
    {
        j = -1;
        while (++j < rt->W)
        {
            t_vec direction = {j - rt->W / 2, i - rt->H /2, - rt->W / (2 * tan(rt->fov/2))};
            direction = normalize(direction);
            t_ray ray = {{0,0,0},direction};
            t_vec P;
            t_vec N;
            double pixel_intensity = 0;
            if (intersection(ray, sphere, &P, &N))
            {
                pixel_intensity = intensity * vec_scal(normalize(vec_minus(light, P)),N) / norm2(vec_minus(light,P));
                pixel_intensity = fmin(255, fmax(10, pixel_intensity));
                image[((rt->H-i-1)*rt->W + j) * 3 + 0] = pixel_intensity;
                image[((rt->H-i-1)*rt->W + j) * 3 + 1] = pixel_intensity;
                image[((rt->H-i-1)*rt->W + j) * 3 + 2] = pixel_intensity;
            }
            else
            {
                image[((rt->H-i-1)*rt->W + j) * 3 + 0] = pixel_intensity;
                image[((rt->H-i-1)*rt->W + j) * 3 + 1] = pixel_intensity;
                image[((rt->H-i-1)*rt->W + j) * 3 + 2] = pixel_intensity;
            }
        }
    }
    return image;
}

int main ()
{
    t_rt rt;

    rt = (t_rt){};
    rt.fov = 60 * M_PI / 180;
    rt.W = 1024;
    rt.H = 1024;
    rt.graphic_heap = init_alloc(&rt.graphic_heap); 
    rt.current_heap = rt.graphic_heap;
    save_img(&rt, render(&rt), rt.W, rt.H);
    free_heap(&rt);
    return 0;
}

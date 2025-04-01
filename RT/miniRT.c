/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:19:53 by kgriset           #+#    #+#             */
/*   Updated: 2025/04/01 08:17:08 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include <sys/resource.h>

bool second_degree_solve(const t_ray ray, const t_sphere sphere, double * s)
{
    // res a*t^2 + b*t + c = 0
    double a[3];
    double t[2];
    double delta; 

    a[0] = 1;
    a[1] = 2 * vec_scal(ray.direction,vec_minus(ray.origin,sphere.origin));
    a[2] = norm2(vec_minus(ray.origin,sphere.origin)) - sphere.radius * sphere.radius;
    delta = a[1] * a[1] - 4 * a[0] * a[2];
    if (delta < 0)
        return false;
    t[1] = (-a[1] + sqrt(delta)) / (2 * a[0]);
    if (t[1] < 0)
        return false;
    t[0] = (-a[1] - sqrt(delta)) / (2 * a[0]);
    if (t[0] > 0)
        *s = t[0];
    else 
        *s = t[1];
    return true;
}

bool intersection(const t_ray ray, const t_sphere sphere, t_vec * P, t_vec * N, double * t)
{
    bool has_sol;
    
    has_sol = second_degree_solve(ray, sphere, t);
    if (has_sol == true)
    {
        *P = vec_plus(ray.origin, vec_mult(*t,ray.direction)); 
        *N = normalize(vec_minus(*P, sphere.origin));
    }
    return (has_sol);
}

bool intersections(const t_ray ray, t_scene scene, t_vec * P, t_vec * N, int *sphere_id, double * min_t)
{
    t_vec local_P;
    t_vec local_N;
    bool has_inter;
    double t;

    *min_t = 1e99;
    has_inter = false;
    while (scene.spheres_nb)
    {
        if (intersection(ray, scene.spheres[scene.spheres_nb - 1], &local_P, &local_N, &t))
        {
            has_inter = true;
            if (t < *min_t)
            {
                *min_t = t;
                *P = local_P;
                *N = local_N;
                *sphere_id = scene.spheres_nb - 1;
            }
        }
        scene.spheres_nb--;
    }
    return has_inter;
}

unsigned char * render (t_rt * rt)
{
    unsigned char * image;
    int i;
    int j;

    // rt->scene.spheres_nb = 6;
    // rt->scene.spheres = wrap_malloc(rt,sizeof(t_sphere) * rt->scene.spheres_nb);
    rt->scene.spheres[0] = (t_sphere){0,{0,0,-55},20, {1,0,0}}; // sphere
    rt->scene.spheres[1] = (t_sphere){1,{0,-2000-20,0},2000, {1,1,1}}; // sol
    rt->scene.spheres[2] = (t_sphere){2,{0,2000+100,0},2000, {1,1,1}}; // plafond;
    rt->scene.spheres[3] = (t_sphere){3,{-2000-50,0,0},2000, {0,1,0}}; // mur gaughe;
    rt->scene.spheres[4] = (t_sphere){4,{2000+50,0,0},2000, {0,0,1}}; // mur droit;
    rt->scene.spheres[5] = (t_sphere){5,{0,0,-2000 - 100},2000, {0,1,1}}; // mur fond;
    // t_vec light = {15, 60, -40};
    double intensity = 100000000;
    image = wrap_malloc(rt, sizeof(unsigned char)*rt->W*rt->H*3);
    i = -1;
    while (++i < rt->H)
    {
        j = -1;
        while (++j < rt->W)
        {
            t_vec direction = {j - rt->W / 2, i - rt->H /2, - rt->W / (2 * tan((rt->scene.camera.fov * M_PI / 180)/2))};
            direction = normalize(direction);
            t_ray ray = {{0,0,0},direction};
            t_vec P;
            t_vec N;
            t_vec pixel_intensity;
            int sphere_id = 0;
            double t;
            if (intersections(ray, rt->scene, &P, &N, &sphere_id, &t))
            {
                t_ray ray_light = {vec_plus(P,vec_mult(0.01,N)),normalize(vec_minus(rt->scene.light.origin,P))};
                t_vec P_light;
                t_vec N_light;
                int sphere_id_light = 0;
                double t_light;
                bool inter = intersections(ray_light,rt->scene, &P_light, &N_light, &sphere_id_light, &t_light);
                double d_light2 = norm2(vec_minus(rt->scene.light.origin,P));
                if (inter && t_light*t_light < d_light2)
                    pixel_intensity = (t_vec){0,0,0};
                else
                    pixel_intensity = vec_mult(intensity * fmax(0,vec_scal(normalize(vec_minus(rt->scene.light.origin, P)),N)) / d_light2,rt->scene.spheres[sphere_id].albedo);
            }
            image[((rt->H-i-1)*rt->W + j) * 3 + 0] = fmin(255, fmax(0, pow(pixel_intensity.x,1/2.2)));
            image[((rt->H-i-1)*rt->W + j) * 3 + 1] = fmin(255, fmax(0, pow(pixel_intensity.y,1/2.2)));
            image[((rt->H-i-1)*rt->W + j) * 3 + 2] = fmin(255, fmax(0, pow(pixel_intensity.z,1/2.2)));
        }
    }
    return image;
}

int main (int ac,char ** av)
{
    // struct rlimit rlim;;
    // rlim.rlim_cur = 1024 * 1000;
    // rlim.rlim_max = 1024 * 1000;
    // if (setrlimit(RLIMIT_DATA, &rlim) == -1)
    // {
    //     perror("setrlimit");
    //     return (43);
    // }
    
    t_rt rt;
    if (check_args(ac, av))
		return (1);
    rt = (t_rt){};
    rt.graphic_heap = init_alloc(&rt.graphic_heap); 
    rt.parsing_heap = init_alloc(&rt.parsing_heap);  
    rt.current_heap = rt.parsing_heap;
    parsing_minirt(&rt,av[1]);
    // rt.fov = 80 * M_PI / 180;
    rt.W = 1024;
    rt.H = 1024;
    rt.current_heap = rt.graphic_heap;
    save_img(&rt, render(&rt), rt.W, rt.H);
    close(rt.fd_file);
    free_heap(&rt);
    return 0;
}

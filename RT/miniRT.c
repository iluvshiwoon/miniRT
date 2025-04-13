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
#define intensity 300000000.0

bool second_degree_solve(const t_ray ray, const t_sphere sphere, double * s)
{
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

t_vec   get_color(t_ray ray, t_rt * rt, int nb_rebound) {
    t_vec P;
    t_vec N;
    t_vec pixel_intensity;
    int sphere_id = 0;
    double t;

    if (nb_rebound == 0)
        return (t_vec){0,0,0};
    if (intersections(ray, rt->scene, &P, &N, &sphere_id, &t))
    {
        // direct lighting 
        t_ray ray_light = {vec_plus(P,vec_mult(0.01,N)),normalize(vec_minus(rt->scene.light.origin,P))};
        t_vec P_light, N_light;
        int sphere_id_light = 0;
        double t_light;
        bool inter = intersections(ray_light,rt->scene, &P_light, &N_light, &sphere_id_light, &t_light);
        double d_light2 = norm2(vec_minus(rt->scene.light.origin,P));
        if (inter && t_light*t_light < d_light2)
            pixel_intensity = (t_vec){0,0,0};
        else
            pixel_intensity = vec_mult(intensity * fmax(0,vec_scal(normalize(vec_minus(rt->scene.light.origin, P)),N)) / d_light2,vec_mult(1/M_PI,rt->scene.spheres[sphere_id].albedo));

        // indirect lighting
        // double r1 = uniform_uint32(&rt->state);
        // double r2 = uniform_uint32(&rt->state);
        double r1 = double_rng(&rt->rng);
        double r2 = double_rng(&rt->rng);
        t_vec direction_random_local_basis = (t_vec){cos(2 * M_PI*r1)*sqrt(1-r2), sin(2*M_PI*r1)*sqrt(1-r2),sqrt(r2)};
        // t_vec random_vec = (t_vec){uniform_uint32(&rt->state)-0.5,uniform_uint32(&rt->state)-0.5,uniform_uint32(&rt->state)-0.5};
        t_vec random_vec = (t_vec){double_rng(&rt->rng)-0.5,double_rng(&rt->rng)-0.5,double_rng(&rt->rng)-0.5};
        t_vec tangent1 = normalize(cross(N,random_vec));
        t_vec tangent2 = cross(tangent1, N);
        t_vec direction_random = vec_plus(vec_plus(vec_mult(direction_random_local_basis.x,tangent1),vec_mult(direction_random_local_basis.y,tangent2)\
                                                           ),vec_mult(direction_random_local_basis.z,N));
        t_ray random_ray = (t_ray){vec_plus(P,vec_mult(0.001,N)),direction_random};
        pixel_intensity = vec_plus(pixel_intensity, vec_m_vec(get_color(random_ray,rt,nb_rebound-1),rt->scene.spheres[sphere_id].albedo));
    }
    return pixel_intensity;
}

unsigned char * render (t_rt * rt)
{
    unsigned char * image;
    int i;
    int nrays = 80;
    int j;
    int k;

    // double intensity = 1000000;
    image = wrap_malloc(rt, sizeof(unsigned char)*rt->W*rt->H*3);
    i = -1;
    while (++i < rt->H)
    {
        j = -1;
        while (++j < rt->W)
        {
            k = -1;
            t_vec direction = {j - rt->W / 2 - rt->scene.camera.origin.x, i - rt->H /2 -rt->scene.camera.origin.y, - rt->W / (2 * tan((rt->scene.camera.fov)/2))};
            direction = normalize(direction);
            t_ray ray = {{0.,0.,0.},direction};
            t_vec pixel_intensity = (t_vec){0.,0.,0.};

            while (++k < nrays)
                pixel_intensity = vec_plus(pixel_intensity,vec_mult(1.0/nrays,get_color(ray, rt, 5)));
            image[((rt->H-i-1)*rt->W + j) * 3 + 0] = fmin(255, fmax(0, pow(pixel_intensity.x,1/2.2)));
            image[((rt->H-i-1)*rt->W + j) * 3 + 1] = fmin(255, fmax(0, pow(pixel_intensity.y,1/2.2)));
            image[((rt->H-i-1)*rt->W + j) * 3 + 2] = fmin(255, fmax(0, pow(pixel_intensity.z,1/2.2)));
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
    // initialize_state(&rt.state,19650218UL);
    rt.current_heap = rt.parsing_heap;
    parsing_minirt(&rt,av[1]);
    rt.W = 1024;
    rt.H = 1024;
    rt.current_heap = rt.graphic_heap;
    save_img(&rt, render(&rt), rt.W, rt.H);
    // initialize_state(&state,19650218UL);
    // for (int i = 1000; i;i--)
    // {
    //     double uni = double_rng(&rt.rng);
    //     if (uni > 1)
    //         printf("failed\n");
    //     printf("%lf\n",uni);
    //     // uint32_t rn = pcg32_random_r(&rt.rng);
    //     // printf("%u\n",rn);
    // }
    close(rt.fd_file);
    free_heap(&rt);
    return 0;
}

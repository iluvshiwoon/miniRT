/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:28 by gschwand          #+#    #+#             */
/*   Updated: 2025/03/19 08:41:03 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

unsigned char * render_v2(t_rt * rt)
{
    unsigned char * image;
    int i;
    int j;

    image = wrap_malloc(rt, sizeof(unsigned char) * rt->W * rt->H * 3);
    if (!image)
        return(NULL);
    // rt->scene.light->intensity *= 1000000000;
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
            t_vec pixel_intensity;
            int sphere_id = 0;
            double t;
            if (intersections(ray, rt->scene, &P, &N, &sphere_id, &t))
            {
                // printf("sphere_id : %d\n", sphere_id);
                t_ray ray_light = {vec_plus(P,vec_mult(0.01,N)),normalize(vec_minus(*rt->scene.light->origin, P))};
                t_vec P_light;
                t_vec N_light;
                int sphere_id_light = 0;
                double t_light;
                bool inter = intersections(ray_light, rt->scene, &P_light, &N_light, &sphere_id_light, &t_light);
                double d_light2 = norm2(vec_minus(*rt->scene.light->origin, P));
                printf("d_light2 : %f\n", d_light2);

                // si le retour d'intersections est nul alors l'image sort sans couleur POURQUOI???
                if (!inter && t_light*t_light < d_light2)
                    pixel_intensity = (t_vec){0,0,0};
                else
                {
                    // if (rt->scene.sphere[sphere_id].albedo->x != rt->scene.sphere[sphere_id].albedo->y || rt->scene.sphere[sphere_id].albedo->x != rt->scene.sphere[sphere_id].albedo->z || rt->scene.sphere[sphere_id].albedo->y != rt->scene.sphere[sphere_id].albedo->z)
                    //     printf("couleur de la sphere r:%f g: %f b: %f \n", rt->scene.sphere[sphere_id].albedo->x, rt->scene.sphere[sphere_id].albedo->y, rt->scene.sphere[sphere_id].albedo->z);

                    // pixel_intensity = vec_mult(rt->scene.light->intensity * fmax(0,vec_scal(normalize(vec_minus(*rt->scene.light->origin, P)),N)) / d_light2, *rt->scene.sphere[sphere_id].albedo);

                    double light_factor = fmax(0, vec_scal(normalize(vec_minus(*rt->scene.light->origin, P)), N)) / (d_light2);
                    // if (light_factor > 0)
                    //     printf("\n-------------\nlight_factor : %f", light_factor);
                    
                    // double intensity = fmin(1.0, fmax(0.0, rt->scene.light->intensity));
                    
                    double intensity = fmin(1.0, fmax(0.0, rt->scene.light->intensity * light_factor));

                    // printf("\n-------------\nintensity : %f\n-------------\n", intensity);
                    pixel_intensity = vec_mult(intensity, *rt->scene.sphere[sphere_id].albedo);
                    // printf("pixel_intensity.y : %f\n", pixel_intensity.y);
                    // printf("pixel_intensity.z : %f\n", pixel_intensity.z);
                }
            }
            // printf("---------> pow(pixel_intensity.x,1/2.2) : %f\n", pow(pixel_intensity.x, 1/2.2));

            pixel_intensity.x = pow(pixel_intensity.x, 1/2.2);
            pixel_intensity.y = pow(pixel_intensity.y, 1/2.2);
            pixel_intensity.z = pow(pixel_intensity.z, 1/2.2);

            pixel_intensity = vec_mult(255, pixel_intensity);
            
            // printf("---------> fmin(255, fmax(0, pow(pixel_intensity.x,1/2.2))) : %f\n", fmin(255, fmax(0, pow(pixel_intensity.x,1/2.2))));
            image[((rt->H-i-1)*rt->W + j) * 3 + 0] = fmin(255, fmax(0, pixel_intensity.x));
            printf("pixel_intensity.x : %f\n", pixel_intensity.x);
            image[((rt->H-i-1)*rt->W + j) * 3 + 1] = fmin(255, fmax(0, pixel_intensity.y));
            printf("pixel_intensity.y : %f\n", pixel_intensity.y);
            image[((rt->H-i-1)*rt->W + j) * 3 + 2] = fmin(255, fmax(0, pixel_intensity.z));
            printf("pixel_intensity.z : %f\n", pixel_intensity.z);
        }
    }
    return image;
}

int	mini_rt(char *file)
{
	t_rt	*rt;
	
	rt = parsing_minirt(file);
    if (!rt)
        return (1);
    rt->fov = 80 * M_PI / 180;
    rt->W = 1024;
    rt->H = 1024;
    save_img(rt, render_v2(rt), rt->W, rt->H);
    free_heap(rt);
	return (0);
}

int	main(int ac, char **av)
{
	if (check_args(ac, av))
		return (1);
	if (mini_rt(av[1]))
		return (1);
	return (0);
}

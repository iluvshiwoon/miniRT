/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:48:28 by gschwand          #+#    #+#             */
/*   Updated: 2025/03/17 16:45:00 by gschwand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

unsigned char * render_v2 (t_rt * rt)
{
    unsigned char * image;
    int i;
    int j;

    image = wrap_malloc(rt, sizeof(unsigned char) * rt->W * rt->H * 3);
    if (!image)
        return(NULL);
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
                t_ray ray_light = {vec_plus(P,vec_mult(0.01,N)),normalize(vec_minus(*rt->scene.light->origin, P))};
                t_vec P_light;
                t_vec N_light;
                int sphere_id_light = 0;
                double t_light;
                bool inter = intersections(ray_light,rt->scene, &P_light, &N_light, &sphere_id_light, &t_light);
                double d_light2 = norm2(vec_minus(*rt->scene.light->origin,P));
                if (inter && t_light*t_light < d_light2)
                    pixel_intensity = (t_vec){0,0,0};
                else
                    pixel_intensity = vec_mult(rt->scene.light->intensity * fmax(0,vec_scal(normalize(vec_minus(*rt->scene.light->origin, P)),N)) / d_light2, *rt->scene.sphere[sphere_id].albedo);
            }
            image[((rt->H-i-1)*rt->W + j) * 3 + 0] = fmin(255, fmax(0, pow(pixel_intensity.x,1/2.2)));
            image[((rt->H-i-1)*rt->W + j) * 3 + 1] = fmin(255, fmax(0, pow(pixel_intensity.y,1/2.2)));
            image[((rt->H-i-1)*rt->W + j) * 3 + 2] = fmin(255, fmax(0, pow(pixel_intensity.z,1/2.2)));
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

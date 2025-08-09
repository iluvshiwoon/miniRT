/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kershuen <kershuen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 10:00:00 by kershuen          #+#    #+#             */
/*   Updated: 2025/07/26 10:00:00 by kershuen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniRT.h"
#include "../minilibx-linux/mlx.h"

void	load_normal_maps(t_rt *rt)
{
	int	i;
	int	width;
	int	height;

	i = 0;
	while (i < rt->scene.total_objects)
	{
		if (rt->scene.objects[i].normal_map_path)
		{
			printf("Loading normal map for object %d: %s\n", i, rt->scene.objects[i].normal_map_path);
			rt->scene.objects[i].normal_map.img = mlx_xpm_file_to_image(rt->mlx,
					rt->scene.objects[i].normal_map_path, &rt->scene.objects[i].normal_map.width, &rt->scene.objects[i].normal_map.height);
			if (!rt->scene.objects[i].normal_map.img)
				exit_error(rt, "Error: Could not load normal map");
			rt->scene.objects[i].normal_map.addr = mlx_get_data_addr(
					rt->scene.objects[i].normal_map.img,
					&rt->scene.objects[i].normal_map.bits_per_pixel,
					&rt->scene.objects[i].normal_map.line_length,
					&rt->scene.objects[i].normal_map.endian);
		}
		i++;
	}
}

void	free_normal_maps(t_rt *rt)
{
	int	i;

	i = 0;
	while (i < rt->scene.total_objects)
	{
		if (rt->scene.objects[i].normal_map.img)
			mlx_destroy_image(rt->mlx, rt->scene.objects[i].normal_map.img);
		i++;
	}
} 
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwand <gschwand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:35:47 by gschwand          #+#    #+#             */
/*   Updated: 2025/07/25 13:20:07 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../miniRT.h"

static char	*append_cylinder_dims(t_rt *rt, char *r_value, \
    t_cylinder *cylinder)
{
    char    dest[24 + 1];

    ft_memset(dest, 0, sizeof(dest));
    fpconv_dtoa(cylinder->radius, dest);
    r_value = rt_ft_strjoin(rt, r_value, dest);
    ft_memset(dest, 0, sizeof(dest));
    r_value = rt_ft_strjoin(rt, r_value, " ");
    fpconv_dtoa(cylinder->height, dest);
    return (rt_ft_strjoin(rt, r_value, dest));
}

static char	*append_optional_maps_cy(t_rt *rt, char *r_value, \
    const struct s_object object)
{
    if (object.normal_map_path || object.texture_map_path || \
        object.texture_scale.x != 1.0 || object.texture_scale.y != 1.0)
    {
        r_value = rt_ft_strjoin(rt, r_value, " ");
        if (object.normal_map_path)
            r_value = rt_ft_strjoin(rt, r_value, object.normal_map_path);
        else
            r_value = rt_ft_strjoin(rt, r_value, ".");
        r_value = rt_ft_strjoin(rt, r_value, " ");
        if (object.texture_map_path)
            r_value = rt_ft_strjoin(rt, r_value, object.texture_map_path);
        else
            r_value = rt_ft_strjoin(rt, r_value, ".");
        r_value = rt_ft_strjoin(rt, r_value, " ");
        r_value = rt_ft_strjoin(rt, r_value, \
            vec_toa(rt, object.texture_scale));
    }
    return (r_value);
}

static void	parse_cylinder_optional(t_rt *rt, char **tab, int *id)
{
    if (tab[6])
    {
        if (ft_strncmp(tab[6], ".", 2) != 0)
            rt->scene.objects[*id].specular = vec_mult(1.0 / 255, \
                parse_color(rt, tab[6]));
        if (tab[7])
        {
            if (ft_strncmp(tab[7], ".", 2) != 0)
                rt->scene.objects[*id].shininess = \
                    ft_atoi_double(tab[7]);
            if (tab[8])
            {
                if (ft_strncmp(tab[8], ".", 2) != 0)
                    rt->scene.objects[*id].normal_map_path = \
                        rt_ft_strdup(rt, tab[8]);
                if (tab[9])
                {
                    if (ft_strncmp(tab[9], ".", 2) != 0)
                        rt->scene.objects[*id].texture_map_path = \
                            rt_ft_strdup(rt, tab[9]);
                    if (tab[10])
                        rt->scene.objects[*id].texture_scale = \
                            parse_vec(rt, tab[10]);
                }
            }
        }
    }
}

void	rotate_cylinder_local(t_rt *rt, int id, t_rvec rvec)
{
	t_cl	cl;

	cl.cyl = rt->scene.objects[id].obj;
	cl.axis = normalize(cl.cyl->dir);
	cl.temp = (t_vec){0, 1, 0};
	if (fabs(cl.axis.y) > 0.99)
		cl.temp = (t_vec){1, 0, 0};
	cl.right = normalize(cross(cl.temp, cl.axis));
	cl.forward = cross(cl.axis, cl.right);
	if (fabs(rvec.pitch) > 1e-6)
	{
		cl.pitch_rot = create_rotation_axis(cl.right, rvec.pitch);
		cl.cyl->dir = normalize(mat3_multiply_vec(cl.pitch_rot,
					cl.cyl->dir));
	}
	if (fabs(rvec.yaw) > 1e-6)
	{
		cl.yaw_rot = create_rotation_axis(cl.forward, rvec.yaw);
		cl.cyl->dir = normalize(mat3_multiply_vec(cl.yaw_rot, cl.cyl->dir));
	}
	rt->scene.objects[id].string = rt->scene.objects[id].display_string(rt,
			rt->scene.objects[id]);
	if (rvec.roll == 0)
        atomic_store(&rt->state.re_render_scene, true);
}

void	translate_cylinder(t_rt *rt, int id, t_vec vec)
{
	t_cylinder	*cylinder;

	cylinder = rt->scene.objects[id].obj;
	cylinder->origin = vec_plus(cylinder->origin, vec);
	rt->scene.objects[id].string = rt->scene.objects[id].display_string(rt,
			rt->scene.objects[id]);
    atomic_store(&rt->state.re_render_scene, true);
}

char	*string_cylinder(t_rt *rt, const struct s_object object)
{
	char		*r_value;
	t_cylinder	*cylinder;

	cylinder = object.obj;
	r_value = rt_ft_strjoin(rt, "cy  ", vec_toa(rt, cylinder->origin));
	r_value = rt_ft_strjoin(rt, r_value, "  ");
	r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, cylinder->dir));
	r_value = rt_ft_strjoin(rt, r_value, " ");
    r_value = append_cylinder_dims(rt, r_value, cylinder);
	r_value = rt_ft_strjoin(rt, r_value, "  ");
	r_value = rt_ft_strjoin(rt, r_value, color_toa(rt, object.albedo));
	r_value = rt_ft_strjoin(rt, r_value, " ");
	r_value = rt_ft_strjoin(rt, r_value, color_toa(rt, object.specular));
	r_value = rt_ft_strjoin(rt, r_value, " ");
    {
        char dest2[24 + 1];
        ft_memset(dest2, 0, sizeof(dest2));
        fpconv_dtoa(object.shininess, dest2);
        r_value = rt_ft_strjoin(rt, r_value, dest2);
    }
    r_value = append_optional_maps_cy(rt, r_value, object);
	return (r_value);
}

void	fill_cy(t_rt *rt, t_cylinder *cylinder, int id)
{
	rt->scene.objects[id].obj = cylinder;
	rt->scene.objects[id].id = id;
	rt->scene.objects[id].type = cy;
	rt->scene.objects[id].display_string = &string_cylinder;
	rt->scene.objects[id].rotate = &rotate_cylinder_local;
	rt->scene.objects[id].translate = &translate_cylinder;
	rt->scene.objects[id].string = string_cylinder(rt, \
			rt->scene.objects[id]);
	// Initialize material properties with default values
	rt->scene.objects[id].checkerboard = false; // Default no checkerboard
}

void	parse_cylinder(t_rt *rt, char *line, int *id)
{
	char		**tab;
	t_cylinder	*cylinder;

	cylinder = wrap_malloc(rt, sizeof(*cylinder));
	tab = rt_ft_split(rt, line, ' ');
	if (tab[1] && tab[2] && tab[3] && tab[4] && tab[5])
	{
		rt->scene.objects[*id].texture_map_path = NULL;
		rt->scene.objects[*id].normal_map_path = NULL;
		cylinder->origin = parse_vec(rt, tab[1]);
		cylinder->dir = normalize(parse_vec(rt, tab[2]));
		cylinder->radius = ft_atoi_double(tab[3]);
		if (cylinder->radius < 0)
			exit_error(rt, "Error: Invalid radius for cylinders");
		cylinder->height = ft_atoi_double(tab[4]);
		if (cylinder->height < 0)
			exit_error(rt, "Error: Invalid height for cylinders");
		rt->scene.objects[*id].is_intersection = &is_intersection_cylinder;
		rt->scene.objects[*id].albedo = vec_mult(1.0 / 255, parse_color(rt,
					tab[5]));
		rt->scene.objects[*id].specular = (t_vec){0.5, 0.5, 0.5};
		rt->scene.objects[*id].shininess = 32.0;
		rt->scene.objects[*id].texture_scale = (t_vec){1.0, 1.0, 1.0};
		rt->scene.objects[*id].checkerboard = false;
        parse_cylinder_optional(rt, tab, id);
		fill_cy(rt, cylinder, *id);
		(*id)++;
		return ;
	}
	exit_error(rt, "Error: Invalid number of arguments for cylinders\n");
}

#include "../../miniRT.h"

char	*append_cylinder_dims(t_rt *rt, char *r_value, t_cylinder *cylinder)
{
	char	dest[24 + 1];

	ft_memset(dest, 0, sizeof(dest));
	fpconv_dtoa(cylinder->radius, dest);
	r_value = rt_ft_strjoin(rt, r_value, dest);
	ft_memset(dest, 0, sizeof(dest));
	r_value = rt_ft_strjoin(rt, r_value, " ");
	fpconv_dtoa(cylinder->height, dest);
	return (rt_ft_strjoin(rt, r_value, dest));
}

char	*append_optional_maps_cy(t_rt *rt, char *r_value,
		const struct s_object object)
{
	if (object.normal_map_path || object.texture_map_path
		|| object.texture_scale.x != 1.0 || object.texture_scale.y != 1.0)
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
		r_value = rt_ft_strjoin(rt, r_value, vec_toa(rt, object.texture_scale));
	}
	return (r_value);
}

void	parse_cylinder_optional1(t_rt *rt, char **tab, int *id)
{
    if (tab[9])
    {
        if (ft_strncmp(tab[9], ".", 2) != 0)
            rt->scene.objects[*id].texture_map_path = \
                rt_ft_strdup(rt, tab[9]);
        if (tab[10])
            rt->scene.objects[*id].texture_scale = parse_vec(rt,
                                                             tab[10]);
    }
}

void	parse_cylinder_optional(t_rt *rt, char **tab, int *id)
{
	if (tab[6])
	{
		if (ft_strncmp(tab[6], ".", 2) != 0)
			rt->scene.objects[*id].specular = vec_mult(1.0 / 255,
					parse_color(rt, tab[6]));
		if (tab[7])
		{
			if (ft_strncmp(tab[7], ".", 2) != 0)
				rt->scene.objects[*id].shininess = ft_atoi_double(tab[7]);
			if (tab[8])
			{
				if (ft_strncmp(tab[8], ".", 2) != 0)
					rt->scene.objects[*id].normal_map_path = rt_ft_strdup(rt,
							tab[8]);
                parse_cylinder_optional1(rt, tab, id);
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
		cl.cyl->dir = normalize(mat3_multiply_vec(cl.pitch_rot, cl.cyl->dir));
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

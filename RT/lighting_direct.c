#include "../miniRT.h"

static void	prepare_light_ray(t_rt *rt, t_get_color *gc, int i)
{
    gc->ray_light = (t_ray){vec_plus(gc->intersection.point, \
            vec_mult(EPSILON, gc->intersection.normal)), \
        normalize(vec_minus(rt->scene.lights[i]->origin, \
                gc->intersection.point))};
    gc->obj_id_light = 0;
    gc->inter = visible_intersection(gc->ray_light, rt->scene, \
            &(gc->intersection_light), &(gc->obj_id_light));
    gc->d_light2 = norm2(vec_minus(rt->scene.lights[i]->origin, \
                gc->intersection.point));
}

static t_vec	compute_direct_contribution(t_rt *rt, t_get_color *gc, int i)
{
    t_vec   contrib;

    contrib = (t_vec){0, 0, 0};
    if (!(gc->inter && gc->intersection_light.t * gc->intersection_light.t \
        < gc->d_light2))
    {
        gc->cos_theta = fmax(0, vec_scal(normalize(
                        vec_minus(rt->scene.lights[i]->origin, \
                            gc->intersection.point)), \
                    gc->intersection.normal));
        gc->light_contribution = vec_mult(rt->scene.lights[i]->intensity \
                * EXPOSURE * gc->cos_theta / gc->d_light2, \
                rt->scene.lights[i]->color);
        contrib = gc->light_contribution;
    }
    return (contrib);
}

t_vec	calculate_direct_lighting(t_rt *rt, t_get_color *gc)
{
    t_vec	total_light_contribution;
    int		i;

    total_light_contribution = (t_vec){0, 0, 0};
    i = 0;
    while (i < rt->scene.num_lights)
    {
        prepare_light_ray(rt, gc, i);
        total_light_contribution = vec_plus(total_light_contribution, \
                compute_direct_contribution(rt, gc, i));
        i++;
    }
    return (vec_mult(1 / M_PI, vec_m_vec(total_light_contribution, \
                get_material_color(&rt->scene.objects[gc->obj_id], \
                    gc->intersection.point, &gc->intersection))));
}

t_vec	calculate_ambient_lighting(t_rt *rt, t_get_color *gc)
{
    gc->ambient_contribution = vec_mult(GLOBAL_EXPOSURE \
            * rt->scene.ambient_light.intensity, \
            rt->scene.ambient_light.color);
    return (vec_mult(1 / M_PI, vec_m_vec(gc->ambient_contribution, \
                get_material_color(&rt->scene.objects[gc->obj_id], \
                    gc->intersection.point, &gc->intersection))));
}



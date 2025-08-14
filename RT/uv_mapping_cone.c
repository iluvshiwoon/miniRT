#include "../miniRT.h"

static void get_cone_body_uv(t_object *obj, t_vec p, double *u, double *v)
{
    t_cone  *co;
    t_vec   d;
    t_vec   u_axis;
    t_vec   v_axis;
    t_vec   axis_component;
    t_vec   radial;
    double  h;
    double  theta;

    co = (t_cone *)obj->obj;
    d = vec_minus(p, co->origin);
    if (fabs(co->dir.y) > 0.99)
        u_axis = (t_vec){1, 0, 0};
    else
        u_axis = normalize(cross((t_vec){0, 1, 0}, co->dir));
    v_axis = normalize(cross(co->dir, u_axis));
    h = vec_scal(d, co->dir);
    if (h < 0.0)
        h = 0.0;
    if (h > co->height)
        h = co->height;
    axis_component = vec_mult(h, co->dir);
    radial = vec_minus(d, axis_component);
    if (norm2(radial) < 1e-12)
        theta = 0.0;
    else
        theta = atan2(vec_scal(radial, v_axis), vec_scal(radial, u_axis));
    *u = (theta + M_PI) / (2.0 * M_PI);
    *v = h / co->height;
    if (obj->texture_scale.x != 0.0)
        *u = fmod(*u / obj->texture_scale.x, 1.0);
    else
        *u = fmod(*u, 1.0);
    if (obj->texture_scale.y != 0.0)
        *v = fmod(*v / obj->texture_scale.y, 1.0);
    else
        *v = fmod(*v, 1.0);
    if (*u < 0.0) *u += 1.0;
    if (*v < 0.0) *v += 1.0;
}

static void get_cone_cap_uv(t_object *obj, t_vec p, double *u, double *v)
{
    t_cone  *co;
    t_vec   base_center;
    t_vec   u_axis;
    t_vec   v_axis;
    t_vec   d;

    co = (t_cone *)obj->obj;
    base_center = vec_plus(co->origin, vec_mult(co->height, co->dir));
    d = vec_minus(p, base_center);
    if (fabs(co->dir.y) > 0.99)
        u_axis = (t_vec){1, 0, 0};
    else
        u_axis = normalize(cross((t_vec){0, 1, 0}, co->dir));
    v_axis = normalize(cross(co->dir, u_axis));
    *u = (vec_scal(d, u_axis) / co->radius + 1.0) / 2.0;
    *v = (vec_scal(d, v_axis) / co->radius + 1.0) / 2.0;
    if (*u < 0.0) *u = 0.0;
    if (*u > 1.0) *u = 1.0;
    if (*v < 0.0) *v = 0.0;
    if (*v > 1.0) *v = 1.0;
}

void    get_cone_uv(t_object *obj, t_intersection *intersection, double *u, double *v)
{
    if (intersection->hit_cap)
        get_cone_cap_uv(obj, intersection->point, u, v);
    else
        get_cone_body_uv(obj, intersection->point, u, v);
}

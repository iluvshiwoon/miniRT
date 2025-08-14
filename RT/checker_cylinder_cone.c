#include "../miniRT.h"

static void	cylinder_local_basis(t_cylinder *cylinder, t_vec *axis, \
    t_vec *right, t_vec *up)
{
    t_vec   temp;

    *axis = normalize(cylinder->dir);
    temp = (fabs(axis->y) < 0.99) ? (t_vec){0, 1, 0} : (t_vec){1, 0, 0};
    *right = normalize(cross(temp, *axis));
    *up = normalize(cross(*axis, *right));
}

static t_checker_idx	cylinder_checker_indices(t_cyl_idx_args a)
{
    double  x;
    double  y;
    double  z;
    double  theta;
    double  height_ratio;
    t_checker_idx idx;

    x = vec_scal(a.to_point, a.right);
    y = vec_scal(a.to_point, a.up);
    z = vec_scal(a.to_point, a.axis);
    theta = atan2(y, x);
    height_ratio = z / a.cylinder->height;
    idx.a = (int)floor((theta + M_PI) / (2 * M_PI) * 8.0);
    idx.b = (int)floor(height_ratio * 4.0);
    return (idx);
}

t_vec	cylinder_checker_color_from_idx(t_checker_idx idx, t_vec albedo)
{
    if ((idx.a + idx.b) % 2 == 0)
        return ((t_vec){1.0, 1.0, 1.0});
    return (albedo);
}

t_vec	get_cylinder_checkerboard(t_vec point, t_vec albedo, \
    t_cylinder *cylinder)
{
    t_vec           axis;
    t_vec           right;
    t_vec           up;
    t_vec           to_point;
    t_cyl_idx_args  a;

    to_point = vec_minus(point, cylinder->origin);
    cylinder_local_basis(cylinder, &axis, &right, &up);
    a = (t_cyl_idx_args){cylinder, to_point, axis, right, up};
    return (cylinder_checker_color_from_idx(cylinder_checker_indices(a), \
        albedo));
}

static void	cone_local_basis(t_cone *cone, t_vec *axis, t_vec *right, \
    t_vec *up)
{
    t_vec   temp;

    *axis = normalize(cone->dir);
    temp = (fabs(axis->y) < 0.99) ? (t_vec){0, 1, 0} : (t_vec){1, 0, 0};
    *right = normalize(cross(temp, *axis));
    *up = normalize(cross(*axis, *right));
}

static t_checker_idx	cone_checker_indices(t_cone_idx_args a)
{
    double  x;
    double  y;
    double  z;
    double  theta;
    double  height_ratio;
    t_checker_idx idx;

    x = vec_scal(a.to_point, a.right);
    y = vec_scal(a.to_point, a.up);
    z = vec_scal(a.to_point, a.axis);
    theta = atan2(y, x);
    height_ratio = z / a.cone->height;
    idx.a = (int)floor((theta + M_PI) / (2 * M_PI) * 8.0);
    idx.b = (int)floor(height_ratio * 4.0);
    return (idx);
}

t_vec	get_cone_checkerboard(t_vec point, t_vec albedo, t_cone *cone)
{
    t_vec           axis;
    t_vec           right;
    t_vec           up;
    t_vec           to_point;
    t_cone_idx_args a;

    to_point = vec_minus(point, cone->origin);
    cone_local_basis(cone, &axis, &right, &up);
    a = (t_cone_idx_args){cone, to_point, axis, right, up};
    return (cylinder_checker_color_from_idx(cone_checker_indices(a), albedo));
}

t_vec	get_cylinder_cap_checkerboard(t_vec point, t_vec albedo, t_cylinder *cylinder)
{
    (void)point;
    (void)cylinder;
    return (albedo);
}

t_vec	get_cone_cap_checkerboard(t_vec point, t_vec albedo, t_cone *cone)
{
    (void)point;
    (void)cone;
    return (albedo);
}



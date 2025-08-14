#include "../miniRT.h"

t_vec	get_checkerboard_color(t_vec point, t_vec albedo)
{
    t_chk3  c;
    t_vec   checker_color;

    c.scale = 0.5;
    c.checker_x = (int)(point.x * c.scale);
    c.checker_y = (int)(point.y * c.scale);
    c.checker_z = (int)(point.z * c.scale);
    if ((c.checker_x + c.checker_y + c.checker_z) % 2 == 0)
        checker_color = (t_vec){1.0, 1.0, 1.0};
    else
        checker_color = albedo;
    return (checker_color);
}

t_vec	get_sphere_checkerboard(t_vec point, t_vec albedo, t_sphere *sphere)
{
    t_sph_chk   s;
    t_vec       checker_color;

    s.u = atan2(point.z - sphere->origin.z, point.x - sphere->origin.x) \
        / (2 * M_PI);
    s.v = acos((point.y - sphere->origin.y) / sphere->radius) / M_PI;
    s.scale = 8.0;
    s.checker_u = (int)(s.u * s.scale);
    s.checker_v = (int)(s.v * s.scale);
    if ((s.checker_u + s.checker_v) % 2 == 0)
        checker_color = (t_vec){1.0, 1.0, 1.0};
    else
        checker_color = albedo;
    return (checker_color);
}



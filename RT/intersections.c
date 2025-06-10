#include "../miniRT.h"
#define intensity 300000000.0

bool sphere_second_degree_solve(const t_ray ray, const t_object obj, double * s)
{
    double a[3];
    double t[2];
    double delta; 
    t_sphere sphere;
    sphere = *((t_sphere *){obj.obj});

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

bool is_intersection(const t_ray ray, const t_object obj, t_intersection * intersection)
{
    bool has_sol;
    
    has_sol = obj.intersection(ray, obj, &intersection->t);
    if (has_sol == true)
    {
        intersection->point = vec_plus(ray.origin, vec_mult(intersection->t,ray.direction)); 
        intersection->normal = normalize(vec_minus(intersection->point, (t_sphere *){obj.obj}->origin));
    }
    return (has_sol);
}

bool visible_intersection(const t_ray ray, t_scene scene,t_intersection * intersection, int * obj_id)
{
    t_intersection local_intersection;
    bool has_inter;

    intersection->t = 1e99;
    has_inter = false;
    while (scene.total_objects)
    {
        if (is_intersection(ray, scene.objects[scene.total_objects - 1],&local_intersection))
        {
            has_inter = true;
            if (local_intersection.t < intersection->t)
            {
                intersection->t = local_intersection.t;
                intersection->point = local_intersection.point;
                intersection->normal = local_intersection.normal;
                *obj_id = scene.total_objects - 1;
            }
        }
        scene.total_objects--;
    }
    return has_inter;
}

// P intersection point
// N vector normal to P
// t directioin coefficient in vector O + t*u with 0 the origin of the vector and u the unit direction vector
t_vec   get_color(t_ray ray, t_rt * rt, int nb_rebound) {
    t_intersection intersection;
    t_vec pixel;
    int obj_id = 0;

    if (nb_rebound == 0)
        return (t_vec){0,0,0};
    if (visible_intersection(ray, rt->scene,&intersection,&obj_id))
    {
        // direct lighting 
        t_ray ray_light = {vec_plus(intersection.point,vec_mult(0.01,intersection.normal)),normalize(vec_minus(rt->scene.light.origin,intersection.point))};
        t_intersection intersection_light;
        int obj_id_light = 0;
        bool inter = visible_intersection(ray_light,rt->scene,&intersection_light,&obj_id_light);
        double d_light2 = norm2(vec_minus(rt->scene.light.origin,intersection.point));
        if (inter && intersection_light.t*intersection_light.t < d_light2)
            pixel = (t_vec){0,0,0};
        else
            pixel = vec_mult(intensity * fmax(0,vec_scal(normalize(vec_minus(rt->scene.light.origin, intersection.point)),intersection.normal)) / d_light2,vec_mult(1/M_PI,rt->scene.objects[obj_id].albedo));

        // indirect lighting
        double r1 = double_rng(&rt->rng);
        double r2 = double_rng(&rt->rng);
        t_vec direction_random_local_basis = (t_vec){cos(2 * M_PI*r1)*sqrt(1-r2), sin(2*M_PI*r1)*sqrt(1-r2),sqrt(r2)};
        t_vec random_vec = (t_vec){double_rng(&rt->rng)-0.5,double_rng(&rt->rng)-0.5,double_rng(&rt->rng)-0.5};
        t_vec tangent1 = normalize(cross(intersection.normal,random_vec));
        t_vec tangent2 = cross(tangent1, intersection.normal);
        t_vec direction_random = vec_plus(vec_plus(vec_mult(direction_random_local_basis.x,tangent1),vec_mult(direction_random_local_basis.y,tangent2)\
                                                           ),vec_mult(direction_random_local_basis.z,intersection.normal));
        t_ray random_ray = (t_ray){vec_plus(intersection.point,vec_mult(0.001,intersection.normal)),direction_random};
        pixel = vec_plus(pixel, vec_m_vec(get_color(random_ray,rt,nb_rebound-1),rt->scene.objects[obj_id].albedo));
    }
    return pixel;
}

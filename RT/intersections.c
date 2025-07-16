#include "../miniRT.h"
#define EXPOSURE 300000000.0
#define GLOBAL_EXPOSURE 1000000.0
#define epsilon 1e-6

bool cylinder_intersection_solve(const t_ray ray, const t_cylinder cylinder, double * t)
{
    // Vector from ray origin to cylinder origin
    t_vec oc = vec_minus(ray.origin, cylinder.origin);
    
    // Project ray direction and oc onto the plane perpendicular to cylinder axis
    t_vec ray_dir_proj = vec_minus(ray.direction, vec_mult(vec_scal(ray.direction, cylinder.direction), cylinder.direction));
    t_vec oc_proj = vec_minus(oc, vec_mult(vec_scal(oc, cylinder.direction), cylinder.direction));
    
    // Quadratic equation coefficients for infinite cylinder
    double a = vec_scal(ray_dir_proj, ray_dir_proj);
    double half_b = vec_scal(ray_dir_proj, oc_proj);
    double c = vec_scal(oc_proj, oc_proj) - cylinder.radius * cylinder.radius;
    
    // Check if ray is parallel to cylinder (a ≈ 0)
    if (fabs(a) < epsilon)
        return false;
    
    double discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return false;
    
    double sqrt_discriminant = sqrt(discriminant);
    double t1 = (-half_b - sqrt_discriminant) / a;
    double t2 = (-half_b + sqrt_discriminant) / a;
    
    // Check both intersection points for height constraints
    double candidate_t = -1;
    
    // Check first intersection point
    if (t1 > epsilon)
    {
        t_vec hit_point = vec_plus(ray.origin, vec_mult(t1, ray.direction));
        t_vec to_hit = vec_minus(hit_point, cylinder.origin);
        double height_proj = vec_scal(to_hit, cylinder.direction);
        
        if (height_proj >= 0 && height_proj <= cylinder.height)
            candidate_t = t1;
    }
    
    // Check second intersection point if first didn't work
    if (candidate_t < 0 && t2 > epsilon)
    {
        t_vec hit_point = vec_plus(ray.origin, vec_mult(t2, ray.direction));
        t_vec to_hit = vec_minus(hit_point, cylinder.origin);
        double height_proj = vec_scal(to_hit, cylinder.direction);
        
        if (height_proj >= 0 && height_proj <= cylinder.height)
            candidate_t = t2;
    }
    
    if (candidate_t > 0)
    {
        *t = candidate_t;
        return true;
    }
    
    return false;
}

static bool cylinder_cap_intersection(const t_ray ray, const t_cylinder cylinder, double *t, t_vec *normal)
{
    t_plane cap_planes[2];
    cap_planes[0].origin = cylinder.origin;
    cap_planes[0].normal = vec_mult(-1, cylinder.direction);
    cap_planes[1].origin = vec_plus(cylinder.origin, vec_mult(cylinder.height, cylinder.direction));
    cap_planes[1].normal = cylinder.direction;

    double min_t = -1.0;

    for (int i = 0; i < 2; i++)
    {
        double current_t;
        if (plane_intersection_solve(ray, cap_planes[i], &current_t) && current_t > epsilon)
        {
            t_vec p = vec_plus(ray.origin, vec_mult(current_t, ray.direction));
            if (norm2(vec_minus(p, cap_planes[i].origin)) <= cylinder.radius * cylinder.radius)
            {
                if (min_t < 0 || current_t < min_t)
                {
                    min_t = current_t;
                    *normal = cap_planes[i].normal;
                }
            }
        }
    }

    if (min_t > 0)
    {
        *t = min_t;
        return true;
    }
    return false;
}

bool is_intersection_cylinder(const t_ray ray, const t_object obj, t_intersection * intersection)
{
    t_cylinder cylinder = *((t_cylinder *){obj.obj});
    double body_t;
    bool body_has_sol = cylinder_intersection_solve(ray, cylinder, &body_t);
    double cap_t;
    t_vec cap_normal;
    bool cap_has_sol = cylinder_cap_intersection(ray, cylinder, &cap_t, &cap_normal);
    bool has_sol = false;
    double final_t = -1;
    bool hit_cap = false;

    if (body_has_sol && cap_has_sol) {
        has_sol = true;
        if (body_t < cap_t) {
            final_t = body_t;
        } else {
            final_t = cap_t;
            hit_cap = true;
        }
    } else if (body_has_sol) {
        has_sol = true;
        final_t = body_t;
    } else if (cap_has_sol) {
        has_sol = true;
        final_t = cap_t;
        hit_cap = true;
    }

    if (has_sol) {
        intersection->t = final_t;
        intersection->point = vec_plus(ray.origin, vec_mult(intersection->t, ray.direction));
        if (hit_cap) {
            intersection->normal = cap_normal;
        } else {
            t_vec to_point = vec_minus(intersection->point, cylinder.origin);
            t_vec axis_projection = vec_mult(vec_scal(to_point, cylinder.direction), cylinder.direction);
            intersection->normal = normalize(vec_minus(to_point, axis_projection));
        }
        t_vec to_ray_origin = normalize(vec_minus(ray.origin, intersection->point));
        if (vec_scal(intersection->normal, to_ray_origin) < 0) {
            intersection->normal = vec_mult(-1, intersection->normal);
        }
    }
    return has_sol;
}

bool plane_intersection_solve(const t_ray ray, const t_plane plane, double * t)
{
    double denominator;
    double numerator;
    
    denominator = vec_scal(ray.direction, plane.normal);
    
    // If denominator is zero (or very close to zero), ray is parallel to plane
    if (fabs(denominator) < epsilon)
        return false;
    numerator = vec_scal(vec_minus(plane.origin, ray.origin), plane.normal);
    *t = numerator / denominator;
    // Only return true if intersection is in front of ray origin (t > 0)
    return (*t > epsilon);  // Use small epsilon to avoid self-intersection
}

bool sphere_second_degree_solve(const t_ray ray, const t_sphere sphere, double * s)
{
    double half_b, c;
    double t[2];
    double delta; 

    // Quadratic equation: t² + bt + c = 0 (a = 1 since ray.direction is normalized)
    half_b = vec_scal(ray.direction, vec_minus(ray.origin, sphere.origin));
    c = norm2(vec_minus(ray.origin, sphere.origin)) - sphere.radius * sphere.radius;

    delta = half_b * half_b - c;

    if (delta < 0)
        return false;
    
    double sqrt_delta = sqrt(delta);
    t[0] = -half_b - sqrt_delta; // near intesection
    t[1] = -half_b + sqrt_delta; // far
    
    
    if (t[0] > epsilon)
        *s = t[0];  // Take the near intersection
    else if (t[1] > epsilon)
        *s = t[1];  // Take the far intersection (ray starts inside sphere)
    else
        return false;
    return true;
}

bool is_intersection_plane(const t_ray ray, const t_object obj, t_intersection * intersection)
{
    bool has_sol;
    t_plane plane;
    

    plane = *((t_plane *){obj.obj});
    has_sol = plane_intersection_solve(ray, plane, &intersection->t);
    if (has_sol == true)
    {
        intersection->point = vec_plus(ray.origin, vec_mult(intersection->t,ray.direction)); 
        intersection->normal = plane.normal;
        if (vec_scal(intersection->normal, vec_mult(-1, ray.direction)) < 0)
            intersection->normal = vec_mult(-1, intersection->normal);
    }
    return (has_sol);
}

bool is_intersection_sphere(const t_ray ray, const t_object obj, t_intersection * intersection)
{
    bool has_sol;
    t_sphere sphere;
    
    sphere = *((t_sphere *){obj.obj});
    has_sol = sphere_second_degree_solve(ray, sphere, &intersection->t);
    if (has_sol == true)
    {
        intersection->point = vec_plus(ray.origin, vec_mult(intersection->t,ray.direction)); 
        intersection->normal = normalize(vec_minus(intersection->point, sphere.origin));

        t_vec to_ray_origin = normalize(vec_minus(ray.origin, intersection->point));
            if (vec_scal(intersection->normal, to_ray_origin) < 0)
                intersection->normal = vec_mult(-1, intersection->normal);
    }
    return (has_sol);
}

bool visible_intersection(const t_ray ray, t_scene scene,t_intersection * intersection, int * obj_id)
{
    t_intersection local_intersection;
    t_object * obj;
    bool has_inter;

    intersection->t = 1e99;
    has_inter = false;
    while (scene.total_objects)
    {
        obj = &scene.objects[scene.total_objects - 1];
        if (obj->is_intersection != NULL && obj->is_intersection(ray, *obj,&local_intersection))
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
    t_vec pixel = (t_vec){};
    int obj_id = 0;

    if (nb_rebound == 0)
        return (t_vec){0,0,0};
    if (visible_intersection(ray, rt->scene,&intersection,&obj_id))
    {
        // direct lighting 
        t_ray ray_light = {vec_plus(intersection.point,vec_mult(epsilon,intersection.normal)),normalize(vec_minus(rt->scene.light.origin,intersection.point))};
        t_intersection intersection_light;
        int obj_id_light = 0;
        bool inter = visible_intersection(ray_light,rt->scene,&intersection_light,&obj_id_light);
        double d_light2 = norm2(vec_minus(rt->scene.light.origin,intersection.point));
        if (inter && intersection_light.t*intersection_light.t < d_light2)
            pixel = (t_vec){0,0,0};
        else
	{
		double cos_theta = fmax(0,vec_scal(normalize(vec_minus(rt->scene.light.origin,intersection.point)),intersection.normal));
		t_vec light_contribution = vec_mult(rt->scene.light.intensity * EXPOSURE * cos_theta / d_light2, rt->scene.light.color);
		pixel = vec_mult(1/M_PI, vec_m_vec(light_contribution, rt->scene.objects[obj_id].albedo));
            // pixel = vec_mult(EXPOSURE * fmax(0,vec_scal(normalize(vec_minus(rt->scene.light.origin, intersection.point)),intersection.normal)) / d_light2,vec_mult(1/M_PI,rt->scene.objects[obj_id].albedo));
	}
	t_vec ambient_contribution = vec_mult(GLOBAL_EXPOSURE * rt->scene.ambient_light.intensity  , rt->scene.ambient_light.color);
        t_vec ambient_light = vec_mult(1/M_PI, vec_m_vec(ambient_contribution, rt->scene.objects[obj_id].albedo));
        pixel = vec_plus(pixel, ambient_light);

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

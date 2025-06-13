#include "../miniRT.h"
#define intensity 300000000.0
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
    
    // Pre-calculate for height checks (assuming cylinder.direction is normalized)
    double val_oc_axis = vec_scal(oc, cylinder.direction);
    double val_rd_axis = vec_scal(ray.direction, cylinder.direction);

    // Check both intersection points for height constraints
    double candidate_t = -1;
    
    // Check first intersection point
    if (t1 > epsilon)
    {
        double height_proj = val_oc_axis + t1 * val_rd_axis;
        
        if (height_proj >= 0 && height_proj <= cylinder.height)
            candidate_t = t1;
    }
    
    // Check second intersection point if first didn't work
    if (candidate_t < 0 && t2 > epsilon)
    {
        double height_proj = val_oc_axis + t2 * val_rd_axis;
        
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

bool is_intersection_cylinder(const t_ray ray, const t_object obj, t_intersection * intersection)
{
    bool has_sol;
    t_cylinder cylinder;
    
    cylinder = *((t_cylinder *){obj.obj});
    has_sol = cylinder_intersection_solve(ray, cylinder, &intersection->t);
    
    if (has_sol == true)
    {
        intersection->point = vec_plus(ray.origin, vec_mult(intersection->t, ray.direction));
        
        // Calculate normal: point on cylinder surface perpendicular to axis
        t_vec to_point = vec_minus(intersection->point, cylinder.origin);
        t_vec axis_projection = vec_mult(vec_scal(to_point, cylinder.direction), cylinder.direction);
        intersection->normal = normalize(vec_minus(to_point, axis_projection));
        
        // Ensure normal points toward ray origin
        t_vec to_ray_origin = normalize(vec_minus(ray.origin, intersection->point));
        if (vec_scal(intersection->normal, to_ray_origin) < 0)
            intersection->normal = vec_mult(-1, intersection->normal);
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
        if (obj->is_intersection(ray, *obj,&local_intersection))
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
        t_ray ray_light = {vec_plus(intersection.point,vec_mult(epsilon,intersection.normal)),normalize(vec_minus(rt->scene.light.origin,intersection.point))};
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
        t_vec tangent1, tangent2;
        // Pick a vector 'v_up' that is not parallel to 'intersection.normal'.
        // If normal is (0,1,0), use (1,0,0) as v_up. Otherwise use (0,1,0).
        t_vec v_up = (t_vec){0.0, 1.0, 0.0};
        if (fabs(intersection.normal.y) > 0.9999) { // If normal is primarily along Y
            v_up = (t_vec){1.0, 0.0, 0.0};          // Use X-axis as up
        }
        tangent1 = normalize(cross(intersection.normal, v_up));
    // If the above cross product results in a zero vector (e.g. normal IS {0,1,0} and we cross with {0,1,0}),
    // it means the chosen 'up' was parallel. Pick another.
    // A simple check for zero vector:
    if (norm2(tangent1) < epsilon*epsilon) { // Using epsilon from the file
        // This case happens if intersection.normal was parallel to the chosen 'v_up' vector.
        // This should ideally not happen with the logic above but as a fallback:
        // If normal was (1,0,0), v_up is (0,1,0), tangent1 is (0,0,1)
        // If normal was (0,1,0), v_up is (1,0,0), tangent1 is (0,0,-1)
        // If normal was (0,0,1), v_up is (0,1,0), tangent1 is (1,0,0)
        // The only remaining problematic case is if normalize(cross(intersection.normal, v_up)) leads to a zero vector,
        // which means intersection.normal and v_up are parallel.
        // The current logic for v_up selection should prevent this for non-zero normals.
        // However, if intersection.normal is (0,0,0) (though it should be normalized),
        // cross product would be (0,0,0).
        // As a very robust fallback, if tangent1 is zero, try a different axis for v_up.
        // This situation implies intersection.normal was parallel to the initial v_up.
        if (fabs(intersection.normal.x) > 0.9999) { // Normal is mostly along X-axis
            v_up = (t_vec){0.0, 1.0, 0.0}; // use Y
        } else { // Normal is not predominantly along X-axis
            v_up = (t_vec){1.0, 0.0, 0.0}; // use X
        }
        tangent1 = normalize(cross(intersection.normal, v_up));
        // If it's still zero, then normal must have been zero or v_up was still parallel.
        // This would indicate an issue with normal itself or extremely specific alignment.
        // For a final attempt if tangent1 is still zero (e.g. normal is (0,0,0) or normal is Y and v_up became Y):
        if (norm2(tangent1) < epsilon*epsilon) {
             v_up = (t_vec){0.0, 0.0, 1.0}; // Use Z-axis
             tangent1 = normalize(cross(intersection.normal, v_up));
        }
    }
        tangent2 = cross(intersection.normal, tangent1); // intersection.normal and tangent1 are now orthogonal and unit length.
                                                     // So tangent2 will also be unit length.
        t_vec direction_random = vec_plus(vec_plus(vec_mult(direction_random_local_basis.x,tangent1),vec_mult(direction_random_local_basis.y,tangent2)\
                                                           ),vec_mult(direction_random_local_basis.z,intersection.normal));
        t_ray random_ray = (t_ray){vec_plus(intersection.point,vec_mult(0.001,intersection.normal)),direction_random};
        pixel = vec_plus(pixel, vec_m_vec(get_color(random_ray,rt,nb_rebound-1),rt->scene.objects[obj_id].albedo));
    }
    return pixel;
}

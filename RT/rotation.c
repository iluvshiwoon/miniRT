#include "../miniRT.h"

t_vec get_camera_right(t_camera cam) {
    t_vec world_up = {0, 1, 0};
    
    // Handle edge case when looking straight up/down
    if (fabs(cam.direction.y) > 0.99) {
        world_up = (t_vec){0, 0, 1};
    }
    
    return normalize(cross(cam.direction, world_up));
}

t_vec get_camera_up(t_camera cam) {
    t_vec right = get_camera_right(cam);
    return normalize(cross(right, cam.direction));
}

// Create rotation matrix around arbitrary axis
t_mat3 create_rotation_axis(t_vec axis, double angle) {
    t_mat3 result;
    axis = normalize(axis);
    double c = cos(angle);
    double s = sin(angle);
    double one_minus_c = 1.0 - c;
    
    result.m[0][0] = c + axis.x * axis.x * one_minus_c;
    result.m[0][1] = axis.x * axis.y * one_minus_c - axis.z * s;
    result.m[0][2] = axis.x * axis.z * one_minus_c + axis.y * s;
    
    result.m[1][0] = axis.y * axis.x * one_minus_c + axis.z * s;
    result.m[1][1] = c + axis.y * axis.y * one_minus_c;
    result.m[1][2] = axis.y * axis.z * one_minus_c - axis.x * s;
    
    result.m[2][0] = axis.z * axis.x * one_minus_c - axis.y * s;
    result.m[2][1] = axis.z * axis.y * one_minus_c + axis.x * s;
    result.m[2][2] = c + axis.z * axis.z * one_minus_c;
    
    return result;
}

// Apply matrix to vector
t_vec mat3_multiply_vec(t_mat3 m, t_vec v) {
    return (t_vec){
        m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z,
        m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z,
        m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z
    };
}

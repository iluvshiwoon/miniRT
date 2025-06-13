#ifndef VECTOR_MATH_OPS_H
 #ifndef MATH_H
    #include <math.h> 
#endif
#define VECTOR_MATH_OPS_H

// t_vec struct and math.h (for sqrt, fabs) are assumed to be included
// before this file, likely via rt.h or miniRT.h.

static inline t_vec vec_plus(const t_vec a, const t_vec b) {
    return (t_vec){a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline t_vec vec_minus(const t_vec a, const t_vec b) {
    return (t_vec){a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline t_vec vec_mult(double k, const t_vec v) {
    return (t_vec){k * v.x, k * v.y, k * v.z};
}

static inline t_vec vec_div(const t_vec v, double k) {
    // Consider adding a check for k == 0 if not done elsewhere
    // For now, matching existing behavior.
    return (t_vec){v.x / k, v.y / k, v.z / k};
}

static inline double vec_scal(const t_vec a, const t_vec b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline double norm2(const t_vec v) {
    return (v.x * v.x + v.y * v.y + v.z * v.z);
}

static inline t_vec normalize(const t_vec v) {
    double n = sqrt(norm2(v));
    if (n == 0.0) { // Avoid division by zero
        return (t_vec){0.0, 0.0, 0.0}; // Return zero vector or handle error
    }
    return (t_vec){v.x / n, v.y / n, v.z / n};
}

static inline t_vec vec_m_vec(const t_vec a, const t_vec b) { // Element-wise multiplication
    return (t_vec){a.x * b.x, a.y * b.y, a.z * b.z};
}

static inline t_vec cross(const t_vec a, const t_vec b) {
    return (t_vec){a.y * b.z - b.y * a.z,
                   a.z * b.x - a.x * b.z,
                   a.x * b.y - a.y * b.x};
}

#endif // VECTOR_MATH_OPS_H

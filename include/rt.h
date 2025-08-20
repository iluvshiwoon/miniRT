/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:06:07 by kgriset           #+#    #+#             */
/*   Updated: 2025/08/18 11:45:08 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H
# define KEY_ESC 65307
# define KEY_C 99
# define KEY_N 110
# define KEY_P 112
# define KEY_S 115
# define KEY_W 119
# define KEY_A 97
# define KEY_D 100
# define KEY_Z 122
# define KEY_B 98
# define KEY_ENTER 65293
# define KEY_SHIFT 65505
# define KEY_CTRL 65507
# define KEY_PITCH_UP 65362   // Up arrow
# define KEY_PITCH_DOWN 65364 // Down arrow
# define KEY_YAW_LEFT 65361   // Left arrow
# define KEY_YAW_RIGHT 65363  // Right arrow
# define KEY_ROLL_LEFT 113    // Q
# define KEY_ROLL_RIGHT 101   // E
# define PCG_DEFAULT_MULTIPLIER_64 6364136223846793005ULL
# define EXPOSURE 300000000.0
# define GLOBAL_EXPOSURE 1000000.0
# define EPSILON 1e-6
# define FRACMASK 0x000FFFFFFFFFFFFFU
# define EXPMASK 0x7FF0000000000000U
# define HIDDENBIT 0x0010000000000000U
# define SIGNMASK 0x8000000000000000U
# define EXPBIAS 1075
# define NPOWERS 87
# define STEPPOWERS 8
# define FIRSTPOWER -348
# define EXPMAX -32
# define EXPMIN -60
# include "../42_MyLibC/mylibc.h"
# include <stdbool.h>
# include <stdint.h>
# include <pthread.h>
# include <stdatomic.h>


typedef struct s_fp
{
	uint64_t						frac;
	int								exp;
}									t_fp;

union								u_dbl_bits
{
	double							dbl;
	uint64_t						i;
};

typedef struct s__fp
{
	uint64_t						lomask;
	uint64_t						ah_bl;
	uint64_t						al_bh;
	uint64_t						al_bl;
	uint64_t						ah_bh;
	uint64_t						tmp;
	t_fp							fp;
}									t__fp;

typedef struct s_digits
{
	uint64_t						wfrac;
	uint64_t						delta;
	t_fp							one;
	uint64_t						part1;
	uint64_t						part2;
	int								idx;
	int								kappa;
	uint64_t						*divp;
	uint64_t						div;
	unsigned int					digit;
	uint64_t						tmp;
	uint64_t						*unit;
}									t_digits;

typedef struct s_grisu2
{
	t_fp							w;
	t_fp							lower;
	t_fp							upper;
}									t_grisu2;

typedef struct s_fpconv
{
	char							digits[18];
	int								str_len;
	bool							neg;
	int								spec;
	int								k;
	int								ndigits;
}									t_fpconv;

typedef struct s_emit_digits
{
	int								exp;
	int								max_trailing_zeros;
	int								offset;
	int								idx;
	char							sign;
	int								cent;
	int								dec;
}									t_emit_digits;

typedef struct s_mat3
{
	double							m[3][3];
}									t_mat3;

typedef struct s_pass_config
{
	int								bounces;
	int								skip;
	int								update_freq;
}									t_pass_config;

typedef struct s_ray				t_ray;
typedef struct s_render_state
{
	atomic_bool							re_render_scene;
	atomic_bool							render_paused;
	int								pass;
	int								*shuffled_pixels;
	t_ray							*rays;
	bool							display_string;
	bool							color_black;
	int								display_id;
}									t_render_state;

typedef struct s_data
{
	void							*img;
	char							*addr;
	int								bits_per_pixel;
	int								line_length;
	int								endian;
	int								width;
	int								height;
}									t_data;

typedef struct s_vec
{
	double							x;
	double							y;
	double							z;
}									t_vec;

typedef struct s_ray
{
	t_vec							origin;
	t_vec							direction;
}									t_ray;

typedef struct s_ambient_light
{
	double							intensity;
	t_vec							color;
}									t_ambient_light;

typedef struct s_light
{
	t_vec							origin;
	double							intensity;
	t_vec							color;
}									t_light;

typedef struct s_camera
{
	t_vec							origin;
	t_vec							direction;
	t_vec							up;
	double							fov;
	double							yaw;
	double							pitch;
	double							roll;
}									t_camera;

typedef struct s_sphere
{
	t_vec							origin;
	double							radius;
}									t_sphere;

typedef struct s_plane
{
	t_vec							origin;
	t_vec							normal;
}									t_plane;

typedef struct s_cylinder
{
	t_vec							origin;
	t_vec							dir;
	double							radius;
	double							height;
}									t_cylinder;

typedef struct s_cone
{
	t_vec							origin;
	t_vec							dir;
	double							radius;
	double							height;
}									t_cone;

typedef struct s_intersection
{
	t_vec							point;
	t_vec							normal;
	double							t;
	bool							hit_cap;
}									t_intersection;

typedef struct s_rt					t_rt;

enum								e_type
{
	A = 42,
	L,
	C,
	sp,
	pl,
	cy,
	co
};

typedef struct s_rvec
{
	double							pitch;
	double							yaw;
	double							roll;
}									t_rvec;

typedef struct s_object				t_object;

struct								s_object
{
	int								id;
	enum e_type						type;
	void							*obj;
	t_vec							albedo;
	t_vec							specular;
	double							shininess;
	bool							checkerboard;
	char							*normal_map_path;
	t_data							normal_map;
	char							*texture_map_path;
	t_data							texture_map;
	t_vec							texture_scale;
	char							*string;
	void							(*rotate)(t_rt *rt, int id, t_rvec rvec);
	void							(*translate)(t_rt *rt, int id, t_vec vec);
	char							*(*display_string)(t_rt *, \
									const struct s_object);
	int								(*is_intersection)(const t_ray, \
									const struct s_object, t_intersection *);
};

typedef struct s_scene
{
	int								total_objects;
	t_object						*objects;
	t_camera						camera;
	t_light							**lights;
	int								num_lights;
	t_ambient_light					ambient_light;
}									t_scene;

typedef struct s_mt_state
{
	uint32_t						state_array[624];
	int								state_index;
	double							max_range;
}									t_mt_state;

typedef struct s_pcg_state_setseq_64
{
	uint64_t						state;
	uint64_t						inc;
} t_pcg32_random;

typedef struct s_cylinder_inter
{
	t_cylinder						cylinder;
	double							body_t;
	bool							body_has_sol;
	double							cap_t;
	t_vec							cap_normal;
	bool							cap_has_sol;
	bool							has_sol;
	double							final_t;
	bool							hit_cap;
	t_vec							to_point;
	t_vec							axis_projection;
	t_vec							to_ray_origin;
}									t_cylinder_inter;

typedef struct s_get_color
{
	t_intersection					intersection;
	t_vec							pixel;
	int								obj_id;
	t_ray							ray_light;
	t_intersection					intersection_light;
	int								obj_id_light;
	bool							inter;
	double							d_light2;
	double							cos_theta;
	t_vec							light_contribution;
	t_vec							ambient_contribution;
	t_vec							ambient_light;
	t_vec							light_direction;
	t_vec							half_vector;
	double							cos_alpha;
	t_vec							specular_contribution;
	double							r1;
	double							r2;
	t_vec							direction_random_local_basis;
	t_vec							random_vec;
	t_vec							tangent1;
	t_vec							tangent2;
	t_vec							direction_random;
	t_ray							random_ray;
}									t_get_color;

typedef struct s_cylinder_inter_solve
{
	t_vec							oc;
	t_vec							ray_dir_proj;
	t_vec							oc_proj;
	double							a;
	double							half_b;
	double							c;
	double							discriminant;
	double							sqrt_discriminant;
	double							t1;
	double							t2;
	double							candidate_t;
	t_vec							hit_point;
	t_vec							to_hit;
	double							height_proj;
}									t_cylinder_inter_solve;

typedef struct s_cylinder_cap
{
	t_plane							cap_planes[2];
	double							min_t;
	double							current_t;
	t_vec							p;
	int								i;
}									t_cylinder_cap;

typedef struct s_cone_inter
{
	t_cone							cone;
	double							body_t;
	bool							body_has_sol;
	double							cap_t;
	t_vec							cap_normal;
	bool							cap_has_sol;
	bool							has_sol;
	double							final_t;
	bool							hit_cap;
	t_vec							to_point;
	t_vec							axis_projection;
	t_vec							to_ray_origin;
}									t_cone_inter;

typedef struct s_cone_inter_solve
{
	t_vec							oc;
	t_vec							ray_dir_proj;
	t_vec							oc_proj;
	double							a;
	double							half_b;
	double							c;
	double							discriminant;
	double							sqrt_discriminant;
	double							t1;
	double							t2;
	double							candidate_t;
	t_vec							hit_point;
	t_vec							to_hit;
	double							height_proj;
}									t_cone_inter_solve;

typedef struct s_cone_cap
{
	t_plane							cap_plane;
	double							min_t;
	double							current_t;
	t_vec							p;
	int								i;
}									t_cone_cap;

typedef struct s_gen_rays
{
	t_camera						cam;
	t_vec							cam_x;
	t_vec							cam_y;
	t_vec							cam_z;
	int								pixel_index;
	int								x;
	int								y;
	double							u;
	double							v;
	double							focal_length;
	t_vec							direction;
}									t_gen_rays;

typedef struct s_render
{
	int								nrays;
	int								k;
	int								index;
	int								x;
	int								y;
	t_ray							ray;
	t_vec							pixel_intensity;
}									t_render;

typedef struct s_chunk{
    int start_pixel;
    int end_pixel;
} t_chunk;

typedef struct s_worker t_worker;

typedef struct s_shared{
    t_rt *rt;
    pthread_mutex_t display_mutex;
    pthread_mutex_t work_mutex;
    pthread_cond_t work_available;
    pthread_cond_t to_display;
    atomic_bool should_exit;
    atomic_bool work_paused;
    atomic_int  paused_threads;
    atomic_bool work_ready;
    atomic_int  current_pass;
    int num_threads;
    t_chunk * chunks;
} t_shared; 

typedef struct s_thread_data {
	t_pcg32_random	rng;
    int start_pixel;
    int end_pixel;
    int thread_id;
} t_thread_data;

typedef struct s_worker {
    t_shared * shared;
    pthread_t thread;
    t_thread_data data;
} t_worker;

typedef struct s_rt
{
	void							*mlx;
	void							*win;
	t_data							image;
	t_render_state					state;
	struct s_pcg_state_setseq_64	rng;
	int								w;
	int								h;
	int								total_pixels;
	int								fd_file;
	double							fov;
	t_link_list						*graphic_heap;
	t_link_list						*parsing_heap;
	t_link_list						*current_heap;
	t_scene							scene;
	t_object						selected;
	t_pass_config					*config;
    t_shared                        *shared;
    t_worker                        *workers;
}									t_rt;

typedef struct s_rot_plane
{
	t_plane							*pl;
	t_vec							norm;
	t_vec							temp;
	t_vec							right;
	t_vec							forward;
	t_mat3							pitch_r;
	t_mat3							yaw_r;
}									t_rot_plane;

typedef struct s_cl
{
	t_cylinder						*cyl;
	t_vec							axis;
	t_vec							temp;
	t_vec							right;
	t_vec							forward;
	t_mat3							pitch_rot;
	t_mat3							yaw_rot;
}									t_cl;

typedef struct s_checker_idx
{
    int a;
    int b;
}   t_checker_idx;

typedef struct s_chk3
{
    int checker_x;
    int checker_y;
    int checker_z;
    double scale;
}   t_chk3;

typedef struct s_sph_chk
{
    double u;
    double v;
    int checker_u;
    int checker_v;
    double scale;
}   t_sph_chk;

typedef struct s_cyl_idx_args
{
    t_cylinder *cylinder;
    t_vec to_point;
    t_vec axis;
    t_vec right;
    t_vec up;
}   t_cyl_idx_args;

typedef struct s_cone_idx_args
{
    t_cone *cone;
    t_vec to_point;
    t_vec axis;
    t_vec right;
    t_vec up;
}   t_cone_idx_args;

typedef struct s_cone_idx {
	double			x;
	double			y;
	double			z;
	double			theta;
	double			height_ratio;
}t_cone_idx;

typedef struct s_checkboard{
	t_vec			axis;
	t_vec			right;
	t_vec			up;
	t_vec			to_point;
}t_checkboard;

typedef struct s_calc_cone {
	t_vec	to_point;
	t_vec	axis_projection;
	t_vec	radial;
	double	height_proj;
	double	tan_angle;
	double	radius_at_height;
	t_vec	temp;
	t_vec	normal;
}t_calc_cone;

typedef struct s_cone_solve
{
	double	cos_angle;
	double	cos_angle_sq;
	double	tan_angle;

} t_cone_solve;

typedef struct s_light_comp
{
	t_vec	direct_light;
	t_vec	ambient_light;
	t_vec	specular_light;
	t_vec	view_direction;
}			t_light_comp;

typedef struct s_normal
{
	int		x;
	int		y;
	char	*dst;
	t_vec	map_normal;
	t_vec	tangent;
	t_vec	bitangent;
}t_normal;

typedef struct s_cap_uv{
	t_cone	*co;
	t_vec	base_center;
	t_vec	u_axis;
	t_vec	v_axis;
	t_vec	d;
}t_cap_uv;

typedef struct s_cone_uv
{
	t_cone	*co;
	t_vec	d;
	t_vec	u_axis;
	t_vec	v_axis;
	t_vec	axis_component;
	t_vec	radial;
	double	h;
	double	theta;
} t_cone_uv;

typedef struct s_rot_cone
{
	t_cone	*cone;
	t_vec	axis;
	t_vec	temp;
	t_vec	right;
	t_vec	forward;
	t_mat3	pitch_rot;
	t_mat3	yaw_rot;
} t_rot_cone;

typedef struct s_rc_local {
	t_cone	*cone;
	t_vec	axis;
	t_vec	temp;
	t_vec	right;
	t_vec	forward;
	t_mat3	pitch_rot;
	t_mat3	yaw_rot;
} t_rc_local;

int									close_win(t_rt *rt);
int									create_trgb(int t, int r, int g, int b);
void								my_mlx_put_pixel(t_data *data, int x, int y,
										int color);
t_vec								vec_plus(const t_vec a, const t_vec b);
t_vec								vec_minus(const t_vec a, const t_vec b);
t_vec								vec_mult(double k, const t_vec v);
t_vec								vec_div(const t_vec v, double k);
double								vec_scal(const t_vec a, const t_vec b);
double								norm2(const t_vec v);
t_vec								normalize(const t_vec v);
t_vec								vec_m_vec(const t_vec a, const t_vec b);
t_vec								cross(const t_vec a, const t_vec b);
void								*wrap_malloc(t_rt *rt, size_t size);
void								free_heap(t_rt *rt);
t_link_list							*init_alloc(t_link_list **list);
bool								plane_intersection_solve(const t_ray ray,
										const t_plane plane, double *t);
int									is_intersection_sphere(const t_ray ray, \
										const t_object obj,
										t_intersection *intersection);
int									is_intersection_plane(const t_ray ray, \
										const t_object obj,
										t_intersection *intersection);
int									is_intersection_cylinder(const t_ray ray, \
										const t_object obj,
										t_intersection *intersection);
void								save_img(t_rt *rt, \
										const unsigned char *pixels, int W,
										int H);
void								exit_error(t_rt *rt, char *msg);

char								*rt_ft_strdup(t_rt *rt, const char *s);
char								*rt_ft_strtrim(t_rt *rt, char const *s1,
										char const *set);
char								*rt_ft_substr(t_rt *rt, char const *s,
										unsigned int start, size_t len);
char								**rt_ft_split(t_rt *rt, char const *s,
										char c);
char								*rt_ft_strjoin(t_rt *rt, char const *s1,
										char const *s2);
char								*rt_ft_itoa(t_rt *rt, int n);
void								initialize_state(t_mt_state *state,
										uint32_t seed);
uint32_t							random_uint32(t_mt_state *state);
double								uniform_uint32(t_mt_state *state);
uint32_t							pcg_rotr_32(uint32_t value,
										unsigned int rot);
uint32_t							pcg_output_xsh_rr_64_32(uint64_t state);
uint32_t							pcg_setseq_64_xsh_rr_32_random_r(\
		struct s_pcg_state_setseq_64 *rng);
uint64_t							pcg_advance_lcg_64(uint64_t state,
										uint64_t delta, uint64_t cur_mult,
										uint64_t cur_plus);
void								pcg_setseq_64_advance_r(\
		struct s_pcg_state_setseq_64 *rng,
										uint64_t delta);
uint32_t							pcg_setseq_64_xsh_rr_32_boundedrand_r(\
		struct s_pcg_state_setseq_64 *rng,
										uint32_t bound);
double								double_rng(t_pcg32_random *rng);
bool								entropy_getbytes(void *dest, size_t size);
void								pcg_setseq_64_step_r(\
		struct s_pcg_state_setseq_64 *rng);
void								pcg_setseq_64_srandom_r(\
		struct s_pcg_state_setseq_64 *rng,
										uint64_t initstate, uint64_t initseq);
int									fpconv_dtoa(double fp, char dest[24]);
char								*vec_toa(t_rt *rt, t_vec vec);
char								*color_toa(t_rt *rt, t_vec vec);
t_mat3								create_rotation_axis(t_vec axis,
										double angle);
t_vec								mat3_multiply_vec(t_mat3 m, t_vec v);
t_vec								get_camera_right(t_camera cam);
t_vec								get_camera_up(t_camera cam);
void								gen_shuffled_pixels(t_rt *rt, int *array);

void								gen_rays(t_rt *rt);
void								init_render(t_rt *rt);
void								display_string(t_rt *rt, int id);
int									render(t_rt *rt);
bool								render_pixels(t_rt *rt, t_render *r);
t_vec								camera_to_world_movement(t_camera cam,
										t_vec local_movement);
t_vec								calculate_direct_lighting(t_rt *rt,
										t_get_color *gc);
t_vec								calculate_ambient_lighting(t_rt *rt,
										t_get_color *gc);
t_vec								calculate_specular_reflection(t_rt *rt,
										t_get_color *gc, t_vec view_direction);
t_vec								get_checkerboard_color(t_vec point, t_vec albedo);
t_vec								get_plane_checkerboard(t_vec point, t_vec albedo, t_plane *plane);
t_vec								get_sphere_checkerboard(t_vec point, t_vec albedo, t_sphere *sphere);
t_vec								get_cylinder_checkerboard(t_vec point, t_vec albedo, t_cylinder *cylinder);
t_vec								get_cylinder_cap_checkerboard(t_vec point, t_vec albedo, t_cylinder *cylinder);
t_vec								get_cone_checkerboard(t_vec point, t_vec albedo, t_cone *cone);
t_vec								get_cone_cap_checkerboard(t_vec point, t_vec albedo, t_cone *cone);
t_vec								get_material_color(t_object *obj, t_vec point, t_intersection *intersection);
t_vec								generate_random_hemisphere_direction(\
										t_vec normal, t_pcg32_random *rng);
t_vec								calculate_recursive_reflection(t_rt *rt,
										t_get_color *gc, int nb_rebound, t_pcg32_random *rng);
bool								visible_intersection(const t_ray ray,
										t_scene scene,
										t_intersection *intersection,
										int *obj_id);
void								determine_closest_intersection(\
		t_cylinder_inter *cy);
t_vec								calculate_body_normal(\
		const t_cylinder_inter *cy, const t_vec intersection_point);
t_vec								calculate_intersection_normal(\
		const t_cylinder_inter *cy, \
		const t_vec intersection_point, const t_ray ray);
void								fill_intersection_data(\
		t_intersection *intersection, \
		const t_cylinder_inter *cy, const t_ray ray);
bool								cylinder_intersection_solve1(\
		t_cylinder_inter_solve *cy, \
		const t_ray ray, const t_cylinder cylinder);
bool								cylinder_intersection_solve(\
		const t_ray ray, const t_cylinder cylinder, double *t);
void								cylinder_cap_calc(t_cylinder_cap *cy,
										const t_ray ray,
										const t_cylinder cylinder,
										t_vec *normal);
bool								cylinder_cap_intersection(const t_ray ray,
										const t_cylinder cylinder, double *t,
										t_vec *normal);
void								init_cylinder_intersection(\
		t_cylinder_inter *cy, const t_object obj);

// Cone intersection functions
int									is_intersection_cone(const t_ray ray, \
										const t_object obj,
										t_intersection *intersection);
void								determine_closest_cone_intersection(\
		t_cone_inter *co);
t_vec								calculate_cone_body_normal(\
		const t_cone_inter *co, const t_vec intersection_point);
t_vec								calculate_cone_intersection_normal(\
		const t_cone_inter *co, \
		const t_vec intersection_point, const t_ray ray);
void								fill_cone_intersection_data(\
		t_intersection *intersection, \
		const t_cone_inter *co, const t_ray ray);
bool								cone_intersection_solve1(\
		t_cone_inter_solve *co, \
		const t_ray ray, const t_cone cone);
bool								cone_intersection_solve(\
					const t_ray ray, const t_cone cone, double *t);
typedef struct s_co_cap_calc_args
{
	t_cone_cap							*co;
	t_ray								ray;
	t_cone								cone;
	t_vec								*normal;
} t_co_cap_calc_args;
void								cone_cap_calc_s(t_co_cap_calc_args args);
typedef struct s_co_cap_args
{
	t_ray								ray;
	t_cone								cone;
	double								*t;
	t_vec								*normal;
} t_co_cap_args;
bool								cone_cap_intersection_s(t_co_cap_args args);
void								init_cone_intersection(\
		t_cone_inter *co, const t_object obj);
int									fpconv_dtoa(double fp, char dest[24]);
t_fp								find_cachedpow10(int exp, int *k);
uint64_t							get_dbits(double d);
int									absv(int n);
int									min(int a, int b);
uint64_t							get_dbits(double d);
t_fp								build_fp(double d);
void								_normalize(t_fp *fp);
void								get_normalized_boundaries(t_fp *fp,
										t_fp *lower, t_fp *upper);
t_fp								multiply(t_fp *a, t_fp *b);
void								round_digit(char *digits, t_digits d);
void								init_digits_struct(uint64_t *g_tens,
										t_digits *d, t_grisu2 *g);
int									process_integer_part(t_digits *d,
										char *digits, int *K);
int									emit_integer_format(t_fpconv f, char *dest);
int									emit_decimal_leading_zeros(t_fpconv f,
										char *dest, int offset);
int									emit_decimal_with_point(t_fpconv f,
										char *dest, int offset);
int									emit_scientific_mantissa(t_fpconv f,
										char *dest, int *idx);
int									emit_exponent_digits(int exp, char *dest,
										int idx);
int									process_fractional_part(uint64_t *g_tens,
										t_digits *d, char *digits, int *K);
int									generate_digits(uint64_t *g_tens,
										t_grisu2 *g, char *digits, int *K);
int									grisu2(double d, char *digits, int *K);
int									emit_scientific_exponent(t_fpconv f,
										char *dest, int idx);
int									emit_scientific_format(t_fpconv f,
										char *dest);
t_vec								handle_movement_keys(int keycode);
void								apply_movement(t_rt *rt, int id,
										enum e_type type, t_vec local_movement);
void								handle_rotation_keys(t_rt *rt, int id,
										 int keycode);
bool								handle_toggle_keys(t_rt *rt, int keycode);
bool								handle_navigation_keys(t_rt *rt,
										int keycode);
t_vec								camera_to_world_movement(t_camera cam,
										t_vec local_movement);
void								rotate_camera_local(t_rt *rt, int id,
										t_rvec rvec);
void								translate_camera(t_rt *rt, int id,
										t_vec vec);
char								*string_camera(t_rt *rt,
										const struct s_object object);
void								parse_camera(t_rt *rt, char *line, int *id);
void								apply_pitch_rotation(t_vec *forward,
										t_vec *up, t_vec right, double pitch);
void								apply_yaw_rotation(t_vec *forward,
										t_vec *right, t_vec up, double yaw);
void								apply_roll_rotation(t_vec *up, t_vec *right,
										t_vec forward, double roll);
void								finalize_camera_rotation(t_rt *rt, int id,
										t_vec forward, t_vec up);
void								setup_camera_orientation(t_camera *camera);
void								setup_camera_object(t_rt *rt,
										t_camera *camera, int *id);
void    init_multi_threading(t_rt * rt);
t_vec	get_color(t_ray ray, t_rt *rt, int nb_rebound, t_pcg32_random *rng);
int	key_events(int keycode, t_rt *rt);
void	get_uv(t_object *obj, t_intersection *intersection, double *u, double *v);
t_vec	get_normal_from_map(t_object *obj, double u, double v, t_vec normal);
t_vec	get_texture_color(t_object *obj, double u, double v);
void	write_to_file(t_rt *rt);
void    get_cone_uv(t_object *obj, t_intersection *intersection, double *u, double *v);
void    fill_chunk_index(t_rt * rt, t_chunk * chunks, int num_threads);
void * worker_thread_loop(void * arg);
t_vec	cylinder_checker_color_from_idx(t_checker_idx idx, t_vec albedo);
void	cone_local_basis(t_cone *cone, t_vec *axis, t_vec *right, t_vec *up);
t_checker_idx	cone_checker_indices(t_cone_idx_args a);
void	cone_compute_params(const t_ray ray, const t_cone cone,
		t_cone_solve * c);
void	cone_prepare_projections(t_cone_inter_solve *co, const t_ray ray,
		const t_cone cone);
void	cone_compute_quadratic(t_cone_inter_solve *co, t_cone_solve c, const t_cone cone);
void	cone_check_first_root(t_cone_inter_solve *co, const t_ray ray,
		const t_cone cone);
bool	cone_cap_intersection_s(t_co_cap_args args);
void	init_cone_intersection(t_cone_inter *co, const t_object obj);
void	cyl_prepare_oc_proj(t_cylinder_inter_solve *cy, const t_ray ray,
		const t_cylinder cylinder);
void	cyl_prepare_ray_proj(t_cylinder_inter_solve *cy, const t_ray ray,
		const t_cylinder cylinder);
void	cyl_compute_quadratic(t_cylinder_inter_solve *cy,
		const t_cylinder cylinder);
void	cyl_check_first_root(t_cylinder_inter_solve *cy, const t_ray ray,
		const t_cylinder cylinder);
void	update_best_intersection(t_intersection *intersection,
		t_intersection *local_intersection, int *obj_id, int current_index);
void	put_pixel_gamma_corrected(t_rt *rt, int x, int y, t_vec intensity);
void	accumulate_pixel_intensity(t_rt *rt, t_worker *worker, t_render *r);
void	fill_chunk_index(t_rt *rt, t_chunk *chunks, int num_threads);
void	process_one_pixel(t_rt *rt, t_worker *worker, t_render *r);
void	render_chunk(t_rt *rt, t_worker *worker, int pass);
void	get_cylinder_cap_uv(t_object *obj, t_vec p, double *u, double *v);
void	get_cylinder_uv(t_object *obj, t_intersection *intersection, double *u,
		double *v);
void	get_sphere_uv(t_object *obj, t_vec p, double *u, double *v);
void	get_plane_uv(t_object *obj, t_vec p, double *u, double *v);
void	get_cylinder_body_uv(t_object *obj, t_vec p, double *u, double *v);
void	wait_for_work_available(t_shared *shared);
void	handle_pause_state(t_shared *shared, int *pass);
void	get_cylinder_cap_uv(t_object *obj, t_vec p, double *u, double *v);
void	get_cylinder_uv(t_object *obj, t_intersection *intersection, double *u,
		double *v);
char	*append_cone_dims(t_rt *rt, char *r_value, t_cone *cone);
char	*append_optional_maps_co(t_rt *rt, char *r_value,
		const struct s_object object);
void	parse_cone_optional1(t_rt *rt, char **tab, int *id);
void	parse_cone_optional(t_rt *rt, char **tab, int *id);
void	rotate_cone_local(t_rt *rt, int id, t_rvec rvec);
char	*append_cylinder_dims(t_rt *rt, char *r_value, t_cylinder *cylinder);
char	*append_optional_maps_cy(t_rt *rt, char *r_value,
		const struct s_object object);
void	parse_cylinder_optional1(t_rt *rt, char **tab, int *id);
void	parse_cylinder_optional(t_rt *rt, char **tab, int *id);
void	rotate_cylinder_local(t_rt *rt, int id, t_rvec rvec);
char	*append_optional_maps_pl(t_rt *rt, char *r_value, \
    const struct s_object object);
void	rotate_plane_local(t_rt *rt, int id, t_rvec rvec);
void	translate_plane(t_rt *rt, int id, t_vec vec);
char	*string_plane(t_rt *rt, const struct s_object object);
void    fill_plane(t_rt *rt, char **tab, t_plane *plane, int *id);
char	*append_optional_maps_sp(t_rt *rt, char *r_value, \
    const struct s_object object);
void	translate_sphere(t_rt *rt, int id, t_vec vec);
char	*string_sphere(t_rt *rt, const struct s_object object);
void	fill_sphere(t_rt *rt, t_sphere *sphere, int id);
void    parse_sphere_optional1(t_rt *rt, char **tab, int *id);
#endif

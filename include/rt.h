/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:25:41 by kgriset           #+#    #+#             */
/*   Updated: 2025/07/25 12:38:46 by kgriset          ###   ########.fr       */
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
# define KEY_ENTER 65293
# define KEY_SHIFT 65505
# define KEY_CTRL 65507
#define KEY_PITCH_UP    65362  // Up arrow
#define KEY_PITCH_DOWN  65364  // Down arrow
#define KEY_YAW_LEFT    65361  // Left arrow
#define KEY_YAW_RIGHT   65363  // Right arrow
#define KEY_ROLL_LEFT   113    // Q
#define KEY_ROLL_RIGHT  101    // E
# define PCG_DEFAULT_MULTIPLIER_64  6364136223846793005ULL
# define pcg32_random_r                  pcg_setseq_64_xsh_rr_32_random_r
# define pcg32_srandom_r                 pcg_setseq_64_srandom_r
# define pcg32_advance_r                 pcg_setseq_64_advance_r
#define pcg32_boundedrand_r             pcg_setseq_64_xsh_rr_32_boundedrand_r
#define EXPOSURE 300000000.0
#define GLOBAL_EXPOSURE 1000000.0
#define EPSILON 1e-6
#define FRACMASK 0x000FFFFFFFFFFFFFU
#define EXPMASK 0x7FF0000000000000U
#define HIDDENBIT 0x0010000000000000U
#define SIGNMASK 0x8000000000000000U
#define EXPBIAS (1023 + 52)

#define npowers     87
#define steppowers  8
#define firstpower -348 /* 10 ^ -348 */

#define expmax     -32
#define expmin     -60


# include <stdint.h>
# include <stdbool.h>
# include "../42_MyLibC/mylibc.h"

typedef struct Fp {
    uint64_t frac;
    int exp;
} Fp;

union					u_dbl_bits
{
	double				dbl;
	uint64_t			i;
};


typedef struct s_Fp
{
	uint64_t			lomask;
	uint64_t			ah_bl;
	uint64_t			al_bh;
	uint64_t			al_bl;
	uint64_t			ah_bh;
	uint64_t			tmp;
	Fp					fp;
}						t_Fp;

typedef struct s_digits
{
	uint64_t			wfrac;
	uint64_t			delta;
	Fp					one;
	uint64_t			part1;
	uint64_t			part2;
	int					idx;
	int					kappa;
	uint64_t			*divp;
	uint64_t			div;
	unsigned int		digit;
	uint64_t			tmp;
	uint64_t			*unit;
}						t_digits;


typedef struct s_grisu2
{
	Fp					w;
	Fp					lower;
	Fp					upper;
}						t_grisu2;

typedef struct s_fpconv
{
	char				digits[18];
	int					str_len;
	bool				neg;
	int					spec;
	int					k;
	int					ndigits;
}						t_fpconv;

typedef struct s_emit_digits
{
	int					exp;
	int					max_trailing_zeros;
	int					offset;
	int					idx;
	char				sign;
	int					cent;
	int					dec;
}						t_emit_digits;

typedef struct s_mat3{
	double m[3][3];
}t_mat3;

typedef struct s_pass_config {
    int bounces;
    int skip;        // Render every Nth pixel
    int update_freq; // Update display frequency
}t_pass_config;

typedef struct s_ray t_ray;
typedef struct s_render_state {
    bool re_render_scene;
    int pass;         
    int pixel_index;
    int * shuffled_pixels;
    t_ray * rays;
    bool display_string;
    bool color_black;
    int display_id;
} t_render_state;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_data;

typedef struct s_vec {
    double x;
    double y;
    double z;
} t_vec;

typedef struct s_ray {
    t_vec origin;
    t_vec direction;
} t_ray;

typedef struct s_ambient_light {
    double intensity;
    t_vec color;
} t_ambient_light;

typedef struct s_light {
    t_vec origin;
    double intensity;
    t_vec color;
} t_light;

typedef struct s_camera {
    t_vec origin;
    t_vec direction;
    t_vec up;
    double fov;
    double yaw;
    double pitch;
    double roll;
} t_camera;

typedef struct s_sphere {
    t_vec origin;
    double radius;
}t_sphere;

typedef struct s_plane {
    t_vec origin;
    t_vec normal;
} t_plane;

typedef struct s_cylinder {
    t_vec origin;
    t_vec direction;
    double radius;
    double height;
} t_cylinder;

typedef struct s_intersection {
    t_vec point;
    t_vec normal;
    double t;
} t_intersection;

typedef struct s_rt t_rt;

enum e_type {
	A = 42,
	L,
	C,
	sp,
	pl,
	cy
};

typedef struct s_rvec {
	double pitch;
	double yaw;
	double roll;
} t_rvec;

typedef struct s_object {
    int id;
    enum e_type type;
    void * obj;
    bool (*is_intersection)(const t_ray, const struct s_object, t_intersection *);
    char *(*display_string)(t_rt *, const struct s_object);
    void (*translate)(t_rt * rt, int id, t_vec vec);
    void (*rotate)(t_rt * rt, int id, t_rvec rvec);
    t_vec albedo; 
    char * string;
} t_object;

typedef struct s_scene {
    int total_objects;
    t_object * objects;
    t_camera camera;
    t_light light;
    t_ambient_light ambient_light;
} t_scene; 

typedef struct s_mt_state
{
    uint32_t state_array[624];        
    int state_index;                
    double max_range;
} t_mt_state;

typedef struct pcg_state_setseq_64      t_pcg32_random;

struct pcg_state_setseq_64 {
    uint64_t state;
    uint64_t inc;
};

typedef struct cylinder_inter
{
	t_cylinder	cylinder;
	double		body_t;
	bool		body_has_sol;
	double		cap_t;
	t_vec		cap_normal;
	bool		cap_has_sol;
	bool		has_sol;
	double		final_t;
	bool		hit_cap;
	t_vec		to_point;
	t_vec		axis_projection;
	t_vec		to_ray_origin;

}				t_cylinder_inter;

typedef struct s_get_color
{
    t_intersection intersection;
    t_vec pixel ;
    int obj_id ;
    t_ray ray_light ;
        t_intersection intersection_light;
        int obj_id_light ;
        bool inter ;
        double d_light2 ;
		double cos_theta ;
		t_vec light_contribution ;
	t_vec ambient_contribution ;
        t_vec ambient_light ;

        double r1 ;
        double r2 ;
        t_vec direction_random_local_basis ;
        t_vec random_vec ;
        t_vec tangent1 ;
        t_vec tangent2 ;
        t_vec direction_random ;
        t_ray random_ray ;
} t_get_color;

typedef struct s_cylinder_inter_solve{
	t_vec oc;
	t_vec ray_dir_proj;
	t_vec oc_proj;
	double a;
	double half_b;
	double c;
	double discriminant;
	double sqrt_discriminant;
	double t1;
	double t2;
	double candidate_t;
	t_vec hit_point;
	t_vec to_hit;
	double height_proj;
}t_cylinder_inter_solve;

typedef struct s_cylinder_cap{
	t_plane	cap_planes[2];
	double	min_t;
	double	current_t;
	t_vec	p;
	int i;
}t_cylinder_cap;

typedef struct s_gen_rays {
	t_camera cam;
	t_vec cam_x;
	t_vec cam_y;
	t_vec cam_z;
	int pixel_index;
	int x;
	int y;
	double u;
	double v;
	double focal_length;
	t_vec direction;
} t_gen_rays;

typedef	struct s_render {
	int		nrays;
	int		k;
	int		index;
	int		x;
	int		y;
	t_ray	ray;
	t_vec	pixel_intensity;
}t_render;

typedef struct s_rt {
    void *mlx;
    void *win;
    t_data image;
    t_render_state state;
    t_pcg32_random rng;
    int W;
    int H;
    int total_pixels;
    int fd_file;
    double fov;
    t_link_list * graphic_heap;
    t_link_list * parsing_heap;
    t_link_list * current_heap;
    t_scene scene;
	t_object selected;
	t_pass_config *config;
} t_rt;

// utils.c
int close_win(t_rt * rt);
// my_mlx_utils.c
int	create_trgb(int t, int r, int g, int b);
void	my_mlx_put_pixel(t_data *data, int x, int y, int color);

// vector.c 
t_vec vec_plus(const t_vec a, const t_vec b);
t_vec vec_minus(const t_vec a, const t_vec b);
t_vec vec_mult(double k, const t_vec v);
t_vec vec_div(const t_vec v, double k);
double vec_scal(const t_vec a, const t_vec b);

// vector1.c
double norm2(const t_vec v);
t_vec normalize(const t_vec v);
t_vec vec_m_vec(const t_vec a, const t_vec b);
t_vec cross(const t_vec a, const t_vec b);

// wrap_malloc.c
void	*wrap_malloc(t_rt *rt, size_t size);
void free_heap(t_rt *rt);
t_link_list *init_alloc(t_link_list **list);

// intersection.c
bool plane_intersection_solve(const t_ray ray, const t_plane plane, double * t);
t_vec   get_color(t_ray ray, t_rt * rt, int nb_rebound);
bool is_intersection_sphere(const t_ray ray, const t_object obj, t_intersection * intersection);
bool is_intersection_plane(const t_ray ray, const t_object obj, t_intersection * intersection);
bool is_intersection_cylinder(const t_ray ray, const t_object obj, t_intersection * intersection);
// miniRT.c
// bmp.c
void save_img(t_rt * rt, const unsigned char * pixels, int W, int H); // rgb pixel
// exit_error.c
void exit_error(t_rt * rt, char * msg);
// rt_strdup.c
char	*rt_ft_strdup(t_rt *rt, const char *s);
// rt_ft_strtrim.c
char	*rt_ft_strtrim(t_rt *rt, char const *s1, char const *set);
// rt_ft_substr.c
char	*rt_ft_substr(t_rt *rt, char const *s, unsigned int start, size_t len);
// rt_ft_split.c
char	**rt_ft_split(t_rt *rt, char const *s, char c);
// rt_ft_strjoin.c
char	*rt_ft_strjoin(t_rt *rt, char const *s1, char const *s2);
// rt_ft_itoa.c
char	*rt_ft_itoa(t_rt *rt, int n);

// rt_random.c
void initialize_state(t_mt_state* state, uint32_t seed);
uint32_t random_uint32(t_mt_state* state);
double uniform_uint32(t_mt_state* state);

// pcg.c
uint32_t pcg_rotr_32(uint32_t value, unsigned int rot);
uint32_t pcg_output_xsh_rr_64_32(uint64_t state);
uint32_t pcg_setseq_64_xsh_rr_32_random_r(struct pcg_state_setseq_64* rng);
uint64_t pcg_advance_lcg_64(uint64_t state, uint64_t delta, uint64_t cur_mult,
                            uint64_t cur_plus);
void pcg_setseq_64_advance_r(struct pcg_state_setseq_64* rng, uint64_t delta);

// pcg1.c
uint32_t pcg_setseq_64_xsh_rr_32_boundedrand_r(struct pcg_state_setseq_64* rng,
                                      uint32_t bound);
double double_rng(t_pcg32_random * rng);
bool entropy_getbytes(void* dest, size_t size);
void pcg_setseq_64_step_r(struct pcg_state_setseq_64* rng);
void pcg_setseq_64_srandom_r(struct pcg_state_setseq_64* rng, uint64_t initstate, uint64_t initseq);


// grisu.c
int fpconv_dtoa(double fp, char dest[24]);
// print_object.c
char * vec_toa(t_rt* rt, t_vec vec);
// rotation.c
t_mat3 create_rotation_axis(t_vec axis, double angle);
t_vec mat3_multiply_vec(t_mat3 m, t_vec v);
t_vec get_camera_right(t_camera cam);
t_vec get_camera_up(t_camera cam);
// render_utils.c
void gen_shuffled_pixels(t_rt * rt, int * array);
void	gen_rays(t_rt *rt);
void	init_render(t_rt *rt);
void	display_string(t_rt *rt, int id);
// render.c
int	render(t_rt *rt);
bool render_pixels(t_rt * rt, t_render * r);

// parisng/element/camera.c
t_vec	camera_to_world_movement(t_camera cam, t_vec local_movement);
// get_color_utils.c
t_vec	calculate_direct_lighting(t_rt *rt, t_get_color *gc);
t_vec	calculate_ambient_lighting(t_rt *rt, t_get_color *gc);
t_vec	generate_random_hemisphere_direction(t_rt *rt, t_vec normal);
t_vec	calculate_recursive_reflection(t_rt *rt, t_get_color *gc,
		int nb_rebound);
// get_color
t_vec	get_color(t_ray ray, t_rt *rt, int nb_rebound);
bool	visible_intersection(const t_ray ray, t_scene scene,
		t_intersection *intersection, int *obj_id);
// cylinder_intersection.c

void	determine_closest_intersection(t_cylinder_inter *cy);
t_vec	calculate_body_normal(const t_cylinder_inter *cy,
		const t_vec intersection_point);
t_vec	calculate_intersection_normal(const t_cylinder_inter *cy,
		const t_vec intersection_point, const t_ray ray);
void	fill_intersection_data(t_intersection *intersection,
		const t_cylinder_inter *cy, const t_ray ray);
bool	is_intersection_cylinder(const t_ray ray, const t_object obj,
		t_intersection *intersection);
// cylinder_intersection_utils.c
bool	cylinder_intersection_solve1(t_cylinder_inter_solve *cy,
		const t_ray ray, const t_cylinder cylinder);
bool	cylinder_intersection_solve(const t_ray ray, const t_cylinder cylinder,
		double *t);
void	cylinder_cap_calc(t_cylinder_cap *cy, const t_ray ray,
		const t_cylinder cylinder, t_vec *normal);
bool	cylinder_cap_intersection(const t_ray ray,
		const t_cylinder cylinder, double *t, t_vec *normal);
void	init_cylinder_intersection(t_cylinder_inter *cy, const t_object obj);
// grisu.c
int fpconv_dtoa(double fp, char dest[24]);
// grisu_utils.c
Fp find_cachedpow10(int exp, int* k);
 uint64_t	get_dbits(double d);
int	absv(int n);
int	min(int a, int b);
 uint64_t	get_dbits(double d);
 Fp	build_fp(double d);
 void	_normalize(Fp *fp);
 // grisu_utils2.c
void	get_normalized_boundaries(Fp *fp, Fp *lower, Fp *upper);
Fp	multiply(Fp *a, Fp *b);
void	round_digit(char *digits, t_digits d);
void	init_digits_struct(uint64_t *g_tens, t_digits *d, t_grisu2 *g);
int	process_integer_part(t_digits *d, char *digits, int *K);
// grisu_utils3.c
int	emit_integer_format(t_fpconv f, char *dest);
int	emit_decimal_leading_zeros(t_fpconv f, char *dest, int offset);
int	emit_decimal_with_point(t_fpconv f, char *dest, int offset);
int	emit_scientific_mantissa(t_fpconv f, char *dest, int *idx);
int	emit_exponent_digits(int exp, char *dest, int idx);
// grisu_utils5.c
int	process_fractional_part(uint64_t *g_tens, t_digits *d, \
		char *digits, int *K);
int	generate_digits(uint64_t *g_tens, t_grisu2 *g, char *digits, int *K);
int	grisu2(double d, char *digits, int *K);
int	emit_scientific_exponent(t_fpconv f, char *dest, int idx);
int	emit_scientific_format(t_fpconv f, char *dest);
// keys.c
t_vec handle_movement_keys(int keycode);
void apply_movement(t_rt *rt, int id, enum e_type type, t_vec local_movement);
void handle_rotation_keys(t_rt *rt, int id, enum e_type type, int keycode);
void handle_toggle_keys(t_rt *rt, int keycode);
void handle_navigation_keys(t_rt *rt, int keycode);
// Camera main functions
t_vec	camera_to_world_movement(t_camera cam, t_vec local_movement);
void	rotate_camera_local(t_rt *rt, int id, t_rvec rvec);
void	translate_camera(t_rt *rt, int id, t_vec vec);
char	*string_camera(t_rt *rt, const struct s_object object);
void	parse_camera(t_rt *rt, char *line, int *id);

// Camera utility functions (for camera_utils.c)
void	apply_pitch_rotation(t_vec *forward, t_vec *up, t_vec right, double pitch);
void	apply_yaw_rotation(t_vec *forward, t_vec *right, t_vec up, double yaw);
void	apply_roll_rotation(t_vec *up, t_vec *right, t_vec forward, double roll);
void	finalize_camera_rotation(t_rt *rt, int id, t_vec forward, t_vec up);

// Camera parse utility functions (for camera_parse.c)
void	setup_camera_orientation(t_camera *camera);
void	setup_camera_object(t_rt *rt, t_camera *camera, int *id);
#endif

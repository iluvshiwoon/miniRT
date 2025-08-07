C_FILES = RT/miniRT.c RT/vector1.c RT/vector.c RT/wrap_malloc.c \
		  RT/exit_error.c RT/rt_ft_strjoin.c RT/rt_ft_itoa.c RT/rt_ft_split.c RT/rt_ft_strtrim.c RT/rt_ft_substr.c \
		  RT/rt_ft_strdup.c RT/pcg32.c RT/pcg32_1.c RT/intersections.c RT/my_mlx_utils.c RT/utils.c RT/grisu.c RT/grisu_utils.c RT/grisu_utils2.c RT/grisu_utils3.c RT/grisu_utils4.c \
		  RT/print_object.c RT/rotation.c RT/render_utils.c RT/render.c RT/get_color.c RT/get_color_utils.c\
		  RT/cylinder_intersection.c RT/cylinder_intersection_utils.c RT/keys.c\
			parsing/parsing.c \
			parsing/check_args.c \
			parsing/open_file.c \
			parsing/alloc_rt.c \
			parsing/scene_utils.c\
			parsing/element/Ambient_light.c \
			parsing/element/camera.c parsing/element/camera_utils.c parsing/element/camera_parse.c\
			parsing/element/light.c \
			parsing/element/plane.c \
			parsing/element/sphere.c \
			parsing/element/cylinder.c \
			parsing/element/element_utils.c \
			parsing/element/atoi_double.c

O_FILES = $(C_FILES:%.c=build/%.o)
C_FLAGS = -Wall -Wextra -g -fsanitize=thread
# C_FLAGS = -Wall -Wextra
NAME = miniRT

.PHONY: all clean fclean re
.DEFAULT: all

all: $(NAME)

$(NAME): $(C_FILES) | build
	$(CC) $(C_FLAGS) $^ -o $(NAME) -L ./42_MyLibC -L ./minilibx-linux -lmlx -lXext -lX11 -lft -lm

%.o: %.c 
	$(CC) $(C_FLAGS) -c $< -o $@

build:
	mkdir -p build
	$(MAKE) -C 42_MyLibC

clean:
	rm -rf build
	$(MAKE) -C 42_MyLibC clean

fclean: clean
	-rm -f $(NAME)
	$(MAKE) -C 42_MyLibC fclean

re: fclean all

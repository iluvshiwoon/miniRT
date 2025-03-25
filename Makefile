C_FILES = RT_mandatory/bmp.c RT_mandatory/miniRT.c RT_mandatory/vector1.c RT_mandatory/vector.c RT_mandatory/wrap_malloc.c \
			parsing/parsing.c \
			parsing/check_args.c \
			parsing/open_file.c \
			parsing/alloc_rt.c \
			parsing/element/Ambient_light.c \
			parsing/element/camera.c \
			parsing/element/light.c \
			parsing/element/plane.c \
			parsing/element/sphere.c \
			parsing/element/cylinder.c \
			parsing/element/element_utils.c \
			parsing/element/atoi_double.c

O_FILES = $(C_FILES:%.c=build/%.o)
C_FLAGS = -Wall -Werror -Wextra
NAME = miniRT

.PHONY: all clean fclean re
.DEFAULT: all

all: $(NAME)

$(NAME): $(C_FILES) | build
	$(CC) -g $^ -o $(NAME) -L ./42_MyLibC -lft -lm

%.o: %.c 
	$(CC) -g -c $< -o $@

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

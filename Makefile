C_FILES =  SRC/parsing/parsing.c  SRC/parsing/check_args.c  SRC/parsing/open_file.c  SRC/parsing/alloc_rt.c  SRC/parsing/element/Ambient_light.c  SRC/parsing/element/camera.c  SRC/parsing/element/light.c  SRC/parsing/element/plane.c  SRC/parsing/element/sphere.c  SRC/parsing/element/cylinder.c  SRC/parsing/element/element_utils.c  SRC/parsing/element/atoi_double.c SRC/bmp.c  SRC/miniRT.c  SRC/vector1.c  SRC/vector.c  SRC/wrap_malloc.c

O_FILES = $(C_FILES:%.c=build/%.0)
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

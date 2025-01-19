C_FILES = 
O_FILES = $(C_FILES:%.c=build/%.o)
C_FLAGS = -Wall -Werror -Wextra
NAME = miniRT

.PHONY: all clean fclean re
.DEFAULT: all

all: $(NAME)

$(NAME): $(C_FILES) | build
	$(CC) -O2 $^ -o $(NAME) -L ./42_MyLibC -lft -L ./minilibx-linux -lmlx -lXext -lX11 -lm -lz

%.o: %.c 
	$(CC)  -O2 -c $< -o $@

build:
	mkdir -p build
	$(MAKE) -C 42_MyLibC
	$(MAKE) -C minilibx-linux

clean:
	rm -rf build
	$(MAKE) -C 42_MyLibC clean
	$(MAKE) -C minilibx-linux clean

fclean: clean
	-rm -f $(NAME)
	$(MAKE) -C 42_MyLibC fclean


re: fclean all

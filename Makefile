C_FILES = bmp.c miniRT.c vector1.c vector.c wrap_malloc.c
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

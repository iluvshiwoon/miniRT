SRC		:= 	main.c \
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
			parsing/element/atoi_double.c \
			bmp.c \
			miniRT.c \
			vector1.c \
			vector.c \
			wrap_malloc.c
OBJS	:=	$(SRC:.c=.o)
BUILD	:=	.build
SRC_DIR := 	SRC
C_FILES 	:=	$(addprefix $(SRC_DIR)/, $(SRC))
O_FILES 	:=	$(patsubst $(SRC_DIR)/%.c, $(BUILD)/%.o, $(SRC))
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

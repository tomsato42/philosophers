CC = cc
CFLAGS = -Wall -Wextra -Werror
PTHREAD = -pthread 
HELGRIND_FLAGS = -g

NAME = philo
SRC_DIR = src/
OBJ_DIR = obj/
LIBFT_DIR = libfuture/
LIBFT = $(LIBFT_DIR)libft.a

SRC_FILES = $(SRC_DIR)main.c \
			$(SRC_DIR)init.c \
			$(SRC_DIR)utils.c \
			$(SRC_DIR)philo.c \
			$(SRC_DIR)monitor.c

OBJ_FILES = $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRC_FILES))

all: $(OBJ_DIR) $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ_FILES) $(LIBFT)
	$(CC) $(CFLAGS) $(PTHREAD) -o $@ $^ -L$(LIBFT_DIR) -lft

debug: CFLAGS += $(HELGRIND_FLAGS)
debug: re

helgrind: debug
	valgrind --tool=helgrind ./$(NAME) $(ARGS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re debug helgrind
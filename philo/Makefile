NAME		= philo
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
PTHREAD		= -pthread

SRC_DIR		= ./
OBJ_DIR		= obj/

SRC_FILES	= main.c init.c philo.c monitor.c utils.c ph_atoi.c think.c
OBJ_FILES	= $(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))

all: $(OBJ_DIR) $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(PTHREAD) -o $@ $^

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Object files cleaned"

fclean: clean
	@rm -f $(NAME)
	@echo "Executable cleaned"

re: fclean all

.PHONY: all clean fclean re
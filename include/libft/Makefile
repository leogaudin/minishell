# COLORS
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

CC = cc
CFLAGS = -Wall -Werror -Wextra
# Add any other static library in the same fashion - 1

# INCLUDE: .h files or .a library folders
# SRC: .c files
SRC_DIR = src
OBJ_DIR = obj

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Add any other static library in the same fashion - 2

NAME = libft.a

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ_FILES)
	@ar rcs $(NAME) $(OBJ_FILES)
	@echo "$(GREEN)$(NAME) created.$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)$(NAME) deleted.$(RESET)"

re: fclean all

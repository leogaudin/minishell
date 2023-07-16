# COLORS
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

CC = cc
CFLAGS = -Wall -Werror -Wextra
LIBFT_PATH = include/libft
# Add any other static library in the same fashion - 1

# TODO: reorganize the Makefile and the folder structure
# INCLUDE: .h files or .a library folders
# SRC: .c files
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIRS = include -I $(LIBFT_PATH) -I /opt/homebrew/Cellar/readline/8.2.1/include
LIBS =  -L$(LIBFT_PATH) -lreadline

SYS	= $(shell uname -s)

ifeq ($(SYS), Darwin)
INCLUDE_DIRS +=	-I /opt/vagrant/embedded/include
LIBS	+= -L/opt/vagrant/embedded/lib
endif

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

LIBFT = $(LIBFT_PATH)/libft.a
# Add any other static library in the same fashion - 2

NAME = minishell

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ_FILES) $(LIBFT)
	@$(CC) $(CFLAGS) $(LIBS) -o $@ $^
	@echo "$(GREEN)+ $(NAME)$(RESET)"
# Add any other static library in the same fashion as $(LIBFT) AND -I$(LIBFT_PATH) - 3


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -I $(INCLUDE_DIRS) -c -o $@ $<
# Add any other static library in the same fashion as -I$(LIBFT_PATH) - 4

$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)
# Add any other static library in the same fashion - 5

clean:
	@$(MAKE) -C $(LIBFT_PATH) clean
# Add any other static library in the same fashion - 6
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@$(RM) $(NAME)
	@echo "$(RED)- $(NAME)$(RESET)"
# Add any other static library in the same fashion - 7

re: fclean all

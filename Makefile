# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fpedroso <fpedroso@student.42saopaulo.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/17 16:19:21 by fpedroso          #+#    #+#              #
#    Updated: 2025/12/17 16:19:21 by fpedroso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := gcc
CFLAGS := -Wall -Wextra -Werror -Iinclude -Ilibft

# Directories
SRC_DIR := src
OBJ_DIR := obj
LIBFT_DIR := libft

LIBFT := $(LIBFT_DIR)/libft.a
READLINE_FLAGS := -lreadline

FILES := main.c							\
         minishell_routine.c			\
         get_input_line.c				\
         minishell_utils.c				\
         lexer.c						\
         lexer_2.c						\
         lexer_utils.c					\
         parser.c						\
         parser_2.c						\
         parser_3.c						\
         parser_tools.c					\
         convert_envp_to_env_list.c		\
         env_lst_utils.c				\
         env_lst_utils_2.c           	\
		is_builtin.c					\
         _debug_print_ast_3_pretty.c

SRC := $(addprefix $(SRC_DIR)/,$(FILES))
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(READLINE_FLAGS) -o $(NAME)
	@echo "Minishell compiled"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $<"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

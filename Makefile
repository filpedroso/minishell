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

# ==============================================
# ============= VARIÁVEIS GLOBAIS ==============
# ==============================================

NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude -Ilibft/include

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libft
LEXER_PARSER_DIR = $(SRC_DIR)/lexer_parser
BUILTINS_DIR = $(SRC_DIR)/builtins
EXECUTOR_DIR = $(SRC_DIR)/executor

# Arquivos fonte organizados por diretório
MAIN_SRC = $(SRC_DIR)/main.c

# Lista de arquivos do lexer/parser
LEXER_PARSER_FILES = tokenizer.c tokenizer_utils.c token_creator.c token_utils.c \
                     ast_converter.c ast_utils.c env_utils.c minishell_utils.c
LEXER_PARSER_SRC = $(addprefix $(LEXER_PARSER_DIR)/, $(LEXER_PARSER_FILES))

# Lista de arquivos de builtins
BUILTINS_FILES = builtins.c builtins_adapter.c builtins_env.c \
                 builtins_env_adapter.c builtins_env_utils.c
BUILTINS_SRC = $(addprefix $(BUILTINS_DIR)/, $(BUILTINS_FILES))

# Lista de arquivos do executor
EXECUTOR_FILES = executor.c builtin_logic.c builtins.c cleanup.c command_logic.c \
                 exec_ext_cmd.c get_current_envs.c get_path.c get_path_2.c \
                 handle_redirections_1.c handle_redirections_2.c
EXECUTOR_SRC = $(addprefix $(EXECUTOR_DIR)/, $(EXECUTOR_FILES))

# Todos os arquivos fonte
SRC = $(MAIN_SRC) $(LEXER_PARSER_SRC) $(BUILTINS_SRC) $(EXECUTOR_SRC)

# Objetos mantendo a estrutura de diretórios
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Configuração da libft
LIBFT = $(LIBFT_DIR)/libft.a

# Flags do readline
READLINE_FLAGS = -lreadline

# ==============================================
# ================ REGRAS PRINCIPAIS ===========
# ==============================================

all: $(NAME)

# Regra principal que compila o minishell
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(READLINE_FLAGS)
	@echo "✅ Minishell compilado com sucesso!"

# Regra para compilar os objetos do minishell mantendo estrutura
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compilado: $<"

# ==============================================
# ============= INTEGRAÇÃO COM LIBFT ===========
# ==============================================

# Regra para compilar a libft (se não existir)
$(LIBFT):
	@make -C $(LIBFT_DIR)

# Garante que os objetos do minishell dependem da libft
$(OBJ): | $(LIBFT)

# ==============================================
# ============= REGRAS DE LIMPEZA ==============
# ==============================================

clean:
	rm -f $(OBJ)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
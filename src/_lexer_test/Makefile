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

NAME = test_lexer

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I../../include -I../../libft

# Diretórios
LIBFT_DIR = ../../libft
LIBFT = $(LIBFT_DIR)/libft.a

# Arquivos fonte organizados por diretório
SRC = _lexer_tester.c \
      ../lexer.c \
      ../lexer_2.c \
      ../lexer_utils.c \
      ../not_refactored_yet/tokenizer_utils.c

# Objetos
OBJ = $(SRC:.c=.o)

# ==============================================
# =================== REGRAS ===================
# ==============================================

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) $(LIBFT) -o $(NAME)
	@echo "✅ $(NAME) compiled successfully!"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
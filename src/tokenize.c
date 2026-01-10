/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 03:09:12 by lcosta-a          #+#    #+#             */
/*   Updated: 2026/01/10 15:29:53 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		create_word_token(t_token **tokens, char **input);
static void		create_operator_token(t_token **tokens, char **input);
static t_token	*get_next_token(char **input);


t_token *tokenize(char *input)
{
    t_token	*tokens_lst;
    t_token	*new_token;
    t_token	*tail;

    tokens_lst = NULL;
	tail = NULL;
    while (*input)
    {
        skip_spaces(&input);
        if (!*input)
            break;
        new_token = get_next_token(&input);
        if (!new_token)
            return NULL;
		if (!tokens_lst)
			tokens_lst = new_token;
		else
			tail->next = new_token;
		tail = new_token;
    }
    return (tokens_lst);
}

static t_token	*get_next_token(char **input)
{
	if (is_operator(*input))
		return (create_operator_token(input));
	else
		return (create_word_token(input));
}

static t_token	*create_operator_token(t_token **tokens, char **input)
{
	t_token	*	new_tok;

	new_tok = create_token(NULL, 0, 0);
	if (!new_tok)
	{
		// WHAT TO DO??
	}
	new_tok->value = get_operator_tok_type(*input);
	if (new_tok->type == TOK_APPEND || new_tok->type == TOK_HEREDOC)
		*input += 2;
	else
		(*input)++;
	return (create_token(NULL, 0, type));
}

t_token_type	get_operator_tok_type(char *str_input)
{
	if (is_double_operator(str_input))
	{
		if ((str_input)[0] == '>' && (str_input)[1] == '>')
			return (TOK_APPEND);
		else
			return (TOK_HEREDOC);
	}
	if (*str_input == '|')
		return (TOK_PIPE);
	else if (*str_input == '>')
		return (TOK_OUT);
	else if (*str_input == '<')
		return (TOK_IN);
}

static t_token	*create_word_token(t_token **tokens, char **input)
{
	char	*start;
	int		len;
	char	*word;
	char	original_char;

	original_char = **input;
	start = *input;
	if (**input == '"') 
	{
		printf("DEBUG: Processando aspas duplas\n");
		(*input)++;
		start = *input;
		while (**input && **input != '"')
			(*input)++;
		len = *input - start;
		if (**input == '"') 
			(*input)++;
	}
	else if (**input == '\'')
	{
		printf("DEBUG: Processando aspas simples\n");
		(*input)++;
		start = *input;
		while (**input && **input != '\'')
			(*input)++;
		len = *input - start;
		if (**input == '\'')
			(*input)++;
	}
	else
	{
		printf("DEBUG: Processando palavra sem aspas\n");
		while (**input && !is_operator(**input) && !is_space(**input))
			(*input)++;
		len = *input - start;
	}
	printf("DEBUG: Token criado: '%.*s' (tipo %c)\n", len, start, original_char);
	if (len > 0)
	{
		word = ft_substr(start, 0, len);
		add_token(tokens, create_token(word, len, WORD));
	}
}

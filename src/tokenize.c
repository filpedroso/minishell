/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 03:09:12 by lcosta-a          #+#    #+#             */
/*   Updated: 2026/01/11 17:32:35 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_lst	*create_word_token(t_token_lst **tokens, char **input);
static t_token_lst	*create_operator_token(char **input);
static t_token_lst	*get_next_token(char **input);


t_token_lst *tokenize(char *input)
{
    t_token_lst	*tokens_lst;
    t_token_lst	*new_token;
    t_token_lst	*tail;

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

static t_token_lst	*get_next_token(char **input)
{
	if (is_operator(*input))
		return (create_operator_token(input));
	else
		return (create_word_token(input));
}

//	How to deal with things like ">>>"???
//	We need more input checking and throw "parse errors"
static t_token_lst	*create_operator_token(char **input)
{
	t_token_lst	*	new_tok;
	t_token_type	type;

	type = get_operator_tok_type(*input);
	new_tok = create_token(NULL, 0, type);
	if (!new_tok)
		return (NULL);
	if (type == TOK_APPEND || type == TOK_HEREDOC)
		*input += 2;
	else
		(*input)++;
	return (new_tok);
}

t_token_lst	*create_token(char *value, int len, t_token_type type)
{
	t_token_lst	*token;

	token = malloc(sizeof(t_token_lst));
	if (!token)
		return (NULL);
	if (value && len > 0)
	{
		token->value = malloc(len + 1);
		if (!token->value)
		{
			free(token);
			return (NULL);
		}
		ft_strncpy(token->value, value, len);
		token->value[len] = '\0';
	}
	else
		token->value = NULL;
	token->type = type;
	token->next = NULL;
	return (token);
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

static t_token_lst	*create_word_token(t_token_lst **tokens, char **input)
{
	char	*start;
	int		len;
	char	*word;
	char	debug_orig_char;

	debug_orig_char = **input;
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
	printf("DEBUG: Token criado: '%.*s' (tipo %c)\n", len, start, debug_orig_char);
	if (len > 0)
	{
		word = ft_substr(start, 0, len);
		return (create_token(word, len, TOK_WORD));
	}
}

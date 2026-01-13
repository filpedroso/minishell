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
		(*input) += 1;
	return (new_tok);
}

// REVER ESTA FUNCAO SOB O PRINCIPIO DO ERROR HANDLING!!!
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

// throws parsing error and returns NULL for next iteration
static t_token_lst	*create_word_token(char **input)
{
	t_word_ref	word_ref;

	word_ref = get_word_references(input);
	if (word_ref.parseable == false)
	{
		ft_putstr_fd("Parsing error near ", 2);
		ft_putchar_fd(**input, 2);
		return (NULL);
	}
	return (word_token_from_ref(word_ref));
}

t_word_ref	get_word_references(char **input)
{
	char		*input_str;
	t_word_ref	word_ref;
	char		encloser;
	int			i;

	input_str = *input;
	if (is_encloser(*input_str))
		encloser = *input_str;
	else
		encloser = NO_ENCL;
	i = 0;
	while(input_str[i])
	{
		i++;
		if (input_str[i] = encloser)
			return (generate_word_ref(input, 1, i - 1, encloser));
	}
	if (encloser == NO_ENCL)
		return (generate_word_ref(input, 0, i - 1, 0));
	return (generate_word_ref(input, 0, -1, 0));
}

t_word_ref	generate_word_ref(char **input, int start, int end, char encloser)
{
	t_word_ref	word_ref;

	if (start > end)
	{
		word_ref.parseable = false;
		return (word_ref);
	}
	word_ref.input_ptr = input;
	word_ref.first_idx = start;
	word_ref.last_idx = end;
	word_ref.encloser = encloser;
	word_ref.parseable = true;
	return (word_ref);
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

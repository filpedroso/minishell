/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_expander_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 21:55:37 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/15 13:54:21 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_var_name_len(t_word word, int start);
static char	*handle_dollar(t_word word, int *i, char *res, char **envs);
static char	*append_literal_run(t_word word, int *i, char *result);

char	*expand_word_with_context(t_sh *sh, t_word word, char **envs)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (result && word.token_word_ptr[i])
	{
		if (word.token_word_ptr[i] == '$'
			&& word.context_mask_ptr[i] != CONTEXT_SINGLE)
		{
			if (word.token_word_ptr[i + 1] == '?')
				result = (handle_exit_status_expansion(sh, &i, result));
			else
				result = handle_dollar(word, &i, result, envs);
		}
		else
			result = append_literal_run(word, &i, result);
	}
	return (result);
}

static char	*handle_dollar(t_word word, int *i, char *result, char **envs)
{
	int		var_len;
	char	*value;

	(*i)++;
	var_len = get_var_name_len(word, *i);
	if (var_len == 0)
		return (join_and_free_left(result, "$"));
	value = lookup_env_var(&word.token_word_ptr[*i], var_len, envs);
	if (!value)
	{
		free(result);
		return (NULL);
	}
	*i += var_len;
	result = join_and_free_left(result, value);
	free(value);
	return (result);
}

static char	*append_literal_run(t_word word, int *i, char *result)
{
	int		start;
	char	*seg;

	start = *i;
	while (word.token_word_ptr[*i]
		&& !(word.token_word_ptr[*i] == '$'
			&& word.context_mask_ptr[*i] != CONTEXT_SINGLE))
		(*i)++;
	seg = ft_substr(word.token_word_ptr, start, *i - start);
	if (!seg)
	{
		free(result);
		return (NULL);
	}
	result = join_and_free_left(result, seg);
	free(seg);
	return (result);
}

char	*join_and_free_left(char *left, const char *right)
{
	char	*result;

	if (!right || !*right)
		return (left);
	if (!left)
		return (ft_strdup(right));
	result = ft_strjoin(left, right);
	free(left);
	return (result);
}

static int	get_var_name_len(t_word word, int start)
{
	int	len;

	if (!word.token_word_ptr[start]
		|| word.context_mask_ptr[start] == CONTEXT_SINGLE
		|| (!ft_isalpha(word.token_word_ptr[start])
			&& word.token_word_ptr[start] != '_'))
		return (0);
	len = 1;
	while (word.token_word_ptr[start + len]
		&& word.context_mask_ptr[start + len] != CONTEXT_SINGLE
		&& (ft_isalnum(word.token_word_ptr[start + len])
			|| word.token_word_ptr[start + len] == '_'))
		len++;
	return (len);
}

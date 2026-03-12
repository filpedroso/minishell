/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_expander_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 21:55:37 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/07 13:09:49 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		get_var_name_len(t_word word, int start);
static t_word	handle_dollar(t_word word, int *i, t_word res, char **envs);
static t_word	append_literal_run(t_word word, int *i, t_word result);

t_word	expand_word_with_context(t_sh *sh, t_word word, char **envs)
{
	t_word	result;
	int		i;

	result.token_word_ptr = ft_strdup("");
	result.context_mask_ptr = ft_strdup("");
	if (!result.token_word_ptr || !result.context_mask_ptr)
		return (free_word(result), result);
	i = 0;
	while (result.token_word_ptr && word.token_word_ptr[i])
	{
		if (word.token_word_ptr[i] == '$'
			&& word.context_mask_ptr[i] != CONTEXT_SINGLE)
		{
			if (word.token_word_ptr[i + 1] == '?')
				result = handle_exit_status_expansion(sh, &i, result,
						word.context_mask_ptr[i]);
			else
				result = handle_dollar(word, &i, result, envs);
		}
		else
			result = append_literal_run(word, &i, result);
	}
	return (result);
}

static t_word	handle_dollar(t_word word, int *i, t_word res, char **envs)
{
	int		var_len;
	char	*value;
	char	*mask;
	char	ctx;

	ctx = word.context_mask_ptr[(*i)++];
	var_len = get_var_name_len(word, *i);
	if (var_len == 0)
	{
		mask = make_ctx_mask(1, ctx);
		res.token_word_ptr = join_and_free_left(res.token_word_ptr, "$");
		res.context_mask_ptr = join_and_free_left(res.context_mask_ptr, mask);
		free(mask);
		return (res);
	}
	value = lookup_env_var(&word.token_word_ptr[*i], var_len, envs);
	if (!value)
		return (free_word(res), res);
	mask = make_ctx_mask(ft_strlen(value), ctx);
	*i += var_len;
	res.token_word_ptr = join_and_free_left(res.token_word_ptr, value);
	res.context_mask_ptr = join_and_free_left(res.context_mask_ptr, mask);
	free(value);
	free(mask);
	return (res);
}

static t_word	append_literal_run(t_word word, int *i, t_word result)
{
	int		start;
	char	*seg;
	char	*mask_seg;

	start = *i;
	while (word.token_word_ptr[*i] && !(word.token_word_ptr[*i] == '$'
			&& word.context_mask_ptr[*i] != CONTEXT_SINGLE))
		(*i)++;
	seg = ft_substr(word.token_word_ptr, start, *i - start);
	mask_seg = ft_substr(word.context_mask_ptr, start, *i - start);
	if (!seg || !mask_seg)
	{
		free(seg);
		free(mask_seg);
		free_word(result);
		return (result);
	}
	result.token_word_ptr = join_and_free_left(result.token_word_ptr, seg);
	result.context_mask_ptr = join_and_free_left(result.context_mask_ptr,
			mask_seg);
	free(seg);
	free(mask_seg);
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
	while (word.token_word_ptr[start + len] && word.context_mask_ptr[start
			+ len] != CONTEXT_SINGLE && (ft_isalnum(word.token_word_ptr[start
					+ len]) || word.token_word_ptr[start + len] == '_'))
		len++;
	return (len);
}

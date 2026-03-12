/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_expander_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:00:00 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/07 13:09:47 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*make_ctx_mask(int len, char ctx)
{
	char	*mask;

	mask = malloc(len + 1);
	if (!mask)
		return (NULL);
	ft_memset(mask, ctx, len);
	mask[len] = '\0';
	return (mask);
}

char	*lookup_env_var(const char *name, int name_len, char **envs)
{
	int	i;

	i = 0;
	while (envs[i])
	{
		if (ft_strncmp(name, envs[i], name_len) == 0
			&& envs[i][name_len] == '=')
			return (ft_strdup(envs[i] + name_len + 1));
		i++;
	}
	return (ft_strdup(""));
}

t_word	handle_exit_status_expansion(t_sh *sh, int *i, t_word result, char ctx)
{
	char	*value;
	char	*mask;

	*i += 2;
	value = ft_itoa(sh->last_exit_st);
	if (!value)
		return (free_word(result), result);
	mask = make_ctx_mask(ft_strlen(value), ctx);
	result.token_word_ptr = join_and_free_left(result.token_word_ptr, value);
	result.context_mask_ptr = join_and_free_left(result.context_mask_ptr, mask);
	free(value);
	free(mask);
	return (result);
}

void	free_word(t_word word)
{
	free(word.token_word_ptr);
	free(word.context_mask_ptr);
	word.token_word_ptr = NULL;
	word.context_mask_ptr = NULL;
}

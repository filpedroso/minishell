/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_expander_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 21:55:37 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/02 21:55:37 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_word_with_context(t_word word, char **envs)
{
    char        *result;
    t_status    status;
    int         i;

    result = NULL;
    i = 0;
    status = STATUS_OK;
    while (word.token_word_ptr[i] && status == STATUS_OK)
    {
        if (word.token_word_ptr[i] == '$' && word.context_mask_ptr[i] != CONTEXT_SINGLE)
		{
			i++;
			if (is_valid_var_name_and_not_single_quoted(word, i))
				i += append_expanded_var(&result, &word.token_word_ptr[i], envs, &status);
			else
            	append_char(&result, '$', &status);
		}
        else
            i += append_char(&result, word.token_word_ptr[i], &status);
    }
    return (result);
}

bool	is_valid_var_name_and_not_single_quoted(t_word word, int i)
{
	return (word.token_word_ptr[i]
		&& word.context_mask_ptr[i] != CONTEXT_SINGLE);
}

static int append_char(char **result, char c, t_status *status)
{
    char	*new_result;

    new_result = ft_strjoin(*result, &c);
    if (!new_result)
    {
        free(*result);
        *result = NULL;
        *status = STATUS_ERR;
        return (1);
    }
    *result = new_result;
	return (1);
}

static int append_expanded_var(char **result, const char *var_name,
                                 char **envs, t_status *status)
{
    char	*var_value;
    char	*new_result;
    int		var_len;

    var_len = ft_strlen(var_name);
    var_value = get_var_value(var_name, var_len, envs);
    if (!var_value)
    {
        free(*result);
        *result = NULL;
        *status = STATUS_ERR;
        return (var_len);
    }
    new_result = ft_strjoin(*result, var_value);
    free(var_value);
    if (!new_result)
    {
        free(*result);
        *result = NULL;
        *status = STATUS_ERR;
        return (var_len);
    }
    free(*result);
    *result = new_result;
    return (var_len);
}

char	*get_var_value(const char *var_name, int var_len, char **envs)
{
	char	*value;
	int		i;

	value = NULL;
	i = 0;
	while (envs[i])
	{
		if (ft_strncmp(var_name, envs[i], var_len) == 0)
		{
			value = ft_strdup(envs[i] + var_len + 1);
			if (!value)
				retun (NULL);
			return (value);
		}
	}
	return (ft_strdup(""));
}

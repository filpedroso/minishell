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

static char *expand_word_with_context(t_word word, char **envs)
{
    char        *result;
    t_status    status;
    int         i;

    result = NULL;
    i = 0;
    status = STATUS_OK;
    while (word.token_word_ptr[i] && status == STATUS_OK)
    {
        if (word.token_word_ptr[i] == '$' && should_expand(word, i))
            i += expand_var_in_result(&result, &word.token_word_ptr[i + 1],
                                      envs, &status);
        else
            append_char_in_place(&result, word.token_word_ptr[i], &status);
        i++;
    }
    return (result);
}

static void append_char_in_place(char **result, char c, t_status *status)
{
    char    *new_result;
    int     len;

    len = *result ? ft_strlen(*result) : 0;
    new_result = malloc(len + 2);
    if (!new_result)
    {
        free(*result);
        *result = NULL;
        *status = STATUS_ERR;
        return;
    }
    if (*result)
    {
        ft_memcpy(new_result, *result, len);
        free(*result);      // Free old
    }
    new_result[len] = c;
    new_result[len + 1] = '\0';
    *result = new_result;
}

static int expand_var_in_result(char **result, const char *var_start, 
                                 char **envs, t_status *status)
{
    char    *var_value;
    char    *new_result;
    int     var_len;

    var_len = get_var_name_len(var_start);
    var_value = get_var_value(var_start, var_len, envs);
    if (!var_value)
    {
        free(*result);          // Free on error
        *result = NULL;         // Set to NULL
        *status = STATUS_ERROR;
        return (var_len);
    }
    new_result = ft_strjoin_safe(*result, var_value);
    free(var_value);
    if (!new_result)
    {
        free(*result);          // Free on error
        *result = NULL;         // Set to NULL
        *status = STATUS_ERROR;
        return (var_len);
    }
    free(*result);              // Free old on success
    *result = new_result;
    return (var_len);
}

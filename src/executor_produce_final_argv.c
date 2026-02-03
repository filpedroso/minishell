/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_produce_final_argv.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 21:12:02 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/02 21:12:02 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_word_list(t_argv_str_lst *expanded_words);
static char	**word_list_to_array(t_argv_str_lst *list, int argc);
static void	free_word_list_nodes_only(t_argv_str_lst *list);
static void free_word_list(t_argv_str_lst *list);

char    **produce_final_argv(t_command *cmd, char **current_envs)
{
    t_argv_str_lst	*expanded_words;
    char			**argv;
    int				argc;

    expanded_words = expand_all_words(cmd, current_envs);
	if (!expanded_words)
		return (NULL);
    argc = count_word_list(expanded_words);
    argv = word_list_to_array(expanded_words, argc);
	if (!argv)
	{
    	free_word_list(expanded_words);
		return (NULL);
	}
    free_word_list_nodes_only(expanded_words);
    return (argv);
}

static int	count_word_list(t_argv_str_lst *expanded_words)
{
	int	count;

	count = 0;
	while (expanded_words)
	{
		count++;
		expanded_words = expanded_words->next;
	}
	return (count);
}

static char    **word_list_to_array(t_argv_str_lst *list, int argc)
{
    char    **argv;
    int     i;

    argv = malloc(sizeof(char *) * (argc + 1));
    if (!argv)
        return (NULL);
    i = 0;
    while (list)
    {
        argv[i] = list->string;
        list = list->next;
        i++;
    }
    argv[i] = NULL;
    return (argv);
}

static void    free_word_list_nodes_only(t_argv_str_lst *list)
{
    t_argv_str_lst  *tmp;

    while (list)
    {
        tmp = list->next;
        free(list);
        list = tmp;
    }
}

static void free_word_list(t_argv_str_lst *list)
{
    t_argv_str_lst  *tmp;

    while (list)
    {
        tmp = list->next;
        free(list->string);
        free(list);
        list = tmp;
    }
}

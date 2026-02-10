/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_expander.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 21:13:07 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:48:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	can_split(t_word *word);
static void	append_split_words(t_str_lst **head, t_str_lst **tail, char *str);
static void	append_single_word(t_str_lst **head, t_str_lst **tail, char *str);

t_str_lst	*expand_all_words(t_command *cmd, char **current_envs)
{
	char		*expanded;
	t_str_lst	*head;
	t_str_lst	*tail;
	int			i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (i < cmd->words_count)
	{
		expanded = expand_word_with_context(cmd->words[i], current_envs);
		if (can_split(&cmd->words[i]))
			append_split_words(&head, &tail, expanded);
		else
			append_single_word(&head, &tail, expanded);
		i++;
	}
	return (head);
}

bool	can_split(t_word *word)
{
	int	i;

	i = 0;
	while (word->context_mask_ptr[i])
	{
		if (word->context_mask_ptr[i] == CONTEXT_UNQUOTED)
			return (true);
		i++;
	}
	return (false);
}

void	append_split_words(t_str_lst **head, t_str_lst **tail, char *str)
{
	char	**split;
	int		i;

	split = ft_split(str, " \t\n");
	i = 0;
	while (split[i])
	{
		append_single_word(head, tail, split[i]);
		i++;
	}
	free(split);
}

void	append_single_word(t_str_lst **head, t_str_lst **tail, char *str)
{
	t_str_lst	*node;

	node = malloc(sizeof(t_str_lst));
	if (!node)
		return ;
	node->value = str;
	node->next = NULL;
	if (!*head)
		*head = node;
	else
		(*tail)->next = node;
	*tail = node;
}

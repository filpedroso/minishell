/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_expander.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 21:13:07 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/07 14:22:46 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_words(t_str_lst **head, t_str_lst **tail, t_word exp_word);
static void	append_single_word(t_str_lst **head, t_str_lst **tail, char *str);
static bool	is_unquoted_and_space(t_word word, int i);

t_str_lst	*expand_all_words(t_sh *sh, t_cmd *cmd, char **current_envs)
{
	t_word		exp_word;
	t_str_lst	*head;
	t_str_lst	*tail;
	int			i;
	int			status;

	head = NULL;
	tail = NULL;
	i = 0;
	while (i < cmd->words_count)
	{
		exp_word = expand_word_with_context(sh, cmd->words[i], current_envs);
		if (!exp_word.token_word_ptr || !exp_word.context_mask_ptr)
			return (NULL);
		status = append_words(&head, &tail, exp_word);
		free(exp_word.token_word_ptr);
		free(exp_word.context_mask_ptr);
		if (status == STATUS_ERR)
			return (free_str_lst(head), NULL);
		i++;
	}
	return (head);
}

void	free_str_lst(t_str_lst *lst)
{
	t_str_lst	*current;
	t_str_lst	*next;

	if (!lst)
		return ;
	current = lst;
	while (current)
	{
		next = current->next;
		if (lst->value)
			free(lst->value);
		lst->value = NULL;
		current = next;
	}
}

static int	append_words(t_str_lst **head, t_str_lst **tail, t_word exp_word)
{
	int		i;
	int		start;
	char	*word;

	i = 0;
	while (exp_word.token_word_ptr[i])
	{
		while (is_unquoted_and_space(exp_word, i))
			i++;
		if (!exp_word.token_word_ptr[i])
			break ;
		start = i;
		while (exp_word.token_word_ptr[i] && !is_unquoted_and_space(exp_word,
				i))
			i++;
		word = ft_substr(exp_word.token_word_ptr, start, i - start);
		if (!word)
			return (STATUS_ERR);
		append_single_word(head, tail, word);
	}
	return (STATUS_OK);
}

static bool	is_unquoted_and_space(t_word word, int i)
{
	return (word.context_mask_ptr[i] == CONTEXT_UNQUOTED
		&& (word.token_word_ptr[i] == ' ' || word.token_word_ptr[i] == '\t'));
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

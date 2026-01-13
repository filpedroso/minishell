/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_converter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 04:31:27 by lcosta-a          #+#    #+#             */
/*   Updated: 2025/12/30 21:00:25 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_parser.h"
#include "executor.h"
#include "libft.h"

static	t_node *create_simple_command(t_token **tokens) __attribute__((unused));

static	t_node *create_simple_command(t_token **tokens)
{
	t_node	*node;
	t_command	*cmd;
	int	argc;
	t_token	*temp;
	char	**argv;
	char	*dup;

	node = malloc(sizeof(t_node));
	if (!node)
		return NULL;
	ft_memset(node, 0, sizeof(t_node));
	
	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		free(node);
		return NULL;
	}
	ft_memset(cmd, 0, sizeof(t_command));
	
	node->type = NODE_CMD;
	cmd->type = BUILTIN;
	cmd->is_pipeline = 0;
	
	argc = 0;
	temp = *tokens;
	while (temp && (temp->type == WORD || is_redirection(temp->type)))
	{
		if (temp->type == WORD)
			argc++;
		temp = temp->next;
	}
	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv)
	{
		free(cmd);
		free(node);
		return NULL;
	}
	argc = 0;
	temp = *tokens;
	while (temp && (temp->type == WORD || is_redirection(temp->type)))
	{
		if (temp->type == WORD)
		{
			dup = ft_strdup(temp->value);
			if (!dup)
			{
				while (argc > 0)
					free(argv[--argc]);
				free(argv);
				free(cmd);
				free(node);
				return NULL;
			}
			argv[argc++] = dup;
		}
		temp = temp->next;
	}
	argv[argc] = NULL;
	cmd->args = argv;
	node->cmd = cmd;
	return node;
}

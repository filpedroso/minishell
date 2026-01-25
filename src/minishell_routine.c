/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_routine.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 10:47:45 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/11 15:36:55 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cycle_result	one_shell_cycle(t_env_vars env_vars);
static char				*get_input_line(void);

int	minishell_routine(t_env_vars env_vars)
{
	t_cycle_result	result;

	while(1)
	{
		result = one_shell_cycle(env_vars);
		if (result == CYCLE_CONTINUE)
			continue;
		if (result == CYCLE_EXIT)
			return (SUCCESS);
		else
			return (ERROR);
	}
}

static t_cycle_result	one_shell_cycle(t_env_vars env_vars)
{
	char		*input;
	t_token_lst	*tokens;
	t_ast		ast;

	input = get_input_line();
	if (!input)
		return (CYCLE_EXIT); // ctrl-D
	tokens = lexer(input);
	if (!tokens) // NULL tokens means fatal; Empty tokens will be passed on, normally
	{
		cleanup(input, NULL, NULL);
		return (CYCLE_FATAL);
	}
	ast = make_ast(tokens);
	if (ast.parse_status == PARSE_FATAL)
	{
		cleanup(input, tokens, NULL);
		return (CYCLE_FATAL);
	}
	if (ast.parse_status == PARSE_ERROR)
		return (CYCLE_CONTINUE);
	execute_tree(ast);
	cycle_cleanup(input, tokens, ast);	//terminal cleanup + data cleanup
	return (CYCLE_CONTINUE);
}

static char	*get_input_line(void)
{
	char	*input;

	input = readline("> ");
	if (!input)
		return (NULL);
	if (*input)
		add_history(input);
	return (input);
}

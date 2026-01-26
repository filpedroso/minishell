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
	if (!input)				// ctrl-D
		return (CYCLE_EXIT);
	tokens = lexer(input);
	if (!tokens)			// NULL tokens means fatal; Empty tokens will be passed on, normally
		return (cycle_lexer_err(input));
	ast = make_ast(tokens);
	if (ast.parse_status != PARSE_OK)
		return (cycle_parser_err(input, tokens, ast));
	execute_tree(ast);
	cycle_cleanup(input, tokens, ast);	//terminal cleanup + data cleanup
	return (CYCLE_CONTINUE);
}

t_cycle_result	cycle_lexer_err(char *input)
{
	cycle_cleanup(input, NULL, NULL);
	return (CYCLE_FATAL);
}

t_cycle_result	cycle_parser_err(char *input, t_token_lst *tokens, t_ast ast)
{
	cycle_cleanup(input, tokens, ast);
	if (ast.parse_status == PARSE_FATAL)
		return (CYCLE_FATAL);
	if (ast.parse_status == PARSE_ERROR)
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

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
static t_cycle_result	cycle_lexer_err(char *input);
static t_cycle_result	cycle_parser_err(char *input, t_token_lst *tokens, t_ast ast);
static void				cycle_cleanup(char *input, t_token_lst *tok_lst, t_ast ast);

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
	//execute_tree(ast);
	debug_print_ast(ast.ast_root);
	cycle_cleanup(input, tokens, ast);	//terminal cleanup + data cleanup
	return (CYCLE_CONTINUE);
}

static void	cycle_cleanup(char *input, t_token_lst *tok_lst, t_ast ast)
{
	if (input)
		free(input);
	if (tok_lst)
		free_tok_lst(tok_lst);
	if (ast.ast_root)
		destroy_ast(ast.ast_root);
}

static t_cycle_result	cycle_lexer_err(char *input)
{
	cycle_cleanup(input, NULL, NULL);
	return (CYCLE_FATAL);
}

static t_cycle_result	cycle_parser_err(char *input, t_token_lst *tokens, t_ast ast)
{
	cycle_cleanup(input, tokens, ast);
	if (ast.parse_status == PARSE_FATAL)
		return (CYCLE_FATAL);
	if (ast.parse_status == PARSE_ERROR)
		return (CYCLE_CONTINUE);
}

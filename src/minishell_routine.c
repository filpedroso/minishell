/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_routine.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 10:47:45 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/28 21:11:10 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cycle_result	one_shell_cycle(t_sh *sh);
static t_cycle_result	cycle_lexer_err(char *input);
static t_cycle_result	cycle_parser_err(char *input, t_token_lst *tokens,
							t_ast ast);
static void				cycle_cleanup(char *input, t_token_lst *tok_lst,
							t_ast_node *ast_root);

int	minishell_routine(t_sh *shell)
{
	t_cycle_result	result;

	while (1)
	{
		result = one_shell_cycle(shell);
		if (result == CYCLE_CONTINUE)
			continue ;
		if (result == CYCLE_EXIT)
			return (SUCCESS);
		else
			return (ERROR);
	}
}

static t_cycle_result	one_shell_cycle(t_sh *sh)
{
	char		*input;
	char		*input_base;

	set_signals_interactive();
	input = get_input_line(sh);
	if (!input)
		return (CYCLE_EXIT);
	input_base = input;
	sh->tokens = lexer(&input);
	if (!sh->tokens)
		return (cycle_lexer_err(input_base));
	sh->ast = make_ast(sh->tokens, sh->env_vars);
	if (sh->ast.parse_status != PARSE_OK)
		return (cycle_parser_err(input_base, sh->tokens, sh->ast));
	execute_tree(sh, sh->ast.ast_root);
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		sh->last_exit_st = 130;
	}
	// cycle_cleanup(input_base, sh.tokens, ast.ast_root); // terminal cleanup + data cleanup
	return (CYCLE_CONTINUE);
}

static void	cycle_cleanup(char *input, t_token_lst *tok_lst,
		t_ast_node *ast_root)
{
	if (input)
	{
		free(input);
		input = NULL;
	}
	if (tok_lst)
		free_tok_lst(tok_lst);
	if (ast_root)
		destroy_ast(ast_root);
}

static t_cycle_result	cycle_lexer_err(char *input)
{
	cycle_cleanup(input, NULL, NULL);
	return (CYCLE_FATAL);
}

static t_cycle_result	cycle_parser_err(char *input, t_token_lst *tokens,
		t_ast ast)
{
	cycle_cleanup(input, tokens, ast.ast_root);
	if (ast.parse_status == PARSE_FATAL)
		return (CYCLE_FATAL);
	else
		return (CYCLE_CONTINUE);
}

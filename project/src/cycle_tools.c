/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 19:32:20 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/14 19:32:47 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cycle_cleanup(char *input, t_token_lst *tok_lst,
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

t_cycle_result	cycle_lexer_err(char *input)
{
	cycle_cleanup(input, NULL, NULL);
	return (CYCLE_FATAL);
}

t_cycle_result	cycle_parser_err(char *input, t_token_lst *tokens, t_ast ast)
{
	cycle_cleanup(input, tokens, ast.ast_root);
	if (ast.parse_status == PARSE_FATAL)
		return (CYCLE_FATAL);
	else
		return (CYCLE_CONTINUE);
}

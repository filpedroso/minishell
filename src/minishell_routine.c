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

/*

** ERROR HANDLING PROPOSITION:

**	Malloc/open/close error = FATAL!	>>> handling: perror() + exit(1) immediately;
**	NULL ptr = invalid or empty data	>>> handling: if NULL return NULL,
**		until caller does: if NULL continue; (for next iteration)

*/

#include "minishell.h"

void	minishell_routine(t_env_vars env_vars)
{
	char			*input;
	t_token_lst		*tokens;
	t_ast_node		*ast;

	while(1)
	{
		input = get_input_line();
		tokens = lexer(input);	// NULL tokens means empty input!
		if (!tokens)
		{
			cleanup(input, tokens, ast);
			terminal_cleanup(); // rl_replace_line("", 0) rl_on_new_line() rl_redisplay()
			continue ;
		}
		ast = create_ast_from_tokens(tokens, env_vars);
		expand(ast);
		execute_tree(ast);
		terminal_cleanup(); // rl_replace_line("", 0) rl_on_new_line() rl_redisplay()
		cleanup(input, tokens, ast);
	}
}

char	*get_input_line(void)
{
	char	*input;

	input = readline("> ");
	if (!input)
	{
		ft_putchar('\n');
		exit(1);
	}
	if (input[0])
		add_history(input);
	return (input);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_routine.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 10:47:45 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/10 11:01:02 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell_routine(t_env_vars env_vars)
{
	char		*input;
	t_token		*tokens;
	t_ast_node	*ast;

	while(1)
	{
		// in case of error, each function exits internally with the appropiate code
		input = get_input_line();
		tokens = tokenize(input);
		ast = create_ast_from_tokens(tokens, env_vars);
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

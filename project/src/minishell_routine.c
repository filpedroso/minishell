/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_routine.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 10:47:45 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/15 15:58:54 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cycle_result	one_shell_cycle(t_sh *sh);
static void				executor(t_sh *sh);
static bool				free_input_if_unterm_quote(char *input);


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
	char	*input;
	char	*input_base;

	set_signals_interactive();
	input = get_input_line(sh);
	if (!input)
		return (CYCLE_EXIT);
	if (free_input_if_unterm_quote(input))
		return (CYCLE_CONTINUE);
	input_base = input;
	sh->tokens = lexer(&input);
	if (!sh->tokens)
		return (cycle_lexer_err(input_base));
	sh->ast = make_ast(sh->tokens, sh->env_vars);
	if (sh->ast.parse_status != PARSE_OK)
		return (cycle_parser_err(input_base, sh->tokens, sh->ast));
	executor(sh);
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		sh->last_exit_st = 130;
	}
	unlink_heredoc_files(&sh->heredoc_files);
	cycle_cleanup(input_base, sh->tokens, sh->ast.ast_root);
	return (CYCLE_CONTINUE);
}

static void	executor(t_sh *sh)
{
	struct termios	saved_termios;
	int				is_tty;

	is_tty = tcgetattr(STDIN_FILENO, &saved_termios);
	if (collect_all_heredocs(sh, sh->ast.ast_root) == 0)
		execute_tree(sh, sh->ast.ast_root);
	if (is_tty == 0)
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &saved_termios);
}

static bool free_input_if_unterm_quote(char *input)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		i++;
	}
	if (quote)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
		free(input);
		return (true);
	}
	return (false);
}

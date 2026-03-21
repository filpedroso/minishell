/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 20:48:44 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/21 14:40:43 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_cleanup(t_sh *sh, char **argv, char **envp);

int	ft_exit(t_sh *sh, char **argv, char **envp)
{
	int	code;

	ft_putstr_fd("exit\n", 1);
	if (!argv[1])
	{
		exit_cleanup(sh, argv, envp);
		exit(sh->last_exit_st);
	}
	if (!ft_is_numeric_str(argv[1]))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit_cleanup(sh, argv, envp);
		exit(2);
	}
	code = ft_atoi(argv[1]);
	exit_cleanup(sh, argv, envp);
	exit((unsigned char)code);
}

static void	exit_cleanup(t_sh *sh, char **argv, char **envp)
{
	free_str_arr(argv);
	free_str_arr(envp);
	free(sh->input_base);
	unlink_heredoc_files(&sh->heredoc_files);
	cleanup_env(sh->env_vars);
	cycle_cleanup(NULL, sh->tokens, sh->ast.ast_root);
}

bool	is_exit_builtin(t_ast_node *node)
{
	char	*cmd;

	if (!node
		|| !node->cmd
		|| !node->cmd->words
		|| !node->cmd->words[0].token_word_ptr)
		return (false);
	cmd = node->cmd->words[0].token_word_ptr;
	return (ft_strncmp(cmd, "exit", 5) == 0);
}

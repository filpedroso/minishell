/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 16:52:35 by fpedroso          #+#    #+#             */
/*   Updated: 2025/11/22 16:52:35 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include "libft.h"
#include "executor_types.h"


#define READ			0
#define WRITE			1
#define CHILD			0
#define N_BUILTINS		7
#define N_REDIR_CASES	6

// MAIN EXECUTION
void	execute_tree(t_node *node);
void	command_logic(t_node *node);
void	handle_redirections(t_command cmd);
void	set_heredoc_redir(t_command *cmd, char *heredoc_delim);
void	exec_ext_cmd(t_node *node);
char	*get_cmd_path(t_command cmd);
char	*find_in_path(char *cmd_str, char *path_env);
int		exec_forked_builtin(t_node *node);
int		exec_builtin(t_node *node);
void    init_builtin_table(t_builtin table[N_BUILTINS]);

// BUILTINS
// TODO
// TODO
// TODO
// TODO
// TODO
// TODO
// TODO

// CLEANUP
// TODO
// TODO
// TODO
// TODO

#endif

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

#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include "libft.h"


#define READ			0
#define WRITE			1
#define CHILD			0
#define N_BUILTINS		7
#define N_REDIR_CASES	6


typedef struct	s_node
{
	t_node_type		type;
	struct s_node	*left;
	struct s_node	*right;
	t_command		cmd;
} t_node;

typedef enum	e_node_type
{
	PIPE,
	CMD
} t_node_type;

typedef enum	e_cmd_type
{
	EXT,
	BUILTIN
} t_cmd_type;

typedef struct	s_command
{
	t_cmd_type		type;
	char			**args;
	bool			is_pipeline;
	t_env_vars		env_vars;
	t_redirection	*redirections;
	int				redirections_count;
	t_temp_files	*temp_files_list;
} t_command;

typedef struct	s_redirection
{
	t_redirection_type	type;
	char				*arg;
	char				*target;
} t_redirection;

typedef enum	e_redirection_type
{
	IN,
	OUT,
	APPEND,
	HEREDOC
} t_redirection_type;

typedef struct	s_env_vars
{
	t_var_list	*persistent_envs;
	t_var_list	*inline_envs;
} t_env_vars;

typedef struct	s_temp_files
{
	char				*path;
	struct s_temp_files	*next;
} t_temp_files;

typedef struct	s_var_list
{
	char				*var_name;
	char				*value;
	struct s_var_list	*next;
} t_var_list;

typedef struct	s_builtin
{
    char	*name;
    int		(*func)(char **args, char **envp);
} t_builtin;

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

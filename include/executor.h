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
	bool			is_pipeline;
} t_node;

typedef enum	e_node_type
{
	PIPE,
	EXT_CMD,
	BUILTIN
} t_node_type;

typedef struct	s_command
{
	char			**args;
	t_redirection	*redirections;
	int				redirections_count;
	t_env_vars		env_vars;
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
	t_var_list	persistent_envs;
	t_var_list	inline_envs;
} t_env_vars;

typedef struct	s_var_list
{
	char			*env_var;
	char			*value;
	struct s_list	*next;
	struct s_list	*previous;
} t_var_list;

typedef struct	s_builtin
{
    char	*name;
    int		(*func)(char **args, char **envp);
} t_builtin;

char	*get_path(char **cmds, char **envp);
void	handle_redirections(t_command cmd);

#endif

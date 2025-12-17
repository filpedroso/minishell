/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_types.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 21:21:54 by fpedroso          #+#    #+#             */
/*   Updated: 2025/12/16 21:21:54 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_TYPES_H
# define EXECUTOR_TYPES_H

#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>

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

#endif

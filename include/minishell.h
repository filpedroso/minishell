/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:41:05 by lcosta-a          #+#    #+#             */
/*   Updated: 2026/01/11 15:05:20 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/* ************************    include libs    ****************************** */
/* ************************************************************************** */

#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include "libft.h"
#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

/* ************************************************************************** */
/* ***************************    macros    ********************************* */
/* ************************************************************************** */

#define SUCCESS			0
#define ERROR			-1
#define READ			0
#define NO_ENCL			6
#define WRITE			1
#define CHILD			0
#define N_BUILTINS		7
#define N_REDIR_CASES	6

/* ************************************************************************** */
/* ***************************    types     ********************************* */
/* ************************************************************************** */

typedef enum e_status {
    STATUS_OK,
	STATUS_ERR
} t_status;

typedef enum e_cycle_result
{
	CYCLE_CONTINUE,
	CYCLE_EXIT,
	CYCLE_FATAL
}	t_cycle_result;

typedef enum	e_lexer_state
{
	STATE_NORMAL,
	STATE_SING_QUOTE,
	STATE_DOUB_QUOTE,
	STATE_OPERATOR,
    STATE_ERROR,
	STATE_TOK_END,
	STATE_TOK_END_NO_EAT
} t_lexer_state;

typedef enum e_builtin_type
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	NOT_BUILTIN
} t_builtin_type;

typedef enum	e_redirection_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
} t_redirection_type;

typedef struct s_redirection
{
	t_redirection_type	type;
	char				*arg;
	char				*target;
} t_redirection;

typedef enum	e_cmd_type
{
	EXT,
	BUILTIN
} t_cmd_type;

typedef struct	s_var_lst
{
	char				*var_name;
	char				*value;
	struct s_var_lst	*next;
} t_var_lst;

typedef struct	s_env_vars
{
	t_var_lst	*persistent_envs;
	t_var_lst	*inline_envs;
} t_env_vars;

typedef struct	s_file_lst
{
	char				*path;
	struct s_file_lst	*next;
} t_file_lst;

typedef enum	e_node_type
{
	NODE_PIPE,
	NODE_CMD
} t_node_type;

typedef struct	s_command
{
	t_cmd_type		type;
	char			**args;
	bool			is_pipeline;
	t_env_vars		env_vars;
	t_redirection	*redirections;
	int				redirections_count;
	t_file_lst		*temp_files_list;
} t_command;

typedef struct	s_ast_node
{
	t_node_type		type;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	t_command		*cmd;
} t_ast_node;

typedef enum	e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_APPEND,
	TOK_HEREDOC,
	TOK_EOF
} t_token_type;

typedef struct	s_token_lst
{
	char			*segment;
	char			*seg_mask;
	t_token_type	type;
	struct s_token_lst	*next;
	struct s_token_lst	*previous;
} t_token_lst;


/* ************************************************************************** */
/* ***************************   functions   ******************************** */
/* ************************************************************************** */



/* **************************    REFACTORED    ****************************** */

//	main minishell
int		minishell_routine(t_env_vars env_vars);
char	*get_input_line(void);



//	Lexer
t_token_lst	*lexer(char **input);
void	push_char(t_token_lst *token, char c, char mask, t_lexer_state *state);

// lexer_2.c
void	state_machine_tokenizer(t_lexer_state *st, t_token_lst *tok, char c);

//	lexer_utils.c
void			tok_lst_add_back(t_token_lst **lst, t_token_lst *new);
t_token_lst		*alloc_null_tok(void);
void			free_tok_lst(t_token_lst *lst);
void			free_token(t_token_lst *token);
t_token_type	get_token_type(t_token_lst *token);




void		skip_spaces(char **input);
int			is_operator(char c);


#endif

/* ************************    NOT REFACTORED    **************************** */

/*
void		skip_spaces(char **input);
int			is_operator(char c);
int			is_redirection(t_token_type type);
int			is_double_operator(char *input);
void		create_word_token(t_token **token, char **input);
void		create_operator_token(t_token **tokens, char **input);
t_token		*create_token(char *value, int len, t_token_type type);
void		add_token(t_token **tokens, t_token *new_token);
void		free_tokens(t_token *tokens);
void		print_tokens(t_token *tokens);
void		execute_tree(t_node *node);
void		command_logic(t_node *node);
int			handle_redirections(t_command *cmd);
int			set_heredoc_redir(t_command *cmd, char *heredoc_delim);
void		exec_ext_cmd(t_node *node);
char		*get_cmd_path(t_command cmd);
char		*find_in_path(char *cmd_str, char *path_env);
int			exec_forked_builtin(t_node *node);
int			exec_builtin(t_node *node);
void		init_builtin_table(t_builtin table[N_BUILTINS]);
char		**convert_env_list_to_envp(t_list *env_list);
t_var_lst	*convert_envp_to_env_list(char **envp);
void		free_env_list(t_list *env_list);


int	is_builtin(char *cmd);
int	execute_builtin(t_token *tokens, char ***envp, int *exit_status);
int	ft_echo(t_token *tokens);
int	ft_pwd(void);
int	ft_env(char **envp);
int	ft_cd(t_token *tokens, char ***envp);
int	ft_export(t_token *tokens, char ***envp);
int	ft_unset(t_token *tokens, char ***envp);
int	ft_exit(t_token *tokens, int *exit_status);

int		is_numeric(char *str);
int		unset_env_var(char *name, char ***envp);
int		set_env_var(char *name, char *value, char ***envp);
char	**ft_realloc(char **src, int old_size, int new_size);

char	*expand_tilde(char *path, char **envp);
char	*get_env_var(char *name, char **envp);

char	**copy_envp(char **src);
void	free_envp(char **envp);

int	adapter_echo(char **args, char **envp);
int	adapter_cd(char **args, char **envp);
int	adapter_pwd(char **args, char **envp);
int	adapter_env(char **args, char **envp);
int	adapter_export(char **args, char **envp);
int	adapter_unset(char **args, char **envp);
int	adapter_exit(char **args, char **envp);
t_token	*create_token_from_args(char **args);

void	free_ast(t_node *ast);

t_node	*create_ast_from_tokens(t_token *tokens);
t_token	*create_token_from_args(char **args); 
*/
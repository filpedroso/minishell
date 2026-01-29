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

typedef enum e_status
{
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
	t_word				target;
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
	t_var_lst	*persistent_envs_ptr;
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

typedef struct	s_word
{
	char	*token_word_ptr;
	char	*context_mask_ptr;
} t_word;

typedef struct s_file_lst
{
	char				*path;
	struct s_file_lst	*next;
} t_file_lst;

typedef struct	s_command
{
	t_cmd_type		type;
	t_word			*words;
	int				words_count;
	bool			is_pipeline;
	t_env_vars		env_vars;
	t_redirection	*redirections;
	int				redirections_count;
	t_file_lst		*temp_files_list;
} t_command;

typedef struct	s_ast_node
{
	t_node_type			type;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	t_command			*cmd;
} t_ast_node;

typedef enum e_parse_status
{
	PARSE_OK,
	PARSE_ERROR,
	PARSE_FATAL
} t_parse_status;

typedef struct	s_ast
{
	t_ast_node		*ast_root;
	t_parse_status	parse_status;
} t_ast;

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
	char				*segment;
	char				*seg_mask;
	t_token_type		type;
	struct s_token_lst	*next;
	struct s_token_lst	*previous;
} t_token_lst;


/* ************************************************************************** */
/* ***************************   functions   ******************************** */
/* ************************************************************************** */

//	main minishell
int		minishell_routine(t_env_vars env_vars);
char	*get_input_line(void);

//	lexer
t_token_lst	*lexer(char **input);
void		push_char(t_token_lst *token, char c, char mask, t_lexer_state *state);
void		state_machine_tokenizer(t_lexer_state *st, t_token_lst *tok, char c);

//	lexer_utils
void			tok_lst_add_back(t_token_lst **lst, t_token_lst *new);
t_token_lst		*alloc_null_tok(void);
void			free_tok_lst(t_token_lst *lst);
void			free_token(t_token_lst *token);
t_token_type	get_token_type(t_token_lst *token);

// parser
t_ast		make_ast(t_token_lst *tok_lst, t_env_vars env_vars);
t_ast_node	*new_command_node(t_token_lst *start, t_token_lst *end, t_parse_status *status);
t_token_lst *parse_word(t_command *cmd, t_token_lst *token_node, t_parse_status *status);
t_token_lst	*parse_redirection(t_command *cmd, t_token_lst *token_node, t_parse_status *status);

// parser_tools
t_token_lst	*get_first_pipe(t_token_lst *start, t_token_lst *end);
t_token_lst	*tok_lstlast(t_token_lst *lst);
bool		is_tok_redirection(t_token_type tok_type);
void 		destroy_ast(t_ast_node *node);
void		destroy_cmd_node(t_ast_node *cmd_node);

// minishell_utils
char		*get_input_line(void);
t_var_lst	*envp_to_env_list(char **envp);
t_var_lst	*env_node_from_str(char *str);
t_var_lst	*alloc_t_var_list_node(void);
void    	destroy_env_list(t_var_lst *env_list);
void		cleanup_env(t_env_vars env_vars);
void		skip_spaces(char **input);
int			is_operator(char c);

// DEBUG
void	debug_print_ast(t_ast_node *node, int depth);
void	debug_print_ast_2(FILE *out, const t_ast_node *root);

#endif

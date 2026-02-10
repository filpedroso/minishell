/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:41:05 by lcosta-a          #+#    #+#             */
/*   Updated: 2026/02/10 00:48:00 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/* ************************    include libs    ****************************** */
/* ************************************************************************** */

# include "libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/errno.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>

/* ************************************************************************** */
/* ***************************    macros    ********************************* */
/* ************************************************************************** */

# define CONTEXT_UNQUOTED 'n'
# define CONTEXT_SINGLE 's'
# define CONTEXT_DOUBLE 'd'
# define CONTEXT_OPERATOR 'o'

# define SUCCESS 0
# define ERROR -1
# define READ 0
# define NO_ENCL 6
# define WRITE 1
# define CHILD 0
# define N_BUILTINS 7
# define N_REDIR_CASES 6

/* ************************************************************************** */
/* ***************************    types     ********************************* */
/* ************************************************************************** */

typedef enum e_status
{
	STATUS_OK,
	STATUS_ERR
}							t_status;

typedef enum e_cycle_result
{
	CYCLE_CONTINUE,
	CYCLE_EXIT,
	CYCLE_FATAL
}							t_cycle_result;

typedef enum e_lexer_state
{
	STATE_NORMAL,
	STATE_SING_QUOTE,
	STATE_DOUB_QUOTE,
	STATE_OPERATOR,
	STATE_ERROR,
	STATE_TOK_END,
	STATE_TOK_END_NO_EAT
}							t_lexer_state;

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
}							t_builtin_type;

typedef enum e_redirection_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}							t_redirection_type;

typedef enum e_node_type
{
	NODE_PIPE,
	NODE_CMD
}							t_node_type;

typedef enum e_cmd_type
{
	EXT,
	BUILTIN
}							t_cmd_type;

typedef enum e_parse_status
{
	PARSE_OK,
	PARSE_ERROR,
	PARSE_FATAL
}							t_parse_status;

typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_APPEND,
	TOK_HEREDOC,
	TOK_EOF
}							t_token_type;

typedef struct s_token_lst
{
	char					*segment;
	char					*seg_mask;
	t_token_type			type;
	struct s_token_lst		*next;
	struct s_token_lst		*previous;
}							t_token_lst;

typedef struct s_word
{
	char					*token_word_ptr;
	char					*context_mask_ptr;
}							t_word;

typedef struct s_redirection
{
	t_redirection_type		type;
	t_word					target;
}							t_redirection;

typedef struct s_var_lst
{
	char					*var_name;
	char					*value;
	struct s_var_lst		*next;
}							t_var_lst;

typedef struct s_env_vars
{
	t_var_lst				*persistent_envs_ptr;
	t_var_lst				*inline_envs;
}							t_env_vars;

typedef struct s_str_lst
{
	char					*value;
	struct s_str_lst		*next;
}							t_str_lst;

typedef struct s_command
{
	t_cmd_type				type;
	t_word					*words;
	int						words_count;
	bool					is_pipeline;
	t_env_vars				env_vars;
	t_redirection			*redirections;
	int						redirections_count;
	t_str_lst				*temp_files_list;
}							t_command;

typedef struct s_ast_node
{
	t_node_type				type;
	struct s_ast_node		*left;
	struct s_ast_node		*right;
	t_command				*cmd;
}							t_ast_node;

typedef struct s_ast
{
	t_ast_node				*ast_root;
	t_parse_status			parse_status;
}							t_ast;

/* ************************************************************************** */
/* ***************************   functions   ******************************** */
/* ************************************************************************** */

//	main minishell
int							minishell_routine(t_env_vars env_vars);
char						*get_input_line(void);

//	lexer
t_token_lst					*lexer(char **input);
void						push_char(t_token_lst *token, char c, char mask,
								t_lexer_state *state);
void						state_machine_tokenizer(t_lexer_state *st,
								t_token_lst *tok, char c);

//	lexer_utils
void						tok_lst_add_back(t_token_lst **lst,
								t_token_lst *new);
t_token_lst					*alloc_null_tok(void);
void						free_tok_lst(t_token_lst *lst);
void						free_token(t_token_lst *token);
t_token_type				get_token_type(t_token_lst *token);

// parser
t_ast						make_ast(t_token_lst *tok_lst, t_env_vars env_vars);
t_ast_node					*new_command_node(t_token_lst *start,
								t_token_lst *end, t_parse_status *status);
t_token_lst					*parse_word(t_command *cmd,
								t_token_lst *token_node);
t_token_lst					*parse_redirection(t_command *cmd,
								t_token_lst *token_node,
								t_parse_status *status);

// parser_tools
t_token_lst					*get_first_pipe(t_token_lst *start,
								t_token_lst *end);
t_token_lst					*tok_lstlast(t_token_lst *lst);
bool						is_tok_redirection(t_token_type tok_type);
void						destroy_ast(t_ast_node *node);
void						destroy_cmd_node(t_ast_node *cmd_node);

// minishell_utils
bool						is_builtin(t_word *words, int words_amount);
char						*get_input_line(void);
t_var_lst					*envp_to_env_list(char **envp);
t_var_lst					*env_node_from_str(char *str);
t_var_lst					*alloc_t_var_list_node(void);
void						destroy_env_list(t_var_lst *env_list);
void						cleanup_env(t_env_vars env_vars);
void						skip_spaces(char **input);
int							is_operator(char c);

// executor
void						execute_tree(t_ast_node *node);
void						command_logic(t_ast_node *node);
int							handle_redirections(t_command *cmd);
int							set_heredoc_redir(t_command *cmd,
								char *heredoc_delim);
char						*create_temp_file(char *delim);
void						exec_ext_cmd(t_ast_node *node);
char						*get_cmd_path(t_command *cmd);
char						*find_in_path(const char *cmd_str, char *path_env);

// executor envs
char						**get_current_envs(t_env_vars env_lists);
int							var_lst_size(t_var_lst *lst);
bool						lst_has_var(t_var_lst *lst, const char *var_name);
t_var_lst					*copy_var_list(t_var_lst *src_lst);
bool						append_var_to_list(t_var_lst **lst_ptr,
								t_var_lst *src_node);
t_var_lst					*duplicate_node(t_var_lst *src_node);

// executor expansion
t_str_lst					*expand_all_words(t_command *cmd,
								char **current_envs);
char						**produce_final_argv(t_command *cmd,
								char **current_envs);
char						*expand_word_with_context(t_word word, char **envs);

// Cleanup General
void	free_str_arr(char **arr);


// DEBUG
void						debug_print_ast(t_ast_node *node, int depth);
void						debug_print_ast_2(FILE *out,
								const t_ast_node *root);
void						debug_print_ast_pretty(FILE *out,
								const t_ast_node *root);

#endif

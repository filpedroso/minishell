#include <stdio.h>
#include <stdbool.h>

#include "minishell.h"

/* forward declarations */
typedef struct s_ast_node      t_ast_node;
typedef struct s_command       t_command;
typedef struct s_word          t_word;
typedef struct s_var_lst       t_var_lst;
typedef struct s_redirection   t_redirection;
typedef struct s_file_lst      t_str_lst;

/* ===========================
   ENUM STRING HELPERS
   =========================== */

static const char *node_type_str(int type)
{
	if (type == NODE_PIPE)
		return ("NODE_PIPE");
	if (type == NODE_CMD)
		return ("NODE_CMD");
	return ("UNKNOWN_NODE_TYPE");
}

static const char *cmd_type_str(int type)
{
	if (type == EXT)
		return ("EXT");
	if (type == BUILTIN)
		return ("BUILTIN");
	return ("UNKNOWN_CMD_TYPE");
}

static const char *redir_type_str(int type)
{
	if (type == REDIR_IN)
		return ("REDIR_IN");
	if (type == REDIR_OUT)
		return ("REDIR_OUT");
	if (type == REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == REDIR_HEREDOC)
		return ("REDIR_HEREDOC");
	return ("UNKNOWN_REDIR_TYPE");
}

/* ===========================
   INDENT HELPER
   =========================== */

static void	print_indent(int depth)
{
	int	i = 0;

	while (i < depth)
	{
		printf("    ");
		i++;
	}
}

/* ===========================
   PRINT WORD
   =========================== */

static void	print_word(t_word *w, int depth)
{
	print_indent(depth);
	if (!w)
	{
		printf("WORD: (null)\n");
		return;
	}

	printf("WORD:\n");

	print_indent(depth + 1);
	printf("token_word_ptr    = \"%s\"\n",
		w->token_word_ptr ? w->token_word_ptr : "(null)");

	print_indent(depth + 1);
	printf("context_mask_ptr  = \"%s\"\n",
		w->context_mask_ptr ? w->context_mask_ptr : "(null)");
}

/* ===========================
   PRINT VAR LIST
   =========================== */

static void	print_var_list(t_var_lst *lst, int depth)
{
	int	i = 0;

	if (!lst)
	{
		print_indent(depth);
		printf("(empty)\n");
		return;
	}

	while (lst)
	{
		print_indent(depth);
		printf("[%d] VAR:\n", i);

		print_indent(depth + 1);
		printf("var_name = \"%s\"\n",
			lst->var_name ? lst->var_name : "(null)");

		print_indent(depth + 1);
		printf("value    = \"%s\"\n",
			lst->value ? lst->value : "(null)");

		lst = lst->next;
		i++;
	}
}

/* ===========================
   PRINT REDIRECTIONS
   =========================== */

static void	print_redirections(t_redirection *r, int count, int depth)
{
	int	i = 0;

	if (!r || count <= 0)
	{
		print_indent(depth);
		printf("(none)\n");
		return;
	}

	while (i < count)
	{
		print_indent(depth);
		printf("[%d] REDIRECTION:\n", i);

		print_indent(depth + 1);
		printf("type   = %s\n", redir_type_str(r[i].type));

		print_indent(depth + 1);
		printf("target =\n");
		print_word(&r[i].target, depth + 2);

		i++;
	}
}

/* ===========================
   PRINT TEMP FILE LIST
   =========================== */

static void	print_temp_files(t_str_lst *lst, int depth)
{
	int	i = 0;

	if (!lst)
	{
		print_indent(depth);
		printf("(none)\n");
		return;
	}

	while (lst)
	{
		print_indent(depth);
		printf("[%d] TEMP FILE: \"%s\"\n",
			i,
			lst->path ? lst->path : "(null)");

		lst = lst->next;
		i++;
	}
}

/* ===========================
   PRINT COMMAND
   =========================== */

static void	print_command(t_command *cmd, int depth)
{
	int	i = 0;

	print_indent(depth);
	if (!cmd)
	{
		printf("CMD: (null)\n");
		return;
	}

	printf("CMD:\n");

	print_indent(depth + 1);
	printf("type            = %s\n", cmd_type_str(cmd->type));

	print_indent(depth + 1);
	printf("words_count     = %d\n", cmd->words_count);

	print_indent(depth + 1);
	printf("is_pipeline     = %s\n", cmd->is_pipeline ? "true" : "false");

	/* WORDS */
	print_indent(depth + 1);
	printf("WORDS:\n");

	if (!cmd->words || cmd->words_count == 0)
	{
		print_indent(depth + 2);
		printf("(none)\n");
	}
	else
	{
		while (i < cmd->words_count)
		{
			print_indent(depth + 2);
			printf("[%d]\n", i);
			print_word(&cmd->words[i], depth + 3);
			i++;
		}
	}

	/* ENV VARS */
	print_indent(depth + 1);
	printf("ENV VARS:\n");

	print_indent(depth + 2);
	printf("persistent_envs_ptr:\n");
	print_var_list(cmd->env_vars.persistent_envs_ptr, depth + 3);

	print_indent(depth + 2);
	printf("inline_envs:\n");
	print_var_list(cmd->env_vars.inline_envs, depth + 3);

	/* REDIRECTIONS */
	print_indent(depth + 1);
	printf("redirections_count = %d\n", cmd->redirections_count);

	print_indent(depth + 1);
	printf("REDIRECTIONS:\n");
	print_redirections(cmd->redirections,
		cmd->redirections_count,
		depth + 2);

	/* TEMP FILES */
	print_indent(depth + 1);
	printf("TEMP FILES LIST:\n");
	print_temp_files(cmd->temp_files_list, depth + 2);
}

/* ===========================
   PRINT AST NODE (RECURSIVE)
   =========================== */

void	debug_print_ast(t_ast_node *node, int depth)
{
	if (!node)
	{
		print_indent(depth);
		printf("(null node)\n");
		return;
	}

	print_indent(depth);
	printf("AST NODE:\n");

	print_indent(depth + 1);
	printf("type  = %s\n", node_type_str(node->type));

	print_indent(depth + 1);
	printf("cmd   = %p\n", (void *)node->cmd);

	/* Print command if present */
	if (node->type == NODE_CMD)
		print_command(node->cmd, depth + 2);

	/* Left */
	print_indent(depth + 1);
	printf("LEFT:\n");
	debug_print_ast(node->left, depth + 2);

	/* Right */
	print_indent(depth + 1);
	printf("RIGHT:\n");
	debug_print_ast(node->right, depth + 2);
}

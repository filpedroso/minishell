#include "minishell.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static const char	*redir_symbol(t_redirection_type t)
{
	if (t == REDIR_IN)
		return ("<");
	if (t == REDIR_OUT)
		return (">");
	if (t == REDIR_APPEND)
		return (">>");
	if (t == REDIR_HEREDOC)
		return ("<<");
	return ("?");
}

static const char	*cmd_type_str_short(t_cmd_type t)
{
	if (t == BUILTIN)
		return ("builtin");
	if (t == EXT)
		return ("ext");
	return ("cmd?");
}

static void	fprint_quoted(FILE *out, const char *s)
{
	bool	needs_quotes;

	if (!s)
	{
		fputs("(null)", out);
		return ;
	}
	/* Minimal quoting: only add quotes when there are spaces/tabs. */
	needs_quotes = false;
	for (const char *p = s; *p; ++p)
		if (*p == ' ' || *p == '\t')
		{
			needs_quotes = true;
			break ;
		}
	if (!needs_quotes)
	{
		fputs(s, out);
		return ;
	}
	fputc('"', out);
	for (const char *p = s; *p; ++p)
	{
		if (*p == '"' || *p == '\\')
			fputc('\\', out);
		fputc(*p, out);
	}
	fputc('"', out);
}

static void	fprint_command_compact(FILE *out, const t_cmd *c)
{
	if (!c)
	{
		fputs("CMD (null)", out);
		return ;
	}
	fprintf(out, "CMD(%s)", cmd_type_str_short(c->type));
	/* words */
	fputs(" ", out);
	if (!c->words || c->words_count <= 0)
		fputs("(no-words)", out);
	else
	{
		for (int i = 0; i < c->words_count; i++)
		{
			if (i)
				fputc(' ', out);
			fprint_quoted(out, c->words[i].token_word_ptr);
		}
	}
	/* redirs */
	if (c->redirections && c->redirections_count > 0)
	{
		fputs("  [", out);
		for (int i = 0; i < c->redirections_count; i++)
		{
			if (i)
				fputs(", ", out);
			fputs(redir_symbol(c->redirections[i].type), out);
			fputc(' ', out);
			fprint_quoted(out, c->redirections[i].target.token_word_ptr);
		}
		fputs("]", out);
	}
}

static void	fprint_node_label(FILE *out, const t_ast_node *n)
{
	if (!n)
	{
		fputs("(null)", out);
		return ;
	}
	if (n->type == NODE_PIPE)
		fputs("PIPE |", out);
	else if (n->type == NODE_CMD)
		fprint_command_compact(out, n->cmd);
	else
		fputs("NODE<?>", out);
}

/*
 * Pretty tree printer:
 * - left printed first, then right
 * - draws connectors like:
 *   PIPE |
 *   ├── CMD(ext) echo hi
 *   └── CMD(ext) wc -l
 */
static void	dbg_print_ast_pretty_rec(FILE *out, const t_ast_node *n,
		const char *prefix, bool is_last, int depth)
{
	const int			MAX_DEPTH = 256;
	const t_ast_node	*left = n->left;
	const t_ast_node	*right = n->right;
	char				next_prefix[1024];
	bool				has_left;
	bool				has_right;

	if (depth > MAX_DEPTH)
	{
		fprintf(out, "%s%s** depth limit reached (possible cycle) **\n", prefix,
			is_last ? "└── " : "├── ");
		return ;
	}
	fprintf(out, "%s%s", prefix, is_last ? "└── " : "├── ");
	fprint_node_label(out, n);
	fputc('\n', out);
	if (!n)
		return ;
	/* If you want to hide null children,
		gate these calls with if(left/right). */
	snprintf(next_prefix, sizeof(next_prefix), "%s%s", prefix,
		is_last ? "    " : "│   ");
	/* Determine which children exist to decide connector shape. */
	has_left = (left != NULL);
	has_right = (right != NULL);
	if (has_left && has_right)
	{
		dbg_print_ast_pretty_rec(out, left, next_prefix, false, depth + 1);
		dbg_print_ast_pretty_rec(out, right, next_prefix, true, depth + 1);
	}
	else if (has_left)
		dbg_print_ast_pretty_rec(out, left, next_prefix, true, depth + 1);
	else if (has_right)
		dbg_print_ast_pretty_rec(out, right, next_prefix, true, depth + 1);
	else
		return ;
}

void	debug_print_ast_pretty(FILE *out, const t_ast_node *root)
{
	if (!out)
		out = stderr;
	fputs("AST\n", out);
	dbg_print_ast_pretty_rec(out, root, "", true, 0);
}

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#include "minishell.h"

static void put_indent(FILE *out, int depth)
{
	for (int i = 0; i < depth; i++)
		fputs("  ", out);
}

static const char *node_type_str(t_node_type t)
{
	if (t == NODE_PIPE) return "NODE_PIPE";
	if (t == NODE_CMD)  return "NODE_CMD";
	return "NODE_<UNKNOWN>";
}

static const char *cmd_type_str(t_cmd_type t)
{
	if (t == EXT)     return "EXT";
	if (t == BUILTIN) return "BUILTIN";
	return "CMD_<UNKNOWN>";
}

static const char *redir_type_str(t_redirection_type t)
{
	if (t == REDIR_IN)     return "REDIR_IN";
	if (t == REDIR_OUT)    return "REDIR_OUT";
	if (t == REDIR_APPEND) return "REDIR_APPEND";
	if (t == REDIR_HEREDOC)return "REDIR_HEREDOC";
	return "REDIR_<UNKNOWN>";
}

static void dbg_print_cstr(FILE *out, const char *s)
{
	if (s) fprintf(out, "\"%s\"", s);
	else   fputs("(null)", out);
}

static void dbg_print_word(FILE *out, const t_word *w, int depth, const char *label)
{
	put_indent(out, depth);
	fprintf(out, "%s: { token_word_ptr=", label);
	dbg_print_cstr(out, w ? w->token_word_ptr : NULL);
	fputs(", context_mask_ptr=", out);
	dbg_print_cstr(out, w ? w->context_mask_ptr : NULL);
	fputs(" }\n", out);
}

/* static void dbg_print_var_list(FILE *out, const t_var_lst *v, int depth, const char *label)
{
	put_indent(out, depth);
	fprintf(out, "%s: %p\n", label, (void *)v);

	int i = 0;
	while (v)
	{
		put_indent(out, depth + 1);
		fprintf(out, "[%d] name=", i);
		dbg_print_cstr(out, v->var_name);
		fputs(", value=", out);
		dbg_print_cstr(out, v->value);
		fprintf(out, ", next=%p\n", (void *)v->next);
		v = v->next;
		i++;
	}
} */

/* static void dbg_print_env_vars(FILE *out, const t_env_vars *e, int depth)
{
	put_indent(out, depth);
	fprintf(out, "env_vars: { persistent_envs_ptr=%p, inline_envs=%p }\n",
		(void *)(e ? e->persistent_envs_ptr : NULL),
		(void *)(e ? e->inline_envs : NULL));

	if (!e)
		return;
	dbg_print_var_list(out, e->persistent_envs_ptr, depth + 1, "persistent_envs_ptr");
	dbg_print_var_list(out, e->inline_envs,          depth + 1, "inline_envs");
} */

static void dbg_print_redirections(FILE *out, const t_redirection *r, int count, int depth)
{
	put_indent(out, depth);
	fprintf(out, "redirections: ptr=%p, count=%d\n", (void *)r, count);

	for (int i = 0; i < count; i++)
	{
		put_indent(out, depth + 1);
		fprintf(out, "[%d] { type=%s }\n", i, redir_type_str(r[i].type));
		dbg_print_word(out, &r[i].target, depth + 2, "target");
	}
}

static void dbg_print_file_list(FILE *out, const t_file_lst *f, int depth, const char *label)
{
	put_indent(out, depth);
	fprintf(out, "%s: %p\n", label, (void *)f);

	int i = 0;
	while (f)
	{
		put_indent(out, depth + 1);
		fprintf(out, "[%d] path=", i);
		dbg_print_cstr(out, f->path);
		fprintf(out, ", next=%p\n", (void *)f->next);
		f = f->next;
		i++;
	}
}

static void dbg_print_command(FILE *out, const t_command *c, int depth)
{
	put_indent(out, depth);
	fprintf(out, "cmd: %p\n", (void *)c);
	if (!c)
		return;

	put_indent(out, depth + 1);
	fprintf(out, "type=%s\n", cmd_type_str(c->type));

	put_indent(out, depth + 1);
	fprintf(out, "words: ptr=%p, words_count=%d\n", (void *)c->words, c->words_count);
	for (int i = 0; i < c->words_count; i++)
	{
		char label[64];
		snprintf(label, sizeof(label), "words[%d]", i);
		dbg_print_word(out, &c->words[i], depth + 2, label);
	}

	put_indent(out, depth + 1);
	fprintf(out, "is_pipeline=%s\n", c->is_pipeline ? "true" : "false");

	// dbg_print_env_vars(out, &c->env_vars, depth + 1);

	dbg_print_redirections(out, c->redirections, c->redirections_count, depth + 1);

	dbg_print_file_list(out, c->temp_files_list, depth + 1, "temp_files_list");
}

static void dbg_print_ast_node_rec(FILE *out, const t_ast_node *n, int depth)
{
	const int MAX_DEPTH = 256;
	if (depth > MAX_DEPTH)
	{
		put_indent(out, depth);
		fputs("** depth limit reached (possible cycle) **\n", out);
		return;
	}

	put_indent(out, depth);
	if (!n)
	{
		fputs("(null)\n", out);
		return;
	}

	fprintf(out,
		"ast_node %p { type=%s, left=%p, right=%p, cmd=%p }\n",
		(void *)n,
		node_type_str(n->type),
		(void *)n->left,
		(void *)n->right,
		(void *)n->cmd
	);

	if (n->type == NODE_CMD)
		dbg_print_command(out, n->cmd, depth + 1);

	put_indent(out, depth);
	fputs("left:\n", out);
	dbg_print_ast_node_rec(out, n->left, depth + 1);

	put_indent(out, depth);
	fputs("right:\n", out);
	dbg_print_ast_node_rec(out, n->right, depth + 1);
}

void	debug_print_ast_2(FILE *out, const t_ast_node *root)
{
	if (!out) out = stderr;
	fputs("=== AST DUMP BEGIN ===\n", out);
	dbg_print_ast_node_rec(out, root, 0);
	fputs("=== AST DUMP END ===\n", out);
}

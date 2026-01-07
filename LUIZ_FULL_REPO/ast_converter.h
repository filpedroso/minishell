/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_converter.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosta-a <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 04:31:27 by lcosta-a          #+#    #+#             */
/*   Updated: 2026/01/04 10:48:09 by lcosta-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_CONVERTER_H
# define AST_CONVERTER_H

# include "minishell.h"
# include "executor.h"

t_node	*create_ast_from_tokens(t_token *tokens, t_list *env_list);
t_token	*create_token_from_args(char **args);
void	free_ast(t_node *node);

#endif

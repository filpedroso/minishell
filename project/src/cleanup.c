/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 15:57:40 by fpedroso          #+#    #+#             */
/*   Updated: 2026/03/12 15:57:40 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_cleanup(t_sh *sh)
{
	cycle_cleanup(NULL, sh->tokens, sh->ast.ast_root);
	cleanup_env(sh->env_vars);
}

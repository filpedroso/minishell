/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 16:51:07 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/04 01:06:16 by lcosta-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HANDLE_REDIRECTIONS_H
# define HANDLE_REDIRECTIONS_H

# include "executor.h"
# include "ast_utils.h"

void	handle_redirections(t_node *node);
void	clean_temp_files();
void	restore_stdinout(int stdin_backup, int stdout_backup, int stderr_backup);
#endif

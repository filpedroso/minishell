/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tree.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:56:13 by fpedroso          #+#    #+#             */
/*   Updated: 2026/02/10 00:35:26 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_TREE_H
# define TEST_TREE_H

int	ft_echo(char **args, char **envp);
int	ft_cd(char **args, char **envp);
int	ft_pwd(char **args, char **envp);
int	ft_export(char **args, char **envp);
int	ft_unset(char **args, char **envp);
int	ft_env(char **args, char **envp);
int	ft_exit(char **args, char **envp);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:39:34 by fpedroso          #+#    #+#             */
/*   Updated: 2025/12/16 19:39:34 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.c"

void	cleanup_node(t_node *node)
{
	clean_temp_files(); // checks if there are created files to delete
	clean_allocations();
}

void	clean_temp_files()
{
	//incomplete!
	printf("Code clean_temp_files() cleanup function!\n");
}

void	clean_allocations()
{
	//incomplete!
	printf("Code clean_allocations() cleanup function!\n");
}

void	free_str_arr(char **str_arr)
{
	//incomplete!
	printf("Code free_str_arr() cleanup function!\n");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lst_add_back.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:17:33 by fpedroso          #+#    #+#             */
/*   Updated: 2026/01/19 11:12:55 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tok_lst_add_back(t_token_lst **lst, t_token_lst *new)
{
	t_token_lst	*last;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while(last->next)
	{
		last = last->next;
	}
	last->next = new;
}

/* int	main(void)
{
	t_list	*lst;
	t_list	*new;
	t_list	*current;
	t_list	*next;
	int		*first_val;
	int		*j;
	int		i;
	int		first_value;
	int		last_value;

	i = 0;
	first_val = malloc(sizeof(int));
	*first_val = i;
	lst = ft_lstnew(first_val);
	while (i < 10)
	{
		j = malloc(sizeof(int));
		*j = i * 2;
		new = ft_lstnew(j);
		ft_lstadd_back(&lst, new);
		i++;
	}
	first_value = *(int *)(lst->content);
	last_value = *(int *)(ft_lstlast(lst))->content;
	printf("first: %i, last: %i\n", first_value, last_value);
	current = lst;
	while (current)
	{
		printf("%i\n", *(int *)current->content);
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
	return (0);
} */

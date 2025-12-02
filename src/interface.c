/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 20:27:56 by fpedroso          #+#    #+#             */
/*   Updated: 2025/11/11 20:27:56 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

#define PWD "pwd"
#define EXIT "exit"

/*
* echo with option -n
* cd with only a relative or absolute path
* pwd with no options
* export with no options
* unset with no options
* env with no options or arguments
* exit with no options
*/

void	exec_command(char *cmd);
int	ft_strcmp(char *sa, char *sb);

int	main(void)
{
    char	*rl;

	puts("minishell\n");
	while (true)
	{
    	rl = readline("$ ");
		if (!rl)
			return (1);
		add_history(rl);
		exec_command(rl);
	}

    printf("%s\n", rl);
    return (0);
}

void	exec_command(char *cmd)
{
	if (ft_strcmp(cmd, PWD) == 0)
		printf("%s\n", getcwd(NULL, 0));
	else if (ft_strcmp(cmd, EXIT) == 0)
		exit(0);
}

int	ft_strcmp(char *sa, char *sb)
{
	while (*sa && *sb)
	{
		if (*sa != *sb)
			return (1);
		sa++;
		sb++;
	}
	return (0);
}

/* char	*commands[] = {
	"echo",
	"cd",
	"pwd",
	"export",
	"unset",
	"env",
	"exit",
	NULL
}; */
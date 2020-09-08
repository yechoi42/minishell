/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwon <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:22:44 by jwon              #+#    #+#             */
/*   Updated: 2020/09/08 21:06:47 by jwon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_exit_status()
{
	ft_putstr_fd(ft_itoa(g_exit_value), 1);
	ft_putendl_fd(": command not found", 1);
}

void	cmd_exit(char **argv, t_list *envs)
{
	int	argc;

	argc = 0;
	while (argv[argc] != NULL)
		argc++;
	if (argc == 1)
		exit(EXIT_SUCCESS);
	else if (argc == 2 && all_digit(argv[1]))
	{
		exit(ft_atoi(argv[1]));
	}
	else if (argc > 2 && all_digit(argv[1]))
		ft_putendl_fd("bash: exit: too many arguments", 1);
	else
	{
		ft_putstr_fd("bash: exit: numeric argument required", 1);
		exit(2);
	}
}
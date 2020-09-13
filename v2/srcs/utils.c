/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwon <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/13 16:46:10 by jwon              #+#    #+#             */
/*   Updated: 2020/09/13 17:24:36 by jwon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_puterror_fd(char *s1, char *s2, int fd)
{
	ft_putstr_fd(s1, fd);
	ft_putendl_fd(s2, fd);
	return (127);
}

char	*substr_and_trim(char *command, int start, int num, char *charset)
{
	char	*temp;
	char	*temp2;

	temp = ft_substr(command, start, num);
	temp2 = ft_strtrim(temp, charset);
	free(temp);
	return (temp2);
}

void	free_double_arr(char **arr)
{
	int		idx;

	idx = -1;
	while (arr[++idx])
		free(arr[idx]);
	free(arr);
}

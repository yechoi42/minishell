/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwon <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:24:06 by jwon              #+#    #+#             */
/*   Updated: 2020/09/08 15:46:20 by jwon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_double_arr(char **arr) // 2차원 배열 출력 함수
{
	int		idx;
	idx = 0;
	while (arr[idx])
		ft_putendl_fd(arr[idx++], 1);
}

void	sort_double_arr(char **arr) // 2차원 배열 내림차순 정렬 함수
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (arr[i + 1])
	{
		if (ft_strncmp(arr[i], arr[i + 1], 100) > 0)
		{
			temp = arr[i];
			arr[i] = arr[i + 1];
			arr[i + 1] = temp;
			i = -1;
		}
		i++;
	}
}
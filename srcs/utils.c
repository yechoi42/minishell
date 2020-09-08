/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwon <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:19:40 by jwon              #+#    #+#             */
/*   Updated: 2020/09/08 20:45:40 by jwon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_valid_env(char *arg) // export cmd 인자 유효성 체크
{
	if (ft_strchr(arg, '='))
		return (1);
	return (0);
}

int		is_exist_key(char *key, t_list *envs) // 이미 존재하는 key인지 확인
{
	if (ft_strncmp(((t_env *)envs->content)->key, key, ft_strlen(key)) == 0)
		return (1);
	return (0);
}

int		all_digit(char *str)
{
	int	idx;

	idx = 0;
	while (str[idx])
	{
		if (!ft_isdigit(str[idx]))
			return (0);
		idx++;
	}
	return (1);
}

void	free_double_arr(char **arr)
{
	int		idx;

	idx = -1;
	while (arr[++idx])
		free(arr[idx]);
	free(arr);
}
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

int		is_valid_env(char *arg) // export cmd ���� ��ȿ�� üũ
{
	if (ft_strchr(arg, '='))
		return (1);
	return (0);
}

int		is_exist_key(char *key, t_list *envs) // �̹� �����ϴ� key���� Ȯ��
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

char	*substr_and_trim(char *command, int start, int num, char *charset)
{
	char	*temp;
	char	*temp2;

	temp = ft_substr(command, start, num);
	temp2 = ft_strtrim(temp, charset);
	free(temp);
	return (temp2);
}
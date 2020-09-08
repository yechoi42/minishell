/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwon <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:25:19 by jwon              #+#    #+#             */
/*   Updated: 2020/09/08 15:52:53 by jwon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_value(char *key, t_list *envs) // key에 맞는 value를 반환하는 함수
{
	while (envs)
	{
		if (is_exist_key(key, envs)) // key가 존재한다면?
			return (((t_env *)envs->content)->value);
		envs = envs->next;
	}
	return (NULL);
}

void 	print_envs(t_list *envs) // 환경변수 출력 함수
{
	if (envs == NULL)
		return ;
	ft_putstr_fd(((t_env *)envs->content)->key, 1);
	ft_putchar_fd('=', 1);
	ft_putendl_fd(((t_env *)envs->content)->value, 1);
	print_envs(envs->next);
}

t_list	*get_envs(int argc, char **argv, char **envp)
{
	int 	pos;
	t_env	*env;
	t_list	*envs;

	envs = 0;
	if (!argc && argv)
		exit(1);
	while (*envp)
	{
		env = (t_env *)malloc(sizeof(t_env));
		pos = ft_strchr(*envp, '=') - *envp;
		env->key = ft_substr(*envp, 0, pos);
		env->value = ft_substr(*envp, pos + 1, ft_strlen(*envp) - pos - 1);
		ft_lstadd_back(&envs, ft_lstnew(env));
		envp++;
	}
	return (envs);
}
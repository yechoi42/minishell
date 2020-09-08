/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwon <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:21:56 by jwon              #+#    #+#             */
/*   Updated: 2020/09/08 15:26:52 by jwon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		delete_key(char *argv, t_list *envs) // key와 그에 맞는 value 삭제
{
	t_list *curr;
	t_list *next;

	curr = envs;
	while (curr->next)
	{
		if (is_exist_key(argv, curr->next)) // key가 존재한다면?
		{
			next = curr->next;
			curr->next = next->next;
			free(((t_env *)next->content)->key);
			free(((t_env *)next->content)->value);
			free(next->content);
			free(next);
			return ;
		}
		curr = curr->next;
	}
}

void			cmd_unset(char **argv, t_list *envs) // unset 명령어 실행
{
	argv++;
	while (*argv)
	{
		delete_key(*argv, envs);
		argv++;
	}
}
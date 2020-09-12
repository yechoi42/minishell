/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwon <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:25:19 by jwon              #+#    #+#             */
/*   Updated: 2020/09/12 14:16:04 by jwon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void 	print_envs(t_list *envs) // ȯ�溯�� ���? �Լ�
{
	if (envs == NULL)
		return ;
	ft_putstr_fd(((t_env *)envs->content)->key, 1);
	ft_putchar_fd('=', 1);
	ft_putendl_fd(((t_env *)envs->content)->value, 1);
	print_envs(envs->next);
}

char			*find_value(char *key, t_list *envs) // key�� �´� value�� ��ȯ�ϴ� �Լ�
{
	while (envs)
	{
		if (is_exist_key(key, envs)) // key�� �����Ѵٸ�?
			return (((t_env *)envs->content)->value);
		envs = envs->next;
	}
	return (NULL);
}

t_list			*get_envs(int argc, char **argv, char **envp)
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

static char		*make_tmp_envs(char *tmp_envs, char *argv)
{
	char	*tmp;

	if (tmp_envs == NULL)
		tmp_envs = ft_strjoin(argv, "\n");
	else
	{
		tmp = ft_strjoin(argv, "\n");
		tmp_envs = ft_strjoin(tmp_envs, tmp);
		free(tmp);
	}
	return (tmp_envs);
}

void			cmd_env(char **argv, t_list *envs)
{
	char	*tmp_envs;

	tmp_envs = 0;
	argv++;
	while (*argv)
	{
		if (!is_valid_env(*argv))
		{
			ft_putstr_fd("env: ", 1);
			ft_putstr_fd(*argv, 1);
			ft_putendl_fd(": No such file or directory", 1);
			return ;
		}
		tmp_envs = make_tmp_envs(tmp_envs, *argv);
		argv++;
	}
	print_envs(envs);
	if (tmp_envs != NULL)
	{
		ft_putstr_fd(tmp_envs, 1);
		free(tmp_envs);
	}
}
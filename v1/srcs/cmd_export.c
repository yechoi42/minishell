/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwon <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:24:34 by jwon              #+#    #+#             */
/*   Updated: 2020/09/08 15:26:37 by jwon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		update_value(t_env *env, t_list **envs) // value ������Ʈ
{
	// free(((t_env *)envs->content)->value);
	((t_env *)(*envs)->content)->value = env->value;
	// free(env->key);
	// free(env);
}

static void		modify_env_for_export(char **arr) // export ��� ���� �տ� declare -x ���̴� �Լ�
{
	int		idx;

	idx = 0;
	while (arr[idx])
	{
		arr[idx] = ft_strjoin("declare -x ", arr[idx]);
		idx++;
	}
}

static char		**convert_lst_to_arr(t_list *lst) // ���Ḯ��Ʈ�� �迭�� ��ȯ�ϴ� �Լ�
{
	int		idx;
	int		count;
	char	**arr;
	char	*keytmp;
	char	*valuetmp;

	count = ft_lstsize(lst);
	if (!(arr = (char **)malloc(sizeof(char *) * (count + 1))))
		return (NULL);
	idx = 0;
	while (lst)
	{
		keytmp = ft_strjoin(((t_env *)lst->content)->key, "=\"");
		valuetmp = ft_strjoin((((t_env *)lst->content)->value), "\"");
		arr[idx] = ft_strjoin(keytmp, valuetmp);
		free(keytmp);
		free(valuetmp);
		lst = lst->next;
		idx++;
	}
	arr[idx] = NULL;
	return (arr);
}

void			add_env_or_modify_value(char **argv, t_list **envs) // ȯ�溯���� �߰��ϰų� value�� ������Ʈ�ϴ� �Լ�
{
	t_env	*env;
	t_list	*curr;
	int		pos;
	if (!(env = (t_env *)malloc(sizeof(t_env))))
		return ;
	pos = ft_strchr(*argv, '=') - *argv;
	env->key = ft_substr(*argv, 0, pos);
	env->value = ft_substr(*argv, pos + 1, ft_strlen(*argv) - pos - 1);
	curr = *envs;
	while (curr)
	{
		if (is_exist_key(env->key, curr)) // key�� �����Ѵٸ�?
		{
			update_value(env, &curr);
			return ;
		}
		curr = curr->next;
	}
	if (curr == NULL)
		ft_lstadd_back(envs, ft_lstnew(env)); // �� �ڿ� �߰�
}

void			cmd_export(char **argv, t_list *envs) // export ���ɾ� ����
{
	char	**temp;

	if (argv[1] == NULL) // ���ڰ� ���� ���
	{
		temp = convert_lst_to_arr(envs); // ���Ḯ��Ʈ �迭�� ��ȯ�ؼ� �ӽ� ����
		sort_double_arr(temp); // key �������� �������� ����
		modify_env_for_export(temp); // �տ� declare -x ���̱�
		print_double_arr(temp); // ���
		free_double_arr(temp); // �޸� ����
		return ; // �Լ� ����
	}
	argv++;
	while (*argv)
	{
		if (!is_valid_env(*argv))
		{
			ft_putstr_fd("export: `", 1);
			ft_putstr_fd(*argv, 1);
			ft_putendl_fd("': not a valid identifier", 1);
			return ;
		}
		add_env_or_modify_value(argv, &envs);
		argv++;
	}
}
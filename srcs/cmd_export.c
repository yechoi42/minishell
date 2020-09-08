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

static void		update_value(t_env *env, t_list **envs) // value 업데이트
{
	// free(((t_env *)envs->content)->value);
	((t_env *)(*envs)->content)->value = env->value;
	// free(env->key);
	// free(env);
}

static void		add_env_or_modify_value(char **argv, t_list **envs) // 환경변수를 추가하거나 value를 업데이트하는 함수
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
		if (is_exist_key(env->key, curr)) // key가 존재한다면?
		{
			update_value(env, &curr);
			return ;
		}
		curr = curr->next;
	}
	if (curr == NULL)
		ft_lstadd_back(envs, ft_lstnew(env)); // 맨 뒤에 추가
}

static void		modify_env_for_export(char **arr) // export 출력 위해 앞에 declare -x 붙이는 함수
{
	int		idx;

	idx = 0;
	while (arr[idx])
	{
		arr[idx] = ft_strjoin("declare -x ", arr[idx]);
		idx++;
	}
}

static char		**convert_lst_to_arr(t_list *lst) // 연결리스트를 배열로 변환하는 함수
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

void			cmd_export(char **argv, t_list *envs) // export 명령어 실행
{
	char	**temp;

	if (argv[1] == NULL) // 인자가 없는 경우
	{
		temp = convert_lst_to_arr(envs); // 연결리스트 배열로 변환해서 임시 저장
		sort_double_arr(temp); // key 기준으로 내림차순 정렬
		modify_env_for_export(temp); // 앞에 declare -x 붙이기
		print_double_arr(temp); // 출력
		free_double_arr(temp); // 메모리 해제
		return ; // 함수 종료
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
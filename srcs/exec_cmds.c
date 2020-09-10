/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwon <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:44:33 by jwon              #+#    #+#             */
/*   Updated: 2020/09/08 21:07:17 by jwon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		exec_builtin(char **argv, t_list *envs)
{
	if (has_quote(argv))
		argv = modify_argv(argv, envs);
	if (!ft_strncmp(argv[0], "pwd", ft_strlen(argv[0])))
		cmd_pwd(argv, envs);
	else if (!ft_strncmp(argv[0], "cd", ft_strlen(argv[0])))
		cmd_cd(argv, envs);
	else if (!ft_strncmp(argv[0], "env", ft_strlen(argv[0])))
		cmd_env(argv, envs);
	else if (!ft_strncmp(argv[0], "export", ft_strlen(argv[0])))
		cmd_export(argv, envs);
	else if (!ft_strncmp(argv[0], "unset", ft_strlen(argv[0])))
		cmd_unset(argv, envs);
	else if (!ft_strncmp(argv[0], "exit", ft_strlen(argv[0])))
		cmd_exit(argv, envs);
	else if (!ft_strncmp(argv[0], "$?", ft_strlen(argv[0])))
		print_exit_status();
	else
		cmd_others(argv, envs);
	// �ش����� �ʴ� ���ɾ��� ��쿡�� ��� ���� �����غ��� �� ��. ���ϰ��� �Բ�.
}

void			exec_cmds(t_list *cmds, t_list *envs)
{
	char	**argv;

	while (cmds != NULL)
	{
		if (!(argv = ft_split(((t_cmd *)cmds->content)->command, ' ')))
			exit(1);
		// if (((t_cmd *)cmds->content)->pipe)
		// 	exec_pipe();
		if (((t_cmd *)cmds->content)->redir)
			exec_redir(((t_cmd *)cmds->content), envs);
		else
			exec_builtin(argv, envs);
		free_double_arr(argv);
		cmds = cmds->next;
	}
}
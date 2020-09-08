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

static void		exec_builtin(t_cmd *cmd, t_list *envs)
{
	char **argv;

	if (!(argv = ft_split(cmd->command, ' ')))
		exit(1);
	if (has_quote(argv))
		argv = modify_argv(argv, envs);
	if (!ft_strncmp(argv[0], "pwd", ft_strlen(argv[0])))
		cmd_pwd(argv, envs);
	else if (!ft_strncmp(argv[0], "cd", ft_strlen(argv[0])))
		cmd_cd(argv, envs);
	else if (!ft_strncmp(argv[0], "env", ft_strlen(argv[0])))
		print_envs(envs);
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
	// 해당하지 않는 명령어일 경우에는 어떻게 할지 생각해봐야 할 듯. 리턴값도 함께.
}

void			exec_cmds(t_list *cmds, t_list *envs)
{
	while (cmds != NULL)
	{
		// if (((t_cmd *)cmds->content)->redir)
		// 	exec_redir();
		// if (((t_cmd *)cmds->content)->pipe)
		// 	exec_pipe();
		exec_builtin(((t_cmd *)cmds->content), envs);
		cmds = cmds->next;
	}
}
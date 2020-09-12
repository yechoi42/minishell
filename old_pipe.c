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

void	init_pipe(char *command, t_pipe *p)
{
	int num;

	num = 0;
	while (*command)
	{
		if (ft_strchr("|", *command))
			num++;
		command++;
	}
	p->argc = num + 1;
	p->argv = (char **)malloc(sizeof(char*) * (p->argc + 1));
	p->cmds = (char ***)malloc(sizeof(char **) * (p->argc + 1));
}

void	edit_p_cmds(t_pipe *p, int i, t_list *envs)
{
	p->cmds[i] = ft_split(p->argv[i], ' ');
	if (has_quote(p->cmds[i]))
		p->cmds[i] = modify_argv(p->cmds[i], envs);
}

	// if (!(path = find_path(p->cmds[i][0], envs)))
	// {
	// 	ft_putstr_fd(p->cmds[i][0], 1);
	// 	ft_putendl_fd(": command not found", 1);
	// 	return ; 
	// }

int		parse_pipe(char *command, t_pipe *p, t_list *envs)
{
	int		i;
	int		j;
	int		start;
	char 	*temp;

	i = -1;
	j = 0;
	start = 0;
	while (command[++i])
	{
		if (ft_strchr("|", command[i]))
		{
			p->argv[j] = substr_and_trim(command, start, i - start, " ");
			start = i + 1;
			j++;
		}
	}
	p->argv[j] = substr_and_trim(command, start, i - start, " ");
	if (!*(p->argv[j]))
		return (0);
	p->argv[++j] = NULL;
	i = -1;
	while (p->argv[++i])
		edit_p_cmds(p, i, envs);
	return (1);
}


int		has_redir(char *str)
{
	int i; 

	i = -1;
	while (str[++i])
	{
		if (ft_strchr("><", str[i]))
			return (1);
	}
	return (0);
}

void	cmd_pipe(t_pipe *p, int i, char *path, t_list *envs)
{
	int		status;
	int 	fd[2];
	pid_t	child;

	child = fork();
	if (child == 0)
	{
		pipe(fd);
		if (fd[1] != 1)
			close(fd[1]);
		if (fd[0] != 0)
		{
			dup2(fd[0], 0);
			close(fd[0]);
		}
		printf("CHILD i is ...%d\n", i);
		if (!exec_builtin(p->cmds[i], envs))
		{
			if (execve(path, p->cmds[i], g_envp) == -1)
			{
				ft_putstr_fd(p->cmds[i][0], 1);
                ft_putendl_fd(": command not found", 1);
				exit(EXIT_SUCCESS);				
			}
		}
		exit(EXIT_SUCCESS);	
	}
	else
	{
		close(fd[0]);
		if (fd[1] != 1)
		{
			dup2(fd[1], 0);
			close(fd[1]);
		}
		waitpid(child, &status, 0);
		g_exit_value = status / 256;
		if (!exec_builtin(p->cmds[++i], envs))
		{
			if (execve(path, p->cmds[i], g_envp) == -1)
			{
				ft_putstr_fd(p->cmds[i][0], 1);
                ft_putendl_fd(": command not found", 1);
				exit(EXIT_SUCCESS);				
			}
		}
		// fd2[0] = 0;
		// fd2[1] = 1;
		// if (++i < p->argc && p->argv[i])
		// {
		// 	printf("PARENT i is... %d\n", i);
		// 	pipe(fd2);
		// 	branch_pipe(p, fd2, i, envs);
		// }
	}
}

void	branch_pipe(t_pipe *p, int *fd, int i, t_list *envs)
{
	char 	*path;

	if (has_redir(p->argv[i]))
	{
		//cmd_redir_pipe();
	}
	else
	{
		p->cmds[i] = ft_split(p->argv[i], ' ');
		if (has_quote(p->cmds[i]))
			p->cmds[i] = modify_argv(p->cmds[i], envs);
		if (!(path = find_path(p->cmds[i][0], envs)))
		{
			ft_putstr_fd(p->cmds[i][0], 1);
			ft_putendl_fd(": command not found", 1);
			return ; 
		}
		cmd_pipe(p, fd, i, path, envs);
		free(path);
	}
}


void	exec_pipe(t_cmd *cmd, t_list *envs)
{
	int		i;
	int		ret;
	t_pipe	p;

	init_pipe(cmd->command, &p);
	if ((ret = parse_pipe(cmd->command, &p, envs)) <= 0)
		return;
	i = 0;
	cmd_pipe(&p, i, envs);
}


int		exec_builtin(char **argv, t_list *envs)
{
	if (has_quote(argv))
		argv = modify_argv(argv, envs);
	if (!ft_strncmp(argv[0], "cd", ft_strlen(argv[0])))
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
		return (0);
	return (1);
}

void			exec_cmds(t_list *cmds, t_list *envs)
{
	char	**argv;

	while (cmds != NULL)
	{
		if (!(argv = ft_split(((t_cmd *)cmds->content)->command, ' ')))
			exit(1);
		// if (((t_cmd *)cmds->content)->pipe)
		//  	exec_pipe(((t_cmd *)cmds->content), envs);
		else if (((t_cmd *)cmds->content)->redir)
			exec_redir(((t_cmd *)cmds->content), envs);
		else if (!exec_builtin(argv, envs))
			exec_others(argv, envs);
		free_double_arr(argv);
		cmds = cmds->next;
	}
}

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

typedef struct s_redir 
{
	int		argc;
	char	**argv;
	char	**cmds;
	char	*types;
	char	*file_name;
} t_redir;

#define	INIT	-1
#define	REDIR	62
#define	DREDIR	6
#define	BREDIR	60

void	init_redir(char *command, t_redir *r)
{
	int num;

	num = 0;
	while (*command)
	{
		if (ft_strchr("><", *command))
			num++;
		command++;
	}
	r->argc = num + 1;
	r->argv = (char **)malloc(sizeof(char *) * (r->argc + 1));
	r->types = (char *)malloc(sizeof(char) * (r->argc + 1));
	r->cmds = NULL;
	r->file_name = 0;
}

int		is_single_redir(char *command, int i)
{
	int ret;

	if (command[i] == '>' && command[i - 1] != '>' && command[i + 1] != '>')
		return (1);
	return (0);
}

int		find_redir_type(char *command, int i)
{
	if (command[i] == '>' && command[i + 1] == '>')
		return (DREDIR);
	else if (is_single_redir(command, i)) 
		return (REDIR);
	return (BREDIR);
}

int		has_syntax_error(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (!ft_strchr(" \n", str[i]))
			return (0);
		i++;
	}
	return (1);
}

int		parse_redir(char *command, t_redir *r)
{
	int		i;
	int		j;
	int		start;
	char	*temp;

	i = -1;
	j = 0;
	start = 0;
	while (command[++i])
	{
		if (ft_strchr("><", command[i]))
		{
			if (j > 0 && ((r->types[0] == BREDIR && command[i] != '<') || 
				(r->types[0] != BREDIR && command[i] == '<')))
					return (0);
			r->argv[j] = substr_and_trim(command, start, i - start, " ");			
			if (j == 0)
				r->cmds = ft_split(r->argv[j], ' ');
			r->types[j] = find_redir_type(command, i);
			r->types[j++] == DREDIR ? i++ : 0;
			start = i + 1;
		}
	}
	if (command[i] == '\0')
	{
		r->argv[j] = substr_and_trim(command, start, i - start, " ");
		if (has_syntax_error(r->argv[j]))
			return (-1);
		r->types[j++] = 0;
		r->argv[j] = 0;
	}	
	return (1);
}		
// 0, -1 리턴값에서 프리 

// int		get_fd(t_redir r)
// {
// 	int fd;
// 	int i;

// 	i = 0;
// 	fd = 0;
// 	while (r.argv[++i])
// 	{
// 		if (i > 0)
// 			close(fd);
// 		if (r.types[i] == REDIR)
// 			fd = open(r.argv[i + 1], O_WRONLY | O_CREAT, 0744);
// 		else if (r.types[i] == DREDIR)
// 			fd = open(r.argv[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0744);
// 		else
// 			fd = open(r.argv[i + 1], O_RDWR | O_CREAT, 0644);			
// 	}
// 	return (fd);
// }

void	cmd_redir(t_redir *r, t_list *envs)
{
	int		i;
	int		fd;
	pid_t	child;
	char	*path;

	i = 0;
	child = fork();
	if (child == 0)
	{
		fd = 0;
		while (r->argv[++i])
		{
			if (i > 0)
				close(fd);
			if (r->types[i - 1] == REDIR)

				fd = open(r->argv[i], O_WRONLY | O_CREAT | O_TRUNC, 0744);
			else if (r->types[i - 1] == DREDIR)
				fd = open(r->argv[i], O_WRONLY | O_CREAT | O_APPEND, 0744);
			else
				fd = open(r->argv[i], O_RDWR | O_CREAT, 0644);
		}
		path = find_path(r->argv[0], envs);
		dup2(fd, (r->types[i - 2] == BREDIR ? STDIN_FILENO : STDOUT_FILENO));
		close(fd);
		if (execve(path, r->cmds, g_envp) == -1)
		{
			ft_putstr_fd(r->cmds[0], 1);
			ft_putendl_fd(": command not found", 1);
			exit(EXIT_SUCCESS);
		}
		free(path);
	}
	else
	{
		wait(0);
	}
}

void	exec_redir(t_cmd *cmd, t_list *envs)
{
	int		ret;
	t_redir r;

	init_redir(cmd->command, &r);
	if ((ret = parse_redir(cmd->command, &r)) <= 0)
	{
		if (ret < 0)
			ft_putendl_fd("syntax error near unexpected token `newline'", 1);
		return;
	}
	cmd_redir(&r, envs);
}


void		exec_builtin(char **argv, t_list *envs)
{
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
	// �ش����� �ʴ� ���ɾ��� ��쿡�� ��� ���� �����غ��� �� ��. ���ϰ��� �Բ�.
}

void			exec_cmds(t_list *cmds, t_list *envs)
{
	char	**argv;

	if (!(argv = ft_split(((t_cmd *)cmds->content)->command, ' ')))
		exit(1);
	while (cmds != NULL)
	{
		if (((t_cmd *)cmds->content)->redir)
			exec_redir(((t_cmd *)cmds->content), envs);
		// if (((t_cmd *)cmds->content)->pipe)
		// 	exec_pipe();
		else
			exec_builtin(argv, envs);
		cmds = cmds->next;
	}
}
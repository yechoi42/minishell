/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwon <jwon@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/13 17:42:37 by yechoi            #+#    #+#             */
/*   Updated: 2020/09/14 15:32:15 by jwon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Prints the exit status of the last command
 * 
 * Displays the last exit value followed by ': command not found'
 * 
 * @return Always returns 1
 */
static int		print_exit_status(void)
{
	ft_putstr_fd(ft_itoa(g_exit_value), 1);
	ft_putendl_fd(": command not found", 1);
	return (1);
}

/**
 * @brief Handles the $? special variable
 * 
 * Checks if the command is $? and prints the exit status if it is.
 * 
 * @param line The command line to check
 * @return 1 if the command is $?, 0 otherwise
 */
static int		exec_dollar(char *line)
{
	if (line[0] == '$' && line[1] == '?')
	{
		print_exit_status();
		return (1);
	}
	return (0);
}

/**
 * @brief Executes built-in commands
 * 
 * Parses the command line and executes the appropriate built-in command
 * if the command matches one of the built-ins (cd, echo, pwd, env,
 * export, unset, exit).
 * 
 * @param line The command line to execute
 * @param envs List of environment variables
 * @return 1 if a built-in command was executed, 0 otherwise
 */
int				exec_builtin(char *line, t_list *envs)
{
	char **argv;

	argv = get_argv(line, envs);
	if (!ft_strncmp(argv[0], "cd", ft_strlen(argv[0])))
		cmd_cd(argv, envs);
	else if (!ft_strncmp(argv[0], "echo", ft_strlen(argv[0])))
		cmd_echo(argv);
	else if (!ft_strncmp(argv[0], "pwd", ft_strlen(argv[0])))
		cmd_pwd();
	else if (!ft_strncmp(argv[0], "env", ft_strlen(argv[0])))
		cmd_env(argv, envs);
	else if (!ft_strncmp(argv[0], "export", ft_strlen(argv[0])))
		cmd_export(argv, envs);
	else if (!ft_strncmp(argv[0], "unset", ft_strlen(argv[0])))
		cmd_unset(argv, envs);
	else if (!ft_strncmp(argv[0], "exit", ft_strlen(argv[0])))
		cmd_exit(argv);
	else
	{
		free_double_arr(argv);
		return (0);
	}
	free_double_arr(argv);
	g_exit_value = 0;
	return (1);
}

/**
 * @brief Executes external commands
 * 
 * Finds the path of the command and executes it using execve.
 * Creates a child process to execute the command and waits for it to finish.
 * 
 * @param line The command line to execute
 * @param envs List of environment variables
 */
void			exec_others(char *line, t_list *envs)
{
	int		status;
	char	*path;
	char	**argv;
	pid_t	child;

	argv = get_argv(line, envs);
	path = find_path(argv[0], envs);
	if (!path)
	{
		ft_puterror_fd(argv[0], ": command not found", 2);
		return ;
	}
	child = fork();
	if (child == 0)
	{
		if (execve(path, argv, g_envp) == -1)
			exit(ft_puterror_fd(argv[0], ": command not found", 2));
		exit(EXIT_SUCCESS);
	}
	wait(&status);
	free(path);
	free_double_arr(argv);
	g_exit_value = status / 256;
}

/**
 * @brief Main command execution function
 * 
 * Determines the type of command (pipe, redirection, $?, built-in, or external)
 * and calls the appropriate function to execute it.
 * 
 * @param line The command line to execute
 * @param envs List of environment variables
 */
void			exec_cmds(char *line, t_list *envs)
{
	if (has_pipe(line))
		exec_pipe(line, envs);
	else if (has_redir(line))
		exec_redir(line, envs);
	else if (!exec_dollar(line) && !exec_builtin(line, envs))
		exec_others(line, envs);
}

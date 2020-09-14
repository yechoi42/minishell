/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_prompt_art.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwon <jwon@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/13 16:56:41 by jwon              #+#    #+#             */
/*   Updated: 2020/09/14 12:13:12 by jwon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		input_catch_ctrld(int ctrld, char **line)
{
	char	*temp;
	char	*buf;

	buf = "";
	while (!(get_next_line(0, line)))
	{
		temp = ft_strjoin(buf, *line);
		*buf ? free(buf) : 0;
		buf = temp;
		if (!(**line) && !ctrld)
		{
			ft_putstr_fd("exit", 1);
			free(buf);
			exit(EXIT_SUCCESS);
		}
		ft_putstr_fd("  \b\b", 1);
		ctrld = 1;
		free(*line);
	}
	if (ctrld)
	{
		free(*line);
		*line = ft_strdup(buf);
	}
	*buf ? free(buf) : 0;
}

int				input_from_prompt(char **line)
{
	int		ctrld;
	char	*temp;

	ctrld = 0;
	input_catch_ctrld(ctrld, line);
	if (**line == '\n')
	{
		free(*line);
		return (0);
	}
	if (*line != NULL)
	{
		temp = ft_strtrim(*line, " ");
		if (*line)
		{
			free(*line);
			*line = NULL;
		}
		*line = temp;
	}
	return (1);
}

void			show_prompt(char *user)
{
	char	*pwd;
	char	*tmp;
	char	*tmp2;
	char	*set;

	tmp = getcwd(0, 1024);
	set = ft_strjoin("/Users/", user);
	tmp2 = ft_strtrim(tmp, set);
	pwd = ft_strjoin("~/", tmp2);
	free(tmp2);
	ft_putstr_fd(user, 1);
	ft_putstr_fd(" \033[44m\033[30m ", 1);
	if (ft_strncmp(tmp, set, ft_strlen(tmp)) != 0)
		ft_putstr_fd(pwd, 1);
	else if (ft_strncmp(tmp, "/", ft_strlen(tmp)) == 0)
		ft_putstr_fd("/", 1);
	else if (ft_strncmp(tmp, "/Users", ft_strlen(tmp)) == 0)
		ft_putstr_fd("/Users", 1);
	else if (ft_strncmp(tmp, set, ft_strlen(tmp)) == 0)
		ft_putstr_fd("~", 1);
	ft_putstr_fd(" \033[0m $ ", 1);
	free(set);
	free(tmp);
	free(pwd);
}

void			show_art(void)
{
	int		fd;
	char	*line;

	fd = open("ascii_art", O_RDONLY);
	while (get_next_line(fd, &line))
	{
		ft_putstr_fd("\033[36m", 1);
		ft_putendl_fd(line, STDOUT_FILENO);
		free(line);
	}
	close(fd);
	free(line);
	ft_putendl_fd("\033[0m", 1);
}

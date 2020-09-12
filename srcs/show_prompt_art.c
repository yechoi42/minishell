
#include "minishell.h"

void		show_art(void)
{
	int		fd;
	char	*line;

	fd = open("art_header.txt", O_RDONLY);
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


void		input_from_prompt(char **line)
{
	char *temp;

	if (get_next_line(0, line) == 0)
	{
		ft_putstr_fd("exit", 1);
		exit(EXIT_SUCCESS);
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
}

void		show_prompt(char *user)
{
	char	*pwd;
	char	*tmp;
	char	*set;
	
	tmp = getcwd(0, 1024);
	set = ft_strjoin("/Users/", user);
	pwd = ft_strjoin("~/", ft_strtrim(tmp, set));
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
	free(tmp);
	free(pwd);
}
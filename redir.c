typedef struct s_redir 
{
	int		argc;
	char	**argv;
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
	int i;
	int j;
	int	start;

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
			r->argv[j] = ft_substr(command, start, i - start);
			r->types[j] = find_redir_type(command, i);
			r->types[j++] == DREDIR ? i++ : 0;
			start = i + 1;
		}
	}
	if (command[i] == '\0')
	{
		r->argv[j] = ft_substr(command, start, i - start);
		if (has_syntax_error(r->argv[j]))
			return (-1);
		r->types[j++] = 0;
		r->argv[j] = 0;
	}	
	return (1);
}		
// 0, -1 리턴값에서 프리 

int		get_fd(t_redir r)
{
	int fd;
	int i;

	i = 0;
	fd = 0;
	while (r.argv[++i])
	{
		if (i > 0)
			close(fd);
		if (r.types[i] == REDIR)
			fd = open(r.argv[i + 1], O_WRONLY | O_CREAT, 0744);
		else if (r.types[i] == DREDIR)
			fd = open(r.argv[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0744);
		else
			fd = open(r.argv[i + 1], O_RDWR | O_CREAT, 0644);			
	}
	return (fd);
}

void	exec_redir(t_cmd *cmd, t_list *envs)
{
	int		fd;
	int		ret;
	char	*path;
	t_redir r;
	pid_t	child;

	init_redir(cmd->command, &r);
	if ((ret = parse_redir(cmd->command, &r)) <= 0)
	{
		if (ret < 0)
			ft_putendl_fd("syntax error near unexpected token `newline'", 1);
		return;
	}
	child = fork();
	if (child == 0)
	{
		int i;
		i = 0;
		fd = 0;
		while (r.argv[++i])
		{
			if (i > 0)
				close(fd);
			if (r.types[i] == REDIR)
				fd = open(r.argv[i + 1], O_WRONLY | O_CREAT, 0744);
			else if (r.types[i] == DREDIR)
				fd = open(r.argv[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0744);
			else
				fd = open(r.argv[i + 1], O_RDWR | O_CREAT, 0644);			
		}
		dup2(fd, (r.types[i - 1] == BREDIR ? STDIN_FILENO : STDOUT_FILENO));
		path = find_path(r.argv[0], envs);
		execve(path, r.argv ,g_envp);
	}
	else
	{
		wait(0);
		//free(path);
	}

}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"

typedef struct s_env 
{
	char *key;
	char *value;
} t_env;

typedef struct s_cmd
{
	char	*command;
	int		pipe;
	int		redir;
} t_cmd;


char	**g_envp;

t_list	*get_envs(int argc, char **argv, char **envp)
{
	t_list	*envs;
	t_env	*env;
	int 	pos;
	
	envs = 0;
	if (!argc && argv)
		exit(1);
	while (*envp)
	{
		env = (t_env *)malloc(sizeof(t_env));
		pos = ft_strchr(*envp, '=') - *envp;
		env->key = ft_substr(*envp, 0, pos);
		env->value = ft_substr(*envp, pos + 1, ft_strlen(*envp) - pos - 1);
		ft_lstadd_back(&envs, ft_lstnew(env));
		envp++;
	}
	return (envs);
}

void 	print_envs(t_list *envs) // 환경변수 출력 함수
{
	if (envs == NULL)
		return ;
	ft_putstr_fd(((t_env *)envs->content)->key, 1);
	ft_putchar_fd('=', 1);
	ft_putendl_fd(((t_env *)envs->content)->value, 1);
	print_envs(envs->next);
}

int		is_valid_env(char *str) // export cmd 인자 유효성 체크
{
	if (ft_strchr(str, '='))
		return (1);
	return (0);
}

void	cmd_export(char **args, t_list *envs) // export 명령어 실행
{
	int		pos;
	t_env	*env;
	if (!(env = (t_env *)malloc(sizeof(t_env))))
		return ;
	pos = ft_strchr(*args, '=') - *args;
	env->key = ft_substr(*args, 0, pos - 1);
	env->value = ft_substr(*args, pos + 1, ft_strlen(*args) - pos - 1);
	while (*args)
	{
		if (is_valid_env(*args))
		{
			while (envs)
			{
				if (ft_strncmp(((t_env *)envs->content)->key, env->key, ft_strlen(env->key)) == 0)
				{
					free(((t_env *)envs->content)->value);
					((t_env *)envs->content)->value = ft_strdup(env->value);
					free(env->key);
					free(env);
					break ;
				}
				envs = envs->next;
			}
			if (!envs)
				ft_lstadd_back(&envs, ft_lstnew(env));
		}
		args++;
	}
}

void	catch_sigint()
{
	ft_putstr_fd("\n◕_◕ ༽つ", 1);
}

void	catch_signals(void)
{
	signal(SIGINT, catch_sigint);
	//should add SIGQUIT func
}

int		find_pipe(char *str)
{
	int	num;

	num = 0;
	while(*str)
	{
		if (*str == '|')
			num++;
		str++;
	}
	return (num);
}

int		find_redir(char *str)
{
	int num = 0;
	while (*str)
	{
		if (ft_strchr("><", *str))
			num++;
		str++;
	}
	return (num);
}

void	exec_redir(void)
{	
}

void	exec_pipe(void)
{
}

int		is_exist_key(char *key, t_list *envs) // 이미 존재하는 key인지 확인
{
	if (ft_strncmp(((t_env *)envs->content)->key, key, ft_strlen(key)) == 0)
		return (1);
	return (0);
}

char	*find_value(char *key, t_list *envs)
{
	while (envs)
	{
		if (is_exist_key(key, envs)) // key가 존재한다면?
			return (((t_env *)envs->content)->value);
		envs = envs->next;
	}
	return (NULL);
}

void	exec_builtin(t_cmd *cmd, t_list *envs)
{
	char	*path;
	char	**argv;
	pid_t	child;

	argv = ft_split(cmd->command, ' ');
	path = find_value("PATH", envs);
	child = fork();
	if (!child)
	{
		execve(path, argv, 0);
	}
	else
	{
		wait(0);
		free(path);
	}
	
}

void	exec_cmds(t_list *cmds, t_list *envs)
{
	while (cmds != NULL)
	{
		if (((t_cmd *)cmds->content)->redir)
			exec_redir();
		if (((t_cmd *)cmds->content)->pipe)
			exec_pipe();
		exec_builtin(((t_cmd *)cmds->content), envs);
		cmds = cmds->next;
	}
}

t_list	*get_cmds(char *line)
{
	int		i;
	int		start;
	int		end;
	t_cmd	*com;
	t_list 	*cmds;

	i = 0;
	start = 0;
	end = 0;
	cmds = 0;
	get_next_line(0, &line);
	if (!ft_strncmp(line, "\n", 1))
	{
		free(line);
		return (NULL);
	}
	while (line[i])
	{
		if (line[i] == ';')
		{ 	
			end = i;
			com = (t_cmd *)malloc(sizeof(t_cmd));
			com->command = ft_substr(line, start, end - start);
			com->pipe = find_pipe(com->command);
			com->redir = find_redir(com->command);
			ft_lstadd_back(&cmds, ft_lstnew(com));
			start = i + 1;
		}
		i++;
	}
	free(line);
	return(cmds);
}

void	free_cmds(void *content)
{
	free(((t_cmd *)content)->command);
}

void	init_cmds(t_list **cmds)
{
	if (*cmds)
		ft_lstiter(*cmds, free_cmds);
	*cmds = NULL;
}


int		main(int argc, char **argv, char **envp)
{
	char	*line;
	t_list	*cmds;
	t_list 	*envs;

	g_envp = envp;
	line = 0;
	cmds = 0;
	envs = get_envs(argc, argv, envp);
	catch_signals();
	while (1)
	{
	 	ft_putstr_fd("◕_◕ ༽つ", 1);
	 	cmds = get_cmds(line);
	 	exec_cmds(cmds, envs);
		init_cmds(&cmds);
	}
	return (0);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"

typedef struct s_env 
{
	char *key;
	char *value;
} t_env;

typedef struct s_com
{
	char	*command;
	int		pipe;
	int		redir;
} t_com;

t_list 	*g_envs;

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
		env->key = ft_substr(*envp, 0, pos - 1);
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

t_list	*get_cmds(char *line)
{
	int		i;
	int		start;
	int		end;
	t_com	*com;
	t_list 	*coms;

	i = 0;
	start = 0;
	end = 0;
	coms = 0;
	get_next_line(0, &line);
	if (!ft_strncmp(line, "\n", 1))
	{
		free(line);
		return (NULL);
	}
	while(line[i])
	{
		if (line[i] == ';')
		{ 	
			end = i;
			com = (t_com *)malloc(sizeof(t_com));
			com->command = ft_substr(line, start, end - start + 1);
			ft_lstadd_back(&coms, ft_lstnew(com));
			start = i + 1;
		}
		i++;
	}
	free(line);
	return(coms);
}

void	exec_cmds(t_list *cmds)
{
	while (cmds != NULL)
	{
		printf("%s ", ((t_com *)cmds->content)->command);
		cmds = cmds->next;
	}
}

void	free_cmds(void *content)
{
	free(((t_com *)content)->command);
}

void	init_cmds(t_list *cmds)
{
	if (cmds)
		ft_lstiter(cmds, free_cmds);
	cmds = NULL;
}


int		main(int argc, char **argv, char **envp)
{
	char	*line;
	t_list	*cmds;
	char	*test = "aaaaaa=123123123123";

	line = 0;
	cmds = 0;
	g_envs = get_envs(argc, argv, envp);
	catch_signals();
	//cmd_export(&test, g_envs);
	while (1)
	{
	 	ft_putstr_fd("◕_◕ ༽つ", 1);
		init_cmds(cmds);
	 	cmds = get_cmds(line);
	 	exec_cmds(cmds);
	}
	return (0);
}

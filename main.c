#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
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

int		is_valid_env(char *arg) // export cmd 인자 유효성 체크
{
	if (ft_strchr(arg, '='))
		return (1);
	return (0);
}

int		is_exist_key(char *key, t_list *envs) // 이미 존재하는 key인지 확인
{
	if (ft_strncmp(((t_env *)envs->content)->key, key, ft_strlen(key)) == 0)
		return (1);
	return (0);
}

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
/*************************************************************************/
void	free_double_arr(char **paths)
{
	int i;

	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
}

char	*find_path(char *argv, t_list *envs)
{
	int			i;
	char		*temp;
	char		*new_path;
	char		**paths;
	struct stat	s;

	temp = find_value("PATH", envs);
	paths = ft_split(temp, ':');
	i = -1;
	while (paths[++i])
	{
		temp = ft_strjoin("/", argv);
		new_path = ft_strjoin(paths[i], temp);
		free(temp);
		if (stat(new_path, &s) == 0)
		{
			free_double_arr(paths);
			return (new_path);
		}
		free(new_path);
	}
	free_double_arr(paths);
	return (NULL);
}

void	cmd_others(char **argv, t_list *envs)
{
	char	*path;
	pid_t	child;
	int		status;

	path = find_path(argv[0], envs);
	if (!path)
	{
		ft_putstr_fd(argv[0],1);
		ft_putendl_fd(": command not found", 1);
		return;
	}
	child = fork();
	if (child == 0)
	{
		execve(path, argv, g_envp);
	}
	else
	{
		wait(0);
		free(path);
	}
}
/*************************************************************************/
void	cmd_pwd(char **argv, t_list *envs)
{
	char	*pwd;

	pwd = getcwd(0, 1024);
	ft_putendl_fd(pwd, 1);
	free(pwd);
}
/*************************************************************************/
void	cmd_cd(char **argv, t_list *envs)
{
	char *path;

	path = 0;
	if (argv[1] == NULL)
	{
		path = find_value("HOME", envs);
		if (chdir(path) == -1)
			ft_putstr_fd(strerror(errno), 2);
		free(path);
		return;
	}
	else if (*argv[1] == '$')
	{
		path = find_value(argv[1] + 1, envs);
		if(chdir(path) == -1)
			ft_putendl_fd(strerror(errno), 2); //에러메세지 보완 필요
		free(path);
		return;
	}
	if (chdir(argv[1]) == -1)
		ft_putstr_fd(strerror(errno), 2);
}

/*************************************************************************/
void 	print_envs(t_list *envs) // 환경변수 출력 함수
{
	if (envs == NULL)
		return ;
	ft_putstr_fd(((t_env *)envs->content)->key, 1);
	ft_putchar_fd('=', 1);
	ft_putendl_fd(((t_env *)envs->content)->value, 1);
	print_envs(envs->next);
}
/*************************************************************************/

void	update_value(t_env *env, t_list **envs) // value 업데이트
{
	// free(((t_env *)envs->content)->value);
	((t_env *)(*envs)->content)->value = env->value;
	// free(env->key);
	// free(env);
}

void	add_env_or_modify_value(char **argv, t_list **envs)
{
	t_env	*env;
	t_list	*curr;
	int		pos;
	if (!(env = (t_env *)malloc(sizeof(t_env))))
		return ;
	pos = ft_strchr(*argv, '=') - *argv;
	env->key = ft_substr(*argv, 0, pos);
	env->value = ft_substr(*argv, pos + 1, ft_strlen(*argv) - pos - 1);
	curr = *envs;
	while (curr)
	{
		if (is_exist_key(env->key, curr)) // key가 존재한다면?
		{
			update_value(env, &curr);
			return ;
		}
		curr = curr->next;
	}
	if (curr == NULL)
		ft_lstadd_back(envs, ft_lstnew(env)); // 맨 뒤에 추가
}

void	print_double_arr(char **arr) // 2차원 배열 출력 함수
{
	int		idx;
	idx = 0;
	while (arr[idx])
		ft_putendl_fd(arr[idx++], 1);
}
void	sort_double_arr(char **arr) // 2차원 배열 내림차순 정렬 함수
{
	int		i;
	int		j;
	int		len;
	char	*temp;
	i = 0;
	while (arr[i + 1])
	{
		if (ft_strncmp(arr[i], arr[i + 1], 100) > 0)
		{
			temp = arr[i];
			arr[i] = arr[i + 1];
			arr[i + 1] = temp;
			i = -1;
		}
		i++;
	}
}
void	modify_env_for_export(char **arr) // export 출력 위해 앞에 declare -x 붙이는 함수
{
	int		idx;
	idx = 0;
	while (arr[idx])
	{
		arr[idx] = ft_strjoin("declare -x ", arr[idx]);
		idx++;
	}
}

char	**convert_lst_to_arr(t_list *lst) // 연결리스트를 배열로 변환하는 함수
{
	int		idx;
	int		count;
	char	**arr;
	char	*keytmp;
	char	*valuetmp;
	count = ft_lstsize(lst);
	if (!(arr = (char **)malloc(sizeof(char *) * (count + 1))))
		return (NULL);
	idx = 0;
	while (lst)
	{
		keytmp = ft_strjoin(((t_env *)lst->content)->key, "=\"");
		valuetmp = ft_strjoin((((t_env *)lst->content)->value), "\"");
		arr[idx] = ft_strjoin(keytmp, valuetmp);
		free(keytmp);
		free(valuetmp);
		lst = lst->next;
		idx++;
	}
	arr[idx] = NULL;
	return (arr);
}

void	cmd_export(char **argv, t_list *envs) // export 명령어 실행
{
	char	**copy;

	if (argv[1] == NULL)
	{
		copy = convert_lst_to_arr(envs);
		sort_double_arr(copy);
		modify_env_for_export(copy);
		print_double_arr(copy);
	}
	argv++;
	while (*argv)
	{
		if (!is_valid_env(*argv))
		{
			ft_putstr_fd("export: `", 1);
			ft_putstr_fd(*argv, 1);
			ft_putendl_fd("': not a valid identifier", 1);
			return;
		}
		add_env_or_modify_value(argv, &envs);
		argv++;
	}
}
/*************************************************************************/
void	delete_key(char *argv, t_list *envs)
{
	t_list *curr;
	t_list *next;
	curr = envs;
	while (curr->next)
	{
		if (is_exist_key(argv, curr->next)) // key가 존재한다면?
		{
			next = curr->next;
			curr->next = next->next;
			free(((t_env *)next->content)->key);
			free(((t_env *)next->content)->value);
			free(next->content);
			free(next);
			return ;
		}
		curr = curr->next;
	}
}
void	cmd_unset(char **argv, t_list *envs) // unset 명령어 실행
{
	argv++;
	while (*argv)
	{
		delete_key(*argv, envs);
		argv++;
	}
}
/*************************************************************************/

int		all_digit(char *str)
{
	int	i;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	cmd_exit(char **argv, t_list *envs)
{
	int	argc;
	int	i;

	argc = 0;
	while (argv[argc] != NULL)
		argc++;
	if (argc == 1)
		exit(0);
	else if (argc == 2 && all_digit(argv[1]))
	{
		exit(ft_atoi(argv[1]));
	}
	else if (argc > 2 && all_digit(argv[1]))
		ft_putendl_fd("bash: exit: too many arguments", 1);
	else
	{
		ft_putstr_fd("bash: exit: numeric argument required", 1);
		exit(2);
	}
}
/*************************************************************************/

#define QUOTE	39
#define DQUOTE	34
#define	ETC		42
#define INIT	-1

typedef struct s_quote
{
	int	type;
	int	start;
	int	end;
} t_quote;

void	init_quote(t_quote *q)
{
	q->type = INIT;
	q->start = INIT;
	q->end = INIT;
}

void	check_quote(char *argv, int idx, t_quote *q)
{
	if (q->type == INIT)
	{
		if (argv[idx] == '\'')
		{
			printf("quote starts\n");
			q->type = QUOTE;
			q->start = idx + 1;
		}
		else if (argv[idx] == '\"')
		{
			printf("dquote starts\n");
			q->type = DQUOTE;
			q->start = idx + 1;
		}
		else
		{
			printf("etc starts\n");
			q->type = ETC;
			q->start = idx;
		}
		return;
	}
	if ((argv[idx] == '\'' && q->type == QUOTE) || 
	(argv[idx] == '\"' && q->type == DQUOTE))
		q->end = idx - 1;
	else if ((argv[idx + 1] == '\0' && q->type == QUOTE) ||
	(argv[idx + 1] == '\0' && q->type == DQUOTE))
		q->end = idx;
	else if (q->type == ETC &&
	(argv[idx + 1] == '\'' || argv[idx + 1] == '\"' || argv[idx + 1] == '\0'))
		q->end = idx;
	return;
}

char	*parse_argv(char *argv, t_list *envs)
{
	int		idx;
	char	*tmp[3];
	t_quote	q;

	idx = 0;
	tmp[0] = "";
	init_quote(&q);
	while (argv[idx])
	{
		check_quote(argv, idx, &q);
		if (q.end != INIT)
		{
			tmp[1] = ft_substr(argv, q.start, q.end - q.start + 1);
			tmp[2] = ft_strjoin(tmp[0], tmp[1]);
			if (tmp[0] && *tmp[0])
				free(tmp[0]);
			if (tmp[1])
				free(tmp[1]);
			tmp[0] = tmp[2];
			init_quote(&q);
		}
		idx++;
	}
	return (tmp[0]); 
}

char	**modify_argv(char **argv, t_list *envs)
{
	int		idx;
	int		len;
	char 	**ret;

	len = 0;
	while (argv[len])
		len++;
	ret = (char **)malloc(sizeof(char *) * (len + 1));
	ret[0] = ft_strdup(argv[0]);
	ret[len] = NULL;
	idx = 1;
	while (argv[idx])
	{
		ret[idx] = parse_argv(argv[idx], envs);
		free(argv[idx]);
		idx++;
	}
	return (ret);
}

int		has_quote(char **argv)
{
	int i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] == '\'' || argv[i][j] == '\"')
				return (1);
			j++;
		}
		i++;	
	}
	return (0);
}

void	exec_builtin(t_cmd *cmd, t_list *envs)
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
	else
		cmd_others(argv, envs);	
	// 해당하지 않는 명령어일 경우에는 어떻게 할지 생각해봐야 할 듯. 리턴값도 함께. 	
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

void	add_cmd_to_list(char *line, t_list **cmds, int start, int end)
{
	t_cmd *com;

	com = (t_cmd *)malloc(sizeof(t_cmd));
	com->command = ft_substr(line, start, end - start);
	com->pipe = find_pipe(com->command);
	com->redir = find_redir(com->command);
	ft_lstadd_back(cmds, ft_lstnew(com));
}

void	input_from_prompt(char **line)
{
	char *temp;

	get_next_line(0, line);
	temp = ft_strtrim(*line, " ");
	free(*line);
	*line = temp;
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
	input_from_prompt(&line);
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
			add_cmd_to_list(line, &cmds, start, end);
			start = i + 1;
		}
		i++;
	}
	if (line[i - 1] != ';')
		add_cmd_to_list(line, &cmds, start, i);
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
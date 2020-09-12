#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "./libs/libft/libft.h"


#define INIT	-1
#define QUOTE	39
#define DQUOTE	34
#define	ETC		42
#define	REDIR	62
#define	DREDIR	6
#define	BREDIR	60


char			**g_envp;
char			*g_env_user;
int				g_exit_value;

typedef struct	s_env
{
	char 		*key;
	char 		*value;
}				t_env;

typedef struct	s_quote
{
	int			type;
	int			start;
	int			end;
} 				t_quote;

typedef struct	s_redir 
{
	int			argc;
	char		**argv;
	char		**cmds;
	char		*types;
}				t_redir;


void	ft_puterror_fd(char *s1, char *s2, int fd)
{
	ft_putstr_fd(s1, fd);
	ft_putendl_fd(s2, fd);
}

int		all_digit(char *str)
{
	int	idx;

	idx = 0;
	while (str[idx])
	{
		if (!ft_isdigit(str[idx]))
			return (0);
		idx++;
	}
	return (1);
}

char	*substr_and_trim(char *command, int start, int num, char *charset)
{
	char	*temp;
	char	*temp2;

	temp = ft_substr(command, start, num);
	temp2 = ft_strtrim(temp, charset);
	free(temp);
	return (temp2);
}

void	free_double_arr(char **arr)
{
	int		idx;

	idx = -1;
	while (arr[++idx])
		free(arr[idx]);
	free(arr);
}

int		has_pipe(char *str)
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

int		has_redir(char *str)
{
	int num;

	num = 0;
	while (*str)
	{
		if (ft_strchr("><", *str))
			num++;
		str++;
	}
	return (num);
}

t_list			*get_envs(int argc, char **argv, char **envp)
{
	int 	pos;
	t_env	*env;
	t_list	*envs;

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

int		is_valid_env(char *arg)
{
	if (ft_strchr(arg, '='))
		return (1);
	return (0);
}

int		is_exist_key(char *key, t_list *envs)
{
	if (ft_strncmp(((t_env *)envs->content)->key, key, ft_strlen(key)) == 0)
		return (1);
	return (0);
}


char			*find_value(char *key, t_list *envs)
{
	while (envs)
	{
		if (is_exist_key(key, envs))
			return (((t_env *)envs->content)->value);
		envs = envs->next;
	}
	return ("");
}

char			*find_path(char *argv, t_list *envs)
{
	int			idx;
	char		*temp;
	char		*new_path;
	char		**paths;
	struct stat	s;

	if (!(temp = find_value("PATH", envs)))
		return (NULL);
	paths = ft_split(temp, ':');
	idx = -1;
	while (paths[++idx])
	{
		temp = ft_strjoin("/", argv);
		new_path = ft_strjoin(paths[idx], temp);
		free(temp);
		if (stat(new_path, &s) == 0)
		{
			free_double_arr(paths);
			return (new_path);
		}
		free(new_path);
	}
	free_double_arr(paths);
	return (ft_strdup(argv));
}

/***************************************************/

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


void 	handle_signal(int signo)
{
	int		status;

	if (signo == SIGINT)
	{
		ft_putstr_fd("\b\b  \b\b", 1);
		ft_putstr_fd("\n", 1);
		show_prompt(g_env_user);
		g_exit_value = 130;
	}
	else if (signo == SIGQUIT)
	{
		signo = wait(&status);
		g_exit_value = status / 256;
		ft_putstr_fd("\b\b  \b\b", 1);
		if (signo != -1)
			ft_putstr_fd("^\\Quit: 3\n", 1);
		else
			return ;
		g_exit_value = 131;
	}
	return ;
}

int		input_from_prompt(char **line)
{
	char	*temp;

	if ((get_next_line(0, line) == 0))
	{
		ft_putstr_fd("exit", 1);
		exit(EXIT_SUCCESS);
	}
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

/***********************************************/

int		has_semicolon_syntax_error(const char *line)
{
	int		i;
	char	*temp;

	i = -1;
	temp = ft_strtrim(line, " ");
	if (temp[0] == ';')
	{
		free(temp);
		return (1);
	}
	while (temp[++i])
	{
		if (temp[i] == ';' && temp[i + 1] == ';')
		{
			free(temp);
			return (1);
		}
	}
	free(temp);
	return (0);
}

char	**get_cmds(char *line)
{
	int		i;
	int		empty; // has empty cmds(syntax error)
	char	*temp;
	char	**cmds;

	empty = 0;
	cmds = ft_split(line, ';');
	i = -1;
	while (cmds[++i])
	{
		temp = ft_strtrim(cmds[i], " ");
		empty = (!temp || !(*temp)) ? 1 : 0;
		free(cmds[i]);
		!empty ? cmds[i] = temp : 0;
	}
	if (empty || has_semicolon_syntax_error(line))
	{
		free(cmds);
		ft_putendl_fd("syntax error near unexpected token `;'", 2);
		return (NULL);
	}
	return (cmds);
}

int		has_quote(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (1);
	}
	return (0);
}

int		has_env(char *str, t_list *envs)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '$')
			return (1);
	}
	return (0);
}

static void		init_quote(t_quote *q)
{
	q->type = INIT;
	q->start = INIT;
	q->end = INIT;
}

static void		check_end(char *argv, int idx, t_quote *q)
{
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

static void		check_quote(char *argv, int idx, t_quote *q)
{
	if (q->type == INIT)
	{
		if (argv[idx] == '\'')
		{
			q->type = QUOTE;
			q->start = idx + 1;
		}
		else if (argv[idx] == '\"')
		{
			q->type = DQUOTE;
			q->start = idx + 1;
		}
		else
		{
			q->type = ETC;
			q->start = idx;
		}
		return ;
	}
	check_end(argv, idx, q);
}

char *parse_env(char *argv, t_list *envs)
{
	int 	i;
	int		start;
	char	*ret;
	char	*temp[3];

	i = -1;
	start = 0;
	while (argv[++i])
	{
		if (argv[i] == '$' || argv[i + 1] == '\0')
		{
			if (start == 0)
			{
				ret = ft_substr(argv, start, i);
				start = i + 1;
				continue;
			}
			temp[0] = ft_substr(argv, start, i - start);
			temp[1] = ft_strdup(find_value(temp[0], envs));
			temp[2] = ft_strjoin(ret, temp[1]);
			free(ret);
			ret = temp[2];
			start = i + 1;
		}
	}
	return (ret);
}

char		*parse_quote(char *argv, t_list *envs)
{
	int		idx;
	char	*tmp[4];
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
			if (q.type == DQUOTE && has_env(tmp[1], envs))
				tmp[1] = parse_env(tmp[1], envs);
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


char	*modify_argv(char *str, t_list *envs)
{
	char	*ret;
	
	if (has_quote(str))
	{
		ret = parse_quote(str, envs);
	}
	else if (has_env(str, envs))
		ret = parse_env(str, envs);
	free(str);
	return (ret);
}

char	**get_argv(char *line, t_list *envs)
{
	int		i;
	char 	**argv;

	if (!(argv = ft_split(line, ' ')))
		return (0);
	i = -1;
	while (argv[++i])
	{
		if (has_quote(argv[i]) || has_env(argv[i], envs))
			argv[i] = modify_argv(argv[i], envs);
	}
	return (argv);
}

/**************************************************************/

void	cmd_cd(char **argv, t_list *envs)
{
	char *path;

	path = 0;
	if (ft_strlen(argv[1]) == 1 && *argv[1] == '~')
		argv[1] = ft_strjoin("$", "HOME");
	if (*argv[1] == '$')
	{
		path = find_value(argv[1] + 1, envs);
		if (chdir(path) == -1)
			ft_putendl_fd(strerror(errno), 2);
		free(path);
		return ;
	}
	else if (argv[1] == NULL)
	{
		path = find_value("HOME", envs);
		if (chdir(path) == -1)
			ft_putendl_fd(strerror(errno), 2);
		free(path);
		return ;
	}
	if (chdir(argv[1]) == -1)
		ft_putendl_fd(strerror(errno), 2);
}

/****************************************************************/

static void 	print_envs(t_list *envs) // ȯ�溯�� ���? �Լ�
{
	if (envs == NULL)
		return ;
	ft_putstr_fd(((t_env *)envs->content)->key, 1);
	ft_putchar_fd('=', 1);
	ft_putendl_fd(((t_env *)envs->content)->value, 1);
	print_envs(envs->next);
}

static char		*make_tmp_envs(char *tmp_envs, char *argv)
{
	char	*tmp;

	if (tmp_envs == NULL)
		tmp_envs = ft_strjoin(argv, "\n");
	else
	{
		tmp = ft_strjoin(argv, "\n");
		tmp_envs = ft_strjoin(tmp_envs, tmp);
		free(tmp);
	}
	return (tmp_envs);
}

void			cmd_env(char **argv, t_list *envs)
{
	char	*tmp_envs;

	tmp_envs = 0;
	argv++;
	while (*argv)
	{
		if (!is_valid_env(*argv))
		{
			ft_putstr_fd("env: ", 1);
			ft_putstr_fd(*argv, 1);
			ft_putendl_fd(": No such file or directory", 1);
			return ;
		}
		tmp_envs = make_tmp_envs(tmp_envs, *argv);
		argv++;
	}
	print_envs(envs);
	if (tmp_envs != NULL)
	{
		ft_putstr_fd(tmp_envs, 1);
		free(tmp_envs);
	}
}

/*************************************************************************/

void	print_double_arr(char **arr) // 2���� �迭 ��� �Լ�
{
	int		idx;
	idx = 0;
	while (arr[idx])
		ft_putendl_fd(arr[idx++], 1);
}

void	sort_double_arr(char **arr) // 2���� �迭 �������� ���� �Լ�
{
	int		i;
	int		j;
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


static void		update_value(t_env *env, t_list **envs) // value ������Ʈ
{
	// free(((t_env *)envs->content)->value);
	((t_env *)(*envs)->content)->value = env->value;
	// free(env->key);
	// free(env);
}

static void		modify_env_for_export(char **arr) // export ��� ���� �տ� declare -x ���̴� �Լ�
{
	int		idx;

	idx = 0;
	while (arr[idx])
	{
		arr[idx] = ft_strjoin("declare -x ", arr[idx]);
		idx++;
	}
}

static char		**convert_lst_to_arr(t_list *lst) // ���Ḯ��Ʈ�� �迭�� ��ȯ�ϴ� �Լ�
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

void			add_env_or_modify_value(char **argv, t_list **envs) 
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
		if (is_exist_key(env->key, curr)) 
		{
			update_value(env, &curr);
			return ;
		}
		curr = curr->next;
	}
	if (curr == NULL)
		ft_lstadd_back(envs, ft_lstnew(env)); 
}

void			cmd_export(char **argv, t_list *envs) 
{
	char	**temp;

	if (argv[1] == NULL)
	{
		temp = convert_lst_to_arr(envs); 
		sort_double_arr(temp); 
		modify_env_for_export(temp); 
		print_double_arr(temp); 
		free_double_arr(temp); 
		return ; 
	}
	argv++;
	while (*argv)
	{
		if (!is_valid_env(*argv))
		{
			ft_putstr_fd("export: `", 1);
			ft_putstr_fd(*argv, 1);
			ft_putendl_fd("': not a valid identifier", 1);
			return ;
		}
		add_env_or_modify_value(argv, &envs);
		argv++;
	}
}

/****************************************************************/
static void		delete_key(char *argv, t_list *envs) 
{
	t_list *curr;
	t_list *next;

	curr = envs;
	while (curr->next)
	{
		if (is_exist_key(argv, curr->next)) // key�� �����Ѵٸ�?
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

void			cmd_unset(char **argv, t_list *envs) // unset ���ɾ� ����
{
	argv++;
	while (*argv)
	{
		delete_key(*argv, envs);
		argv++;
	}
}

/****************************************************************/

void	print_exit_status()
{
	ft_putstr_fd(ft_itoa(g_exit_value), 1);
	ft_putendl_fd(": command not found", 1);
}

void	cmd_exit(char **argv, t_list *envs)
{
	int	argc;

	argc = 0;
	while (argv[argc] != NULL)
		argc++;
	if (argc == 1)
	{
		ft_putendl_fd("exit", 1);
		exit(EXIT_SUCCESS);
	}
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
/**************************************************************/


void	cmd_pwd(char **argv, t_list *envs)
{
	char	*pwd;

	pwd = getcwd(0, 1024);
	ft_putendl_fd(pwd, 1);
	free(pwd);
}



int		exec_builtin(char *line, t_list *envs)
{
	char **argv;

	if (line[0] == '$' && line[1] == '?')
	{
		print_exit_status();
		return (1);
	}
	argv = get_argv(line, envs);
	if (!ft_strncmp(argv[0], "cd", ft_strlen(argv[0])))
		cmd_cd(argv, envs);
	else if (!ft_strncmp(argv[0], "pwd", ft_strlen(argv[0])))
		cmd_pwd(argv, envs);
	else if (!ft_strncmp(argv[0], "env", ft_strlen(argv[0])))
		cmd_env(argv, envs);
	else if (!ft_strncmp(argv[0], "export", ft_strlen(argv[0])))
		cmd_export(argv, envs);
	else if (!ft_strncmp(argv[0], "unset", ft_strlen(argv[0])))
		cmd_unset(argv, envs);
	else if (!ft_strncmp(argv[0], "exit", ft_strlen(argv[0])))
		cmd_exit(argv, envs);
	else
		return (0);
	return (1);
}

/***********************************************************/

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
		ft_putstr_fd(argv[0], 1);
		ft_putendl_fd(": command not found", 1);
		exit(127);
	}
	child = fork();
	if (child == 0)
	{
		if (execve(path, argv, g_envp) == -1)
		{
			ft_putstr_fd(argv[0], 1);
			ft_putendl_fd(": command not found", 1);
			free(path);
			exit(127);
		}
		free(path);
	}
	else
	{
		wait(&status);
		g_exit_value = status / 256;
	}
}
/*********************************************/

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

int		has_redir_syntax_error(char *str)
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

int	parse_redir(char *command, t_redir *r)
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
	r->argv[j] = substr_and_trim(command, start, i - start, " ");
	if (has_redir_syntax_error(r->argv[j]))
		return (-1);
	r->types[j++] = 0;
	r->argv[j] = 0;
	r->argc = j;
	return (1);
}

void	open_unnecessary_files(t_redir *r)
{
	int	i;
	int	fd;

	i = 1;
	while (i < r->argc - 1)
	{
		if (i > 1)
			close(fd);
		if (r->types[i - 1] == REDIR)
			fd = open(r->argv[i], O_WRONLY | O_CREAT | O_TRUNC, 0744);
		else if (r->types[i - 1] == DREDIR)
			fd = open(r->argv[i], O_WRONLY | O_CREAT | O_APPEND, 0744);
		else
            fd = open(r->argv[i], O_RDONLY, 0644);	
		i++;
	}
	close(fd);
}		

// void	cmd_redir(t_redir *r, t_list *envs)
// {
// 	int		i;
// 	int		fd;
// 	int		status;
// 	pid_t	child;
// 	char	*path;

// 	i = 0;
// 	child = fork();
// 	if (child == 0)
// 	{
// 		if (r->types[r->argc - 2] == REDIR)
// 			fd = open(r->argv[r->argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0744);
// 		else if (r->types[r->argc - 2] == DREDIR)
// 			fd = open(r->argv[r->argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0744);
// 		else
// 			fd = open(r->argv[r->argc - 1], O_RDONLY, 0644);
//         if (fd < 0)
//         {
//             ft_putstr_fd(r->argv[r->argc - 1], 1);
// 			ft_putendl_fd(": No such file or directory", 1);
// 			exit(EXIT_FAILURE);
//         }
// 		if (!(path = find_path(r->cmds[0], envs)))
// 		{
// 			ft_putstr_fd(r->cmds[0], 1);
// 			ft_putendl_fd(": command not found", 1);
// 			exit(127);
// 		}
//         if (r->types[r->argc - 2] == BREDIR)
//             dup2(fd, STDIN_FILENO);
//         else
// 		    dup2(fd, STDOUT_FILENO);
// 		close(fd);
//         if (!exec_builtin(r->argv[r->argc - 1], envs))
//         {
//             if (execve(path, r->cmds, g_envp) == -1)
//             {
//                 ft_putstr_fd(r->cmds[0], 1);
//                 ft_putendl_fd(": command not found", 1);
//                 free(path);
//                 exit(127);
//             }
//         }
// 		free(path);
//         exit(EXIT_SUCCESS);
// 	}
// 	else
// 	{
// 		wait(&status);
// 		g_exit_value = status / 256;
// 	}
// }

int		do_redir(t_redir *r, t_list *envs)
{
	int		i;
	int		fd;
	char	*path;

	fd = open(r->argv[r->argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0744);
	if (fd < 0)
    {
		ft_puterror_fd(r->argv[r->argc - 1], ": No such file or directory", 2);
		return(EXIT_FAILURE);
    }
	if (!(path = find_path(r->cmds[0], envs)))
	{
		ft_puterror_fd(r->cmds[0], ": command not found", 2);
		return(127);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	if (!exec_builtin(r->argv[r->argc - 1], envs) && (execve(path, r->cmds, g_envp) == -1))
    {
		ft_puterror_fd(r->cmds[0], ": command not found", 2);
        free(path);
        return (127);
    }
	free(path);
    return(EXIT_SUCCESS);
}

int		do_dredir(t_redir *r, t_list *envs)
{
	int		i;
	int		fd;
	char	*path;

	fd = open(r->argv[r->argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0744);
	if (fd < 0)
    {
		ft_puterror_fd(r->argv[r->argc - 1], ": No such file or directory", 2);
		return(EXIT_FAILURE);
    }
	if (!(path = find_path(r->cmds[0], envs)))
	{
		ft_puterror_fd(r->cmds[0], ": command not found", 2);
		return(127);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	if (!exec_builtin(r->argv[r->argc - 1], envs) && (execve(path, r->cmds, g_envp) == -1))
    {
		ft_puterror_fd(r->cmds[0], ": command not found", 2);
        free(path);
        return (127);
    }
	free(path);
    return(EXIT_SUCCESS);
}

int		do_bredir(t_redir *r, t_list *envs)
{
	int		i;
	int		fd;
	char	*path;

	fd = open(r->argv[r->argc - 1], O_RDONLY, 0644);
	if (fd < 0)
    {
		ft_puterror_fd(r->argv[r->argc - 1], ": No such file or directory", 2);
		return(EXIT_FAILURE);
    }
	if (!(path = find_path(r->cmds[0], envs)))
	{
		ft_puterror_fd(r->cmds[0], ": command not found", 2);
		return(127);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	if (!exec_builtin(r->argv[r->argc - 1], envs) && (execve(path, r->cmds, g_envp) == -1))
    {
		ft_puterror_fd(r->cmds[0], ": command not found", 2);
        free(path);
        return (127);
    }
	free(path);
    return(EXIT_SUCCESS);
}

void	cmd_redir(t_redir *r, t_list *envs)
{
	int		i;
	int		ret;
	int		status;
	pid_t	child;
	char	*path;

	i = 0;
	child = fork();
	if (child == 0)
	{
		if (r->types[r->argc - 2] == REDIR)
			ret = do_redir(r, envs);
		else if (r->types[r->argc - 2] == DREDIR)
			ret = do_dredir(r, envs);
		else
			ret = do_bredir(r, envs);
		exit(ret);
	}
	else
	{
		wait(&status);
		g_exit_value = status / 256;
	}
}


void	exec_redir(char *line, t_list *envs)
{
	int		i;
	int		ret;
	t_redir r;

	init_redir(line, &r);
	if ((ret = parse_redir(line, &r)) <= 0)
	{
		if (ret < 0)
			ft_putendl_fd("syntax error near unexpected token `newline'", 1);
		return;
	}
	i = -1;
	while (r.cmds[++i])
	{
		if (has_quote(r.cmds[i]))
			r.cmds[i] = modify_argv(r.cmds[i], envs);
	}
	open_unnecessary_files(&r);
	cmd_redir(&r, envs);
}


/************************************************/
void	exec_cmds(char *line, t_list *envs)
{
	//if (has_pipe(line))
	// 	exec_pipe(line, envs);
	if (has_redir(line))
		exec_redir(line, envs);
	else if (!exec_builtin(line, envs))
		exec_others(line, envs);	
}

void	setup(t_list *envs)
{
	g_exit_value = 0;
	g_env_user = find_value("USER", envs);
	show_art();
	signal(SIGINT, (void *)handle_signal);
	signal(SIGQUIT, (void *)handle_signal);
}

int     main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**cmds;
	t_list	*envs;

	g_envp = envp;
	envs = get_envs(argc, argv, envp);
	setup(envs);
	line = 0;
	while (1)
	{
		show_prompt(g_env_user);
		if (!input_from_prompt(&line))
			continue ;
		if ((cmds = get_cmds(line)) == NULL)
			continue ;
		while (*cmds)
		{
			exec_cmds(*cmds, envs);
			cmds++;
		}
	}
	return (0);
}
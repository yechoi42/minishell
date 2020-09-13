#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../libs/libft/libft.h"


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

typedef struct	s_pipe
{
	char		*line;
	char		**argv;
}				t_pipe;


void	exec_cmds(char *line, t_list *envs);

#endif
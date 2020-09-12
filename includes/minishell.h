/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwon <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:13:45 by jwon              #+#    #+#             */
/*   Updated: 2020/09/08 21:11:23 by jwon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

typedef struct 	s_cmd
{
	char		*command;
	int			pipe;
	int			redir;
} 				t_cmd;

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
	int			argc;
	char		***argv;
	char		**cmds;
}				t_pipe;



/* show_prompt_art.c */
void			show_art(void);
// void			show_prompt(t_list *envs);
void			input_from_prompt(char **line);
void			show_prompt(char *user);

/* signal.c */
void 			handle_signal(int signo);

/* get_cmds.c */
void			init_cmds(t_list **cmds);
t_list			*get_cmds(char *line);

/* exec_cmds.c */
int				exec_builtin(char **argv, t_list *envs);
void			exec_cmds(t_list *cmds, t_list *envs);

/* quote.c */
char			**modify_argv(char **argv, t_list *envs);
int				has_quote(char **argv);

/* cmd_pwd_cd.c */
void			cmd_pwd(char **argv, t_list *envs);
void			cmd_cd(char **argv, t_list *envs);

/* cmd_env.c */
char			*find_value(char *key, t_list *envs);
t_list			*get_envs(int argc, char **argv, char **envp);
void			cmd_env(char **argv, t_list *envs);

/* cmd_export.c */
void			add_env_or_modify_value(char **argv, t_list **envs);
void			cmd_export(char **argv, t_list *envs);

/* cmd_export_utils.c */
void			print_double_arr(char **arr);
void			sort_double_arr(char **arr);

/* cmd_unset.c */
void			cmd_unset(char **argv, t_list *envs);

/* cmd_exit.c */
void			print_exit_status(void);
void			cmd_exit(char **argv, t_list *envs);

/* cmd_others.c */
char			*find_path(char *argv, t_list *envs);
void			exec_others(char **argv, t_list *envs);

/* cmd_redir.c*/
void			cmd_redir(t_redir *r, t_list *envs);
void			exec_redir(t_cmd *cmd, t_list *envs);

/* cmd_redir_utils.c*/
void			init_redir(char *command, t_redir *r);
int				is_single_redir(char *command, int i);
int				find_redir_type(char *command, int i);
int				has_syntax_error(char *str);

/* utils */
int				is_valid_env(char *arg);
int				is_exist_key(char *key, t_list *envs);
int				all_digit(char *str);
void			free_double_arr(char **arr);
char			*substr_and_trim(char *command, int start, int num, char *charset);

/* ...ing */
int				find_pipe(char *str);
int				find_redir(char *str);


void	branch_pipe(t_pipe *p, int *fd, int i, t_list *envs);

#endif
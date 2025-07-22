/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwon <jwon@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/13 17:01:51 by jwon              #+#    #+#             */
/*   Updated: 2020/09/14 12:10:09 by jwon             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "../libs/libft/libft.h"

# define INIT	-1
# define QUOTE	39
# define DQUOTE	34
# define ETC	42
# define REDIR	62
# define DREDIR	6
# define BREDIR	60

int				g_exit_value;
char			**g_envp;
char			*g_env_user;

/**
 * @struct	s_env
 * @brief	Structure for environment variables
 * @param key	Environment variable name
 * @param value	Environment variable value
 */
typedef struct	s_env
{
	char		*key;
	char		*value;
}				t_env;

/**
 * @struct	s_quote
 * @brief	Structure for handling quoted strings
 * @param type	Type of quote (QUOTE, DQUOTE, ETC)
 * @param start	Starting index of quoted content
 * @param end	Ending index of quoted content
 */
typedef struct	s_quote
{
	int			type;
	int			start;
	int			end;
}				t_quote;

/**
 * @struct	s_redir
 * @brief	Structure for handling redirections
 * @param argc	Argument count
 * @param argv	Argument vector
 * @param cmds	Commands to execute
 * @param types	Types of redirections
 */
typedef struct	s_redir
{
	int			argc;
	char		**argv;
	char		**cmds;
	char		*types;
}				t_redir;

/**
 * @struct	s_pipe
 * @brief	Structure for handling pipes
 * @param line	Command line string
 * @param argv	Argument vector
 */
typedef struct	s_pipe
{
	char		*line;
	char		**argv;
}				t_pipe;

/*
**	main.c
*/
/**
 * @brief	Initializes the shell environment
 * @param envs	List of environment variables
 */
void			setup(t_list *envs);

/*
**	control_signal.c
*/
/**
 * @brief	Handles signals (SIGINT, SIGQUIT)
 * @param signo	Signal number
 */
void			handle_signal(int signo);

/*
**	show_prompt_art.c
*/
/**
 * @brief	Reads input from the command prompt
 * @param line	Pointer to store the input line
 * @return	1 if input successful, 0 otherwise
 */
int				input_from_prompt(char **line);

/**
 * @brief	Displays the shell prompt with username
 * @param user	Username to display
 */
void			show_prompt(char *user);

/**
 * @brief	Displays the shell ASCII art
 */
void			show_art(void);

/*
**	get_envs.c
*/
/**
 * @brief	Parses environment variables into a linked list
 * @param argc	Argument count
 * @param argv	Argument vector
 * @param envp	Environment variables
 * @return	Linked list of environment variables
 */
t_list			*get_envs(int argc, char **argv, char **envp);

/*
**	get_cmds.c
*/
/**
 * @brief	Splits a command line into individual commands
 * @param line	Input command line
 * @return	Array of command strings
 */
char			**get_cmds(char *line);

/*
**	get_argv.c
*/
/**
 * @brief	Modifies a string by expanding environment variables
 * @param str	String to modify
 * @param envs	List of environment variables
 * @return	Modified string
 */
char			*modify_argv(char *str, t_list *envs);

/**
 * @brief	Parses a command line into argument vector
 * @param line	Command line
 * @param envs	List of environment variables
 * @return	Argument vector
 */
char			**get_argv(char *line, t_list *envs);

/*
**	utils.c
*/
/**
 * @brief	Prints an error message to the specified file descriptor
 * @param s1	First part of the error message
 * @param s2	Second part of the error message
 * @param fd	File descriptor
 * @return	Always returns 1
 */
int				ft_puterror_fd(char *s1, char *s2, int fd);

/**
 * @brief	Extracts a substring and trims specified characters
 * @param command	Source string
 * @param start	Starting index
 * @param num	Number of characters to extract
 * @param charset	Characters to trim
 * @return	Processed substring
 */
char			*substr_and_trim(char *command, int start, \
				int num, char *charset);

/**
 * @brief	Frees a double array (array of strings)
 * @param arr	Array to free
 */
void			free_double_arr(char **arr);

/*
**	utils_env.c
*/
/**
 * @brief	Checks if a string is a valid environment variable
 * @param arg	String to check
 * @return	1 if valid, 0 otherwise
 */
int				is_valid_env(char *arg);

/**
 * @brief	Checks if an environment variable key exists
 * @param key	Key to check
 * @param envs	List of environment variables
 * @return	1 if exists, 0 otherwise
 */
int				is_exist_key(char *key, t_list *envs);

/**
 * @brief	Finds the value of an environment variable
 * @param key	Environment variable key
 * @param envs	List of environment variables
 * @return	Value of the environment variable, NULL if not found
 */
char			*find_value(char *key, t_list *envs);

/**
 * @brief	Finds the path of an executable
 * @param argv	Command name
 * @param envs	List of environment variables
 * @return	Full path of the executable, NULL if not found
 */
char			*find_path(char *argv, t_list *envs);

/*
**	utils_export.c
*/
/**
 * @brief	Converts a linked list to an array of strings
 * @param lst	Linked list to convert
 * @return	Array of strings
 */
char			**convert_lst_to_arr(t_list *lst);

/**
 * @brief	Sorts an array of strings alphabetically
 * @param arr	Array to sort
 */
void			sort_double_arr(char **arr);

/**
 * @brief	Modifies environment variables for export display
 * @param arr	Array of environment variables
 */
void			modify_env_for_export(char **arr);

/**
 * @brief	Prints an array of strings
 * @param arr	Array to print
 */
void			print_double_arr(char **arr);

/*
**	utils_has.c
*/
/**
 * @brief	Checks if a string contains quotes
 * @param str	String to check
 * @return	1 if contains quotes, 0 otherwise
 */
int				has_quote(char *str);

/**
 * @brief	Checks if a string contains environment variables
 * @param str	String to check
 * @return	1 if contains environment variables, 0 otherwise
 */
int				has_env(char *str);

/**
 * @brief	Checks if a string contains pipes
 * @param str	String to check
 * @return	1 if contains pipes, 0 otherwise
 */
int				has_pipe(char *str);

/**
 * @brief	Checks if a string contains redirections
 * @param str	String to check
 * @return	1 if contains redirections, 0 otherwise
 */
int				has_redir(char *str);

/*
**	utils_redir.c
*/
/**
 * @brief	Checks if a redirection is single (> or <)
 * @param command	Command string
 * @param i	Current index
 * @return	1 if single redirection, 0 otherwise
 */
int				is_single_redir(char *command, int i);

/**
 * @brief	Determines the type of redirection
 * @param command	Command string
 * @param i	Current index
 * @return	Type of redirection (REDIR, DREDIR, BREDIR)
 */
int				find_redir_type(char *command, int i);

/**
 * @brief	Checks for syntax errors in redirection
 * @param str	String to check
 * @return	1 if syntax error, 0 otherwise
 */
int				has_redir_syntax_error(char *str);

/*
**	parse_quote_env.c
*/
/**
 * @brief	Parses and expands environment variables in a string
 * @param argv	String to parse
 * @param envs	List of environment variables
 * @return	Processed string with expanded variables
 */
char			*parse_env(char *argv, t_list *envs);

/**
 * @brief	Parses quoted strings and handles their content
 * @param argv	String to parse
 * @param envs	List of environment variables
 * @return	Processed string with quotes handled
 */
char			*parse_quote(char *argv, t_list *envs);

/*
**	cmd_pwd_cd_echo.c
*/
/**
 * @brief	Implements the pwd built-in command
 */
void			cmd_pwd(void);

/**
 * @brief	Implements the cd built-in command
 * @param argv	Argument vector
 * @param envs	List of environment variables
 */
void			cmd_cd(char **argv, t_list *envs);

/**
 * @brief	Implements the echo built-in command
 * @param argv	Argument vector
 */
void			cmd_echo(char **argv);

/*
**	cmd_exit.c
*/
/**
 * @brief	Implements the exit built-in command
 * @param argv	Argument vector
 */
void			cmd_exit(char **argv);

/*
**	cmd_env.c
*/
/**
 * @brief	Implements the env built-in command
 * @param argv	Argument vector
 * @param envs	List of environment variables
 */
void			cmd_env(char **argv, t_list *envs);

/*
**	cmd_export.c
*/
/**
 * @brief	Implements the export built-in command
 * @param argv	Argument vector
 * @param envs	List of environment variables
 */
void			cmd_export(char **argv, t_list *envs);

/*
**	cmd_unset.c
*/
/**
 * @brief	Implements the unset built-in command
 * @param argv	Argument vector
 * @param envs	List of environment variables
 */
void			cmd_unset(char **argv, t_list *envs);

/*
**	exec_redir.c
*/
/**
 * @brief	Initializes redirection structure
 * @param command	Command string
 * @param r	Redirection structure
 */
void			init_redir(char *command, t_redir *r);

/**
 * @brief	Parses redirections in a command
 * @param command	Command string
 * @param r	Redirection structure
 * @return	1 if successful, 0 otherwise
 */
int				parse_redir(char *command, t_redir *r);

/**
 * @brief	Opens files that are not used for redirection
 * @param r	Redirection structure
 */
void			open_unnecessary_files(t_redir *r);

/**
 * @brief	Executes a command with redirections
 * @param line	Command line
 * @param envs	List of environment variables
 */
void			exec_redir(char *line, t_list *envs);

/*
**	cmd_redir.c
*/
/**
 * @brief	Handles command execution with redirections
 * @param r	Redirection structure
 * @param envs	List of environment variables
 */
void			cmd_redir(t_redir *r, t_list *envs);

/*
**	exec_pipe.c
*/
/**
 * @brief	Parses pipe commands
 * @param line	Command line
 * @param p	Pipe structure
 * @param envs	List of environment variables
 */
void			parse_pipe(char **line, t_pipe *p, t_list *envs);

/**
 * @brief	Executes a command with pipes
 * @param line	Command line
 * @param envs	List of environment variables
 */
void			exec_pipe(char *line, t_list *envs);

/*
**	exec_cmds.c
*/
/**
 * @brief	Executes a built-in command
 * @param line	Command line
 * @param envs	List of environment variables
 * @return	1 if command is a built-in, 0 otherwise
 */
int				exec_builtin(char *line, t_list *envs);

/**
 * @brief	Executes an external command
 * @param line	Command line
 * @param envs	List of environment variables
 */
void			exec_others(char *line, t_list *envs);

/**
 * @brief	Executes a command (pipes, redirections, builtins, or external)
 * @param line	Command line
 * @param envs	List of environment variables
 */
void			exec_cmds(char *line, t_list *envs);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:26:37 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/26 12:54:09 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/src/libft.h"
/* readline printf perror */
# include <stdio.h>
/* malloc free exit getenv */
# include <stdlib.h>
/* write access read close fork getcwd chdir unlink execve dup dup2 pipe isatty
 ttyname ttyslot*/
# include <unistd.h>
/* wait waitpid wait3 wait4 */
# include <sys/wait.h>
/* open */
# include <fcntl.h>
/* signal sigaction sigemptyset sigaddset kill */
# include <signal.h>
/* strerror */
# include <string.h>
/* errno */
# include <errno.h>
/* stat lstat fstat */
# include <sys/stat.h>
/* readline  rl_clear_history rl_on_new_line rl_replace_line rl_redisplay */
# include <readline/readline.h>
/* readline add_history */
# include <readline/history.h>
/* opendir readdir closedir */
# include <dirent.h>
/* opendir closedir */
# include <sys/types.h>
/* ioctl */
# include <sys/ioctl.h>
/* tcsetattr tcgetattr */
# include <termios.h>
/* tgetent tgetflag tgetnum tgetstr tgoto tputs */
# include <curses.h>
/* tgetent tgetflag tgetnum tgetstr tgoto tputs */
# include <term.h>

//# ifndef EXIT_CODE_VAR
//#  define EXIT_CODE_VAR

int	g_exit_code;

//# endif

// tree.c
typedef struct s_node
{
	char			*operator;
	char			*operand;
	struct s_node	*left;
	struct s_node	*right;
}					t_node;

t_node				*create_node(char *operator, char *operand);
void				destroy_node(t_node *t_node);
t_node				*create_operator_node(char *operator);
t_node				*create_operand_node(char *operand);
t_node				*generate_node_from_command(const char *command);

// dev_utils.c
void				print_ast(t_node *root, int indent);

// execute_tree.c
int					execute_command(const char *command);
int					execute_node(t_node *node, char ***env, t_node *root);
int					execute_operator_node(t_node *node, int left_status,
						char ***env, t_node *root);
int					execute_operand_node(t_node *node, char ***env, t_node *root);

/**
 * Structure that holds the information about redirections.
 *
 * @param reout 	0 if output not redirected, 1 if output redirected
 * @param reoutap 	0 if output not redirected in append mode, 1 if output
 *  redirected in append mode
 * @param outfile 	path to outfile
 */
typedef struct s_reout
{
	int				reout;
	int				reoutapp;
	char			*outfile;
}					t_reout;

/**
 * Structure that holds the information about redirections.
 *
 * @param rein 		0 if input not redirected, 1 if input redirected
 * @param herein 	0 if input not from given lines, 1 if input from given
 *  line, i.e. if <<
 * @param heredel 	delimiter of << given right after
 * @param infile 	path to infile
 */
typedef struct s_rein
{
	int				rein;
	int				herein;
	char			*heredel;
	char			*infile;
}					t_rein;

/**
 * Structure that holds the command with it arguments and a redirection
 * structure.
 *
 * @param redir		a redirection structure
 * @param cmdarg	a string containing the command as first word and then its
 *  arguments.
 */
typedef struct s_cmd
{
	t_rein			rein;
	t_reout			reout;
	char			*cmdarg;

}					t_cmd;

typedef struct s_fullcmd
{
	t_cmd			*cmds;
	int				cmdct;
	char			**argums;
	int				**fds;
}					t_fullcmd;

void				ft_freesplit(char **arrstr);
void				ft_frfds(int **fds, int i);
void				ft_frall(char **arrstr, int **fds, int i);

char				**ft_appendtoarr(char **arr, char **app);
char				**ft_getstring(char **str, char **arrstr);
char				**ft_splitnotstr(char *str, char c);

char				*ft_strjoinfree(char *begin, char *end, int num);

int					**ft_execfdpairs(int cmdct);
char	*ft_execgetpathname(char *cmd, char ***env);
char	**ft_execargums(char *cmdarg, char ***env);
int					ft_execheredoc(char *delim);
int					ft_execcloseall(int **fd, int pipect);
int					ft_execchildproc(t_cmd *cmds, t_fullcmd fullcmd, int i,
						char ***env);
int					ft_execparentproc(t_fullcmd fullcmd, int childpid, int *i);
int					ft_executer(t_cmd *fullcmds, int cmdct, char ***env);

int					ft_min(int a, int b);
char				*ft_pathname(char *cmd, char **paths);

int					ft_arrlen(char **arr);
int					ft_findchar(char *str, char c);
void				ft_freearr(char **arr);
void				ft_freearr2(char **arr, char **newarr);
char				**ft_arrapp(char **arr, char *app);
char				**ft_separatepipes(char *block);
char				*ft_getname(char *line, int *len);
char				*straddfree(char *str, char chr);
char				*ft_get_cmdarg(char *line);
t_cmd				*ft_putinstruct(char **blocksep);
int					ft_parseandexec(char *line, char ***env, t_node *root);

int					ft_echo(t_fullcmd fullcmd);
int					ft_pwd(char **env);

int					ft_export(t_fullcmd fullcmd, char ***env);
int					ft_env(t_fullcmd fullcmd, char ***env);
int					ft_unset(t_fullcmd fullcmd, char ***env);
int					ft_cd(t_fullcmd fullcmd, char ***env);
void ft_exit(char *cmd, char ***env);

int					ft_findcharout(char *str, char c);
void				ft_freecmds(t_cmd *cmds, int n);
char				*ft_getenv(char *var, char **env);
char				*ft_replace(char *line, char *varname, int *pos, char ***env);
char				*ft_getvar_and_replace(int *i, char *expline, char ***env);
char				*ft_expandvar(char *line, char ***env);
char				**ft_expandasterisk(char **pattern_pieces, int *ast);
char				**ft_pattern_pieces(char *str, int *ast);
char				*ft_getstr(char *line, int *len);
char				*ft_strip_quotes(char *str);
int					ft_existenv(char *var, char **env);
char				**ft_arrremove(char **env, int pos);

#endif

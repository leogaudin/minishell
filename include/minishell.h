/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:26:37 by lgaudin           #+#    #+#             */
/*   Updated: 2023/08/10 10:07:12 by ysmeding         ###   ########.fr       */
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

int	g_exit_code;

// tree.c
typedef struct s_node
{
	char			*operator;
	char			*operand;
	struct s_node	*left;
	struct s_node	*right;
}					t_node;

t_node		*create_node(char *operator, char *operand);
void		destroy_node(t_node *t_node);
t_node		*create_operator_node(char *operator);
t_node		*create_operand_node(char *operand);
t_node		*generate_node_from_command(const char *command);

// dev_utils.c
void		print_ast(t_node *root, int indent);

// execute_tree.c
int			execute_command(const char *command);
int			execute_node(t_node *node, char ***env, t_node *root);
int			execute_operator_node(t_node *node, int left_status,
				char ***env, t_node *root);
int			execute_operand_node(t_node *node, char ***env, t_node *root);

/**
 * Structure that holds the information about redirections.
 *
 * @param reout 	0 if output not redirected, 1 if output redirected
 * @param reoutapp 	0 if output not redirected in append mode, 1 if output
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
	int				*childpid;
}					t_fullcmd;

typedef struct s_open_quote
{
	int	opens;
	int	opend;
}		t_openq;

/* ------------------------------ errorfuncs.c ------------------------------ */

void		ft_putstrerror(char *str);
void		ft_puterror(char *error, int code, char *str1, char *str2);

/* ------------------------------- executer.c ------------------------------- */

void		ft_closepipeends(t_fullcmd fullcmd, int *i);
int			ft_executeempty(t_fullcmd fullcmd, int *i);
int			ft_findexecmethod(t_fullcmd fullcmd, int *i, char ***env);
int			ft_executer_loops(t_fullcmd	fullcmd, int cmdct, char ***env);
int			ft_executer(t_cmd *cmds, int cmdct, char ***env);

/* ------------------------- expandasterisk_utils.c ------------------------- */

char		**ft_check_empty_pattern(char **pattern_pieces);
void		ft_set_ast(char *str, int **ast);

/* ---------------------------- expandasterisk.c ---------------------------- */

int			ft_patternfound(char *filename, char **pattern_pieces, int *ast);
char		**ft_addfilename_to_arr(char **files, char *filename, DIR *dirp);
char		**ft_expandasterisk(char **pattern_pieces, int *ast);
int			ft_getlenast(char *str, int i);
char		**ft_pattern_pieces(char *str, int *ast);

/* ------------------------------- expandvar.c ------------------------------ */

char		*ft_replace(char *line, char *varname, int *pos, char ***env);
char		*ft_replacewave(char *line, int *pos, char ***env);
char		*ft_getvar_and_replace(int *i, char *expline, char ***env);
int			ft_expandvar_loopfunc(char **expline, int *i, t_openq *open,
				char ***env);
char		*ft_expandvar(char *line, char ***env);

/* ----------------------------- ft_arr_utils.c ----------------------------- */

int			ft_arrlen(char **arr);
char		**ft_arrapp(char **arr, char *app);
char		**ft_arrremove(char **env, int pos);
char		**ft_appendtoarr(char **arr, char **app);

/* ----------------------------- ft_builtin_cd.c ---------------------------- */

void		determine_path(char **path, char *home, t_fullcmd fullcmd,
				char ***env);
int			change_path(char *path, t_fullcmd fullcmd);
int			update_pwd(char *path, char *oldpath, char ***env);
int			ft_cd(t_fullcmd fullcmd, char ***env);

/* ---------------------------- ft_builtin_echo.c --------------------------- */

int			ft_echo(t_fullcmd fullcmd);

/* ---------------------------- ft_builtin_env.c ---------------------------- */

int			ft_env(t_fullcmd fullcmd, char ***env);

/* ---------------------------- ft_builtin_exit.c --------------------------- */

void		ft_exitwitharg(char **argums);
void		ft_exit(char *cmd, char ***env);

/* --------------------------- ft_builtin_export.c -------------------------- */

void		ft_exportnoarg(char ***env);
int			ft_export_checkname(t_fullcmd fullcmd, int i, int *len);
int			ft_export_replaceoraddvar(t_fullcmd fullcmd, char ***env, int i,
				int len);
int			ft_export(t_fullcmd fullcmd, char ***env);

/* ---------------------------- ft_builtin_pwd.c ---------------------------- */

int			ft_pwd(char **env);

/* --------------------------- ft_builtin_unset.c --------------------------- */

int			ft_unset_checkname(t_fullcmd fullcmd, int i, int *len);
int			ft_unset(t_fullcmd fullcmd, char ***env);

/* ----------------------------- ft_env_utils.c ----------------------------- */

char		*ft_getquestionmark(void);
char		*ft_getenv(char *var, char **env);
int			ft_existenv(char *var, char **env);

/* ------------------------- ft_execute_getargums.c ------------------------- */

int			ft_getlen_str(char *str, int i);
char		**get_exp(char **cmd_parts, char *part);
char		**get_expand_noquote_str(char **cmd_parts, char *cmdarg, int i,
				int len);
char		**ft_splitcmdline(char *cmdarg);
char		**ft_execargums(char *cmdarg, char ***env);

/* -------------------------- ft_execute_methods.c -------------------------- */

int			ft_choose_builtin(t_fullcmd fullcmd, char ***env);
int			ft_execute(t_cmd *cmds, t_fullcmd fullcmd, int i, char ***env);
int			ft_executebuiltin(t_fullcmd fullcmd, int *i, char ***env);
int			ft_execchildproc(t_cmd *cmds, t_fullcmd fullcmd, int i,
				char ***env);
int			ft_execparentproc(t_fullcmd	fullcmd, int childpid, int *i);

/* ----------------------- ft_execute_redirections.c ----------------------- */

int			ft_execheredoc(char *delim);
int			ft_exec_redirin(t_cmd *cmds, t_fullcmd fullcmd, int i, int fdin);
int			ft_exec_redirout(t_cmd *cmds, t_fullcmd fullcmd, int i, int fdo);
int			ft_exec_redir(t_cmd *cmds, t_fullcmd fullcmd, int i);

/* --------------------------- ft_execute_utils.c --------------------------- */

int			**ft_execfdpairs(int cmdct);
int			ft_isbuiltin(char *cmd);
int			ft_execcloseall(int **fd, int pipect);

/* ----------------------------- ft_freefuncs.c ----------------------------- */

void		ft_freesplit(char **arrstr);
void		ft_frfds(int **fds, int i);
void		ft_frall(char **arrstr, int **fds, int i);
void		ft_freecmds(t_cmd *cmds, int n);

/* ---------------------------- ft_getpathname.c ---------------------------- */

int			ft_min(int a, int b);
char		*ft_pathname(char *cmd, char **paths, int acc);
char		*ft_execpathname2(char *cmd, char **paths, char *pathvar, int off);
char		*ft_execgetpathname(char *cmd, char ***env);

/* ----------------------- ft_parse_checkredir_utils.c ---------------------- */

char		*ft_namefile_complete(char **asterisk_exp_arr);
char		*ft_namefile(char *name);
int			ft_open_notlast_heredoc(char *delim, char *pipe_block, int j);
int			ft_check_heredoc_or_infile(int here, char **name, char *pipe_block,
				int *j);

/* -------------------------- ft_parse_checkredir.c ------------------------- */

int			ft_checkinfile(char *name);
int			ft_check_redirin(char *pipe_block, int *j);
int			ft_checkoutfile(char *name, int app);
int			ft_check_redirout(char *pipe_block, int *j);
int			ft_check_redir_inandout(char **separate_pipe, char	***env);

/* ------------------------ ft_parse_separatepipes.c ------------------------ */

int			ft_getlen_pipe(char *block, int i);
char		**ft_separatepipes(char *block);

/* ----------------------- ft_parse_structcmds_utils.c ---------------------- */

char		*ft_addwhilenotchar(char *line, char *name, int *len, char c);
char		*ft_getstr(char *line, int *len);
void		ft_skip_name(char *line, int *i);
char		*ft_get_cmdarg(char *line);
t_cmd		*ft_initcmds(char **blocksep, t_cmd *cmds);

/* -------------------------- ft_parse_structcmds.c ------------------------- */

void		ft_skip_str(char **blocksep, int i, int *j);
char		*ft_putinstruct_in(t_cmd *cmd, char **blocksep, int i, int *j);
char		*ft_putinstruct_out(t_cmd *cmd, char **blocksep, int i, int *j);
int			ft_putinstruct_block(t_cmd **cmds, char **blocksep, int i);
t_cmd		*ft_putinstruct(char **blocksep);

/* ---------------------------- ft_parse_utils.c ---------------------------- */

int			ft_findchar(char *str, char c);
int			ft_findcharout(char *str, char c);
char		*ft_straddfree(char *str, char chr);
void		ft_freearr(char **arr);
void		ft_freearr2(char **arr, char **newarr);

/* ------------------------------- ft_parse.c ------------------------------- */

int			check_first_last_char(char *line);
char		*ft_get_pipe_block(char *line, int i);
char		**ft_separate_pipe(char *line);
void		ft_check_exit(char **separate_pipe, char ***env, t_node *root);
int			ft_parseandexec(char *line, char ***env, t_node *root);

/* ---------------------------- ft_strjoinfree.c ---------------------------- */

char		*ft_strjoinfree(char *begin, char *end, int num);

#endif

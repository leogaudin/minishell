/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:26:37 by lgaudin           #+#    #+#             */
/*   Updated: 2023/08/19 12:30:05 by ysmeding         ###   ########.fr       */
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

typedef struct s_gen_info
{
	char	**env;
	int		exit_code;
	t_node	*root;
	char	*prompt;
}			t_gen_info;

t_node		*create_node(char *operator, char *operand);
void		destroy_node(t_node *t_node);
t_node		*create_operator_node(char *operator);
t_node		*create_operand_node(char *operand);
t_node		*generate_node_from_command(const char *command);

// dev_utils.c
void		print_ast(t_node *root, int indent);

// execute_tree.c
int			execute_command(const char *command);
int			execute_node(t_node *node, t_gen_info *info);
int			execute_operator_node(t_node *node, int left_status,
				t_gen_info *info);
int			execute_operand_node(t_node *node, t_gen_info *info);

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

void		ft_putstrerror(char *str, t_gen_info *info);
void		ft_puterror(char *error, int code, char *str1, char *str2);
int			ft_put_error(char *error, int code, char *str1, char *str2);
void		ftme(t_gen_info *info);

/* ------------------------------- executer.c ------------------------------- */

void		ft_closepipeends(t_fullcmd fullcmd, int *i);
int			ft_executeempty(t_fullcmd fullcmd, int *i, t_gen_info *info);
int			ft_findexecmethod(t_fullcmd fullcmd, int *i, t_gen_info *info);
int			ft_executer_loops(t_fullcmd	fullcmd, int cmdct, t_gen_info *info);
int			ft_executer(t_cmd *cmds, int cmdct, t_gen_info *info);

/* ------------------------- expandasterisk_utils.c ------------------------- */

char		**ft_check_empty_pattern(char **pattern_pieces, t_gen_info *info);
void		ft_set_ast(char *str, int **ast);

/* ---------------------------- expandasterisk.c ---------------------------- */

int			ft_patternfound(char *filename, char **pattern_pieces, int *ast);
char		**ft_addfile_arr(char **files, char *filename, DIR *dirp,
				t_gen_info *info);
char		**ft_expandasterisk(char **pattern_pieces, int *ast,
				t_gen_info *info);
int			ft_getlenast(char *str, int i);
char		**ft_pattern_pieces(char *str, int *ast, t_gen_info *info);

/* ------------------------------- expandvar.c ------------------------------ */

char		*ft_replace(char *line, char *varname, int *pos, t_gen_info *info);
char		*ft_replacewave(char *line, int *pos, t_gen_info *info);
char		*ft_getvar_and_replace(int *i, char *expline, t_gen_info *info);
int			ft_expandvar_loopfunc(char **expline, int *i, t_openq *open,
				t_gen_info *info);
char		*ft_expandvar(char *line, t_gen_info *info);

/* ----------------------------- ft_arr_utils.c ----------------------------- */

int			ft_arrlen(char **arr);
char		**ft_arrapp(char **arr, char *app, t_gen_info *info);
char		**ft_arrremove(char **env, int pos, t_gen_info *info);
char		**ft_appendtoarr(char **arr, char **app, t_gen_info *info);

/* ----------------------------- ft_builtin_cd.c ---------------------------- */

void		determine_path(char **path, char *home, t_fullcmd fullcmd,
				t_gen_info *info);
int			change_path(char *path, t_fullcmd fullcmd, t_gen_info *info);
int			update_pwd(char *path, char *oldpath, t_gen_info *info);
int			ft_cd(t_fullcmd fullcmd, t_gen_info *info);

/* ---------------------------- ft_builtin_echo.c --------------------------- */

int			ft_echo(t_fullcmd fullcmd, t_gen_info *info);

/* ---------------------------- ft_builtin_env.c ---------------------------- */

int			ft_env(t_fullcmd fullcmd, t_gen_info *info);

/* ---------------------------- ft_builtin_exit.c --------------------------- */

void		ft_exitwitharg(char **argums, t_gen_info *info);
void		ft_exit(char *cmd, t_gen_info *info);

/* --------------------------- ft_builtin_export.c -------------------------- */

void		ft_exportnoarg(t_gen_info *info);
int			ft_export_checkname(t_fullcmd fullcmd, int i, int *len,
				t_gen_info *info);
int			ft_export_replaceaddvar(t_fullcmd fullcmd, t_gen_info *info, int i,
				int len);
int			ft_export(t_fullcmd fullcmd, t_gen_info *info);

/* ---------------------------- ft_builtin_pwd.c ---------------------------- */

int			ft_pwd(t_gen_info *info);

/* --------------------------- ft_builtin_unset.c --------------------------- */

int			ft_unset_checkname(t_fullcmd fullcmd, int i, int *len,
				t_gen_info *info);
int			ft_unset(t_fullcmd fullcmd, t_gen_info *info);

/* ----------------------------- ft_env_utils.c ----------------------------- */

char		*ft_getquestionmark(t_gen_info *info);
char		*ft_getenv(char *var, t_gen_info *info);
int			ft_existenv(char *var, t_gen_info *info);

/* ------------------------- ft_execute_getargums.c ------------------------- */

int			ft_getlen_str(char *str, int i);
char		**get_exp(char **cmd_parts, char *part, t_gen_info *info);
char		**ft_exp_noq(char **cmd_parts, char *cmdarg, int len,
				t_gen_info *info);
char		**ft_splitcmdline(char *cmdarg, t_gen_info *info);
char		**ft_execargums(char *cmdarg, t_gen_info *info);

/* -------------------------- ft_execute_methods.c -------------------------- */

int			ft_choose_builtin(t_fullcmd fullcmd, t_gen_info *info);
int			ft_execute(t_cmd *cmds, t_fullcmd fullcmd, int i, t_gen_info *info);
int			ft_executebuiltin(t_fullcmd fullcmd, int *i, t_gen_info *info);
int			ft_execchildproc(t_cmd *cmds, t_fullcmd fullcmd, int i,
				t_gen_info *info);
int			ft_execparentproc(t_fullcmd	fullcmd, int childpid, int *i,
				t_gen_info *info);

/* ----------------------- ft_execute_redirections.c ----------------------- */

int			ft_execheredoc(char *delim, t_gen_info *info);
int			ft_exec_redirin(t_cmd *cmds, t_fullcmd fullcmd, int i,
				t_gen_info *info);
int			ft_dupout(int fdout, t_gen_info *info);
int			ft_exec_redirout(t_cmd *cmds, t_fullcmd fullcmd, int i,
				t_gen_info *info);
int			ft_exec_redir(t_cmd *cmds, t_fullcmd fullcmd, int i,
				t_gen_info *info);

/* --------------------------- ft_execute_utils.c --------------------------- */

int			**ft_execfdpairs(int cmdct, t_gen_info *info);
int			ft_isbuiltin(char *cmd);
int			ft_execcloseall(int **fd, int pipect);
int			fd_dupin(int fdin, t_gen_info *info);
void		ft_exportlastarg(char **argums, t_gen_info *info);

/* ----------------------------- ft_freefuncs.c ----------------------------- */

void		ft_freesplit(char **arrstr);
void		ft_frfds(int **fds, int i);
void		ft_frall(char **arrstr, int **fds, int i);
void		ft_freecmds(t_cmd *cmds, int n);

/* ---------------------------- ft_getpathname.c ---------------------------- */

int			ft_min(int a, int b);
char		*ft_pathname(char *cmd, char **paths, int acc, t_gen_info *info);
char		*ft_execpathname2(char *cmd, char **paths, int off,
				t_gen_info *info);
char		*ft_execgetpathname(char *cmd, t_gen_info *info);

/* ----------------------- ft_parse_checkredir_utils.c ---------------------- */

char		*ft_namefile_complete(char **asterisk_exp_arr, t_gen_info *info);
char		*ft_namefile(char *name, t_gen_info *info);
int			ft_open_notlast_heredoc(char *delim, char *pipe_block,
				t_gen_info *info);
int			ft_check_hereorin(int here, char **name, char *pipe_block,
				t_gen_info *info);

/* -------------------------- ft_parse_checkredir.c ------------------------- */

int			ft_checkinfile(char *name, t_gen_info *info);
int			ft_check_redirin(char *pipe_block, int *j, t_gen_info *info);
int			ft_checkoutfile(char *name, int app, t_gen_info *info);
int			ft_check_redirout(char *pipe_block, int *j, t_gen_info *info);
int			ft_check_redir_inandout(char **separate_pipe, t_gen_info *info);

/* ------------------------ ft_parse_separatepipes.c ------------------------ */

int			ft_getlen_pipe(char *block, int i);
char		**ft_separatepipes(char *block, t_gen_info *info);
int			ft_check_redir_inandout_loop(char **block, t_gen_info *info);
void		ft_get_chars(char *line, int *i, char **new, t_gen_info *info);

/* ----------------------- ft_parse_structcmds_utils.c ---------------------- */

int			ft_addwhilenotchar(char *line, char **name, char c,
				t_gen_info *info);
char		*ft_getstr(char *line, int *len, t_gen_info *info);
void		ft_skip_name(char *line, int *i);
char		*ft_get_cmdarg(char *line, t_gen_info *info);
t_cmd		*ft_initcmds(char **blocksep, t_cmd *cmds, t_gen_info *info);

/* -------------------------- ft_parse_structcmds.c ------------------------- */

void		ft_skip_str(char **blocksep, int i, int *j);
char		*ft_putinstruct_in(t_cmd *cmd, char *blocksep, int *j,
				t_gen_info *info);
char		*ft_putinstruct_out(t_cmd *cmd, char *blocksep, int *j,
				t_gen_info *info);
int			ft_putinstruct_block(t_cmd **cmds, char **blocksep, int i,
				t_gen_info *info);
t_cmd		*ft_putinstruct(char **blocksep, t_gen_info *info);

/* ---------------------------- ft_parse_utils.c ---------------------------- */

int			ft_findchar(char *str, char c);
int			ft_findcharout(char *str, char c);
char		*ft_straddfree(char *str, char chr, t_gen_info *info);
void		ft_freearr(char **arr);
void		ft_freearr2(char **arr, char **newarr);

/* ------------------------------- ft_parse.c ------------------------------- */

int			check_first_last_char(char *line, t_gen_info *info);
char		*ft_get_pipe_block(char *line, int i);
char		**ft_separate_pipe(char *line, t_gen_info *info);
int			ft_check_exit(char **separate_pipe, t_gen_info *info);
int			ft_parseandexec(char *line, t_gen_info *info);

/* ---------------------------- ft_strjoinfree.c ---------------------------- */

char		*ft_strjoinfree(char *begin, char *end, int num);

#endif

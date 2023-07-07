#ifndef TEMP_H
# define TEMP_H

#include "minishell.h"


void	ft_freesplit(char **arrstr);
void	ft_frfds(int **fds, int i);
void	ft_frall(char **arrstr, int **fds, int i);

char	**ft_appendtoarr(char **arr, char **app);
char	**ft_getstring(char **str, char **arrstr);
char	**ft_splitnotstr(char *str, char c);

char	*ft_strjoinfree(char *begin, char *end, int num);

int **ft_execfdpairs(int cmdct);
char	*ft_execgetpathname(char *cmd, char **envp);
char	**ft_execargums(char *cmdarg, char **env);
int	ft_execheredoc(char *delim);
void	ft_execcloseall(int **fd, int pipect);
void	ft_execchildproc(t_cmd *fullcmds, char **argums, int **fds, char **envp, int i, int cmdct);
void	ft_execparentproc(t_cmd *fullcmds, char **argums, int **fds, int childpid, int *i);
int	ft_executer(t_cmd *fullcmds, int cmdct, char **env);

int	ft_min(int a, int b);
char	*ft_pathname(char *cmd, char **paths);

int	ft_arrlen(char **arr);
int	ft_findchar(char *str, char c);
void	ft_freearr(char **arr);
void	ft_freearr2(char **arr, char **newarr);
char	**ft_arrapp(char **arr, char *app);
char	**ft_separatepipes(char *block);
char	*ft_getname(char *line, int *len);
char *straddfree(char *str, char chr);
char *ft_get_cmdarg(char *line);
t_cmd *ft_putinstruct(char **blocksep);
void	ft_parseexec(char *line, char **env);

#endif
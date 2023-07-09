/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 11:52:45 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/09 09:38:33 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int **ft_execfdpairs(int cmdct)
{
	int	i;
	int	**fds;

	fds = malloc((cmdct + 1) * sizeof(int *));
	if (fds)
	{
		i = 0;
		while (i <= cmdct)
		{
			fds[i] = malloc(2 * sizeof(int));
			if (!fds[i])
				return (ft_frfds(fds, i), NULL); //ft_frfds is in pipex files!!
			i++;
		}
	}
	return (fds);
}

char	*ft_execgetpathname(char *cmd, char **envp)
{
	int		i;
	char	**paths;
	char	*cmdpath;

	i = 0;
	while (strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	cmd = ft_strjoinfree("/", cmd, 2);
	paths = ft_split(envp[i] + 5, ':');
	cmdpath = ft_pathname(cmd, paths);
	if (cmdpath == NULL)
	{
		ft_printf("./pipex: %s: command not found\n", cmd + 1);
		free (cmd);
		ft_freesplit(paths);
		return (strdup(""));
	}
	free (cmd);
	ft_freesplit(paths);
	return (cmdpath);
}

char	**ft_execargums(char *cmdarg, char **env)
{
	char	**argums;

	argums = ft_splitnotstr(cmdarg, ' ');
	argums[0] = ft_execgetpathname(argums[0], env);
	return (argums);
}

int	ft_execheredoc(char *delim)
{
	int		fdpair[2];
	int		same;
	char	*line;

	if (pipe(fdpair) < 0)
		return (-1);
	same = 1;
	while (same != 0)
	{
		ft_printf("pipe heredoc> "); //change this maybe??
		line = get_next_line(0);
		same = ft_strncmp(delim, line, ft_strlen(line) - 1);
		if (same != 0)
			write(fdpair[1], line, ft_strlen(line));
		free(line);
	}
	close(fdpair[1]);
	return (fdpair[0]);
}

void	ft_execcloseall(int **fd, int pipect)
{
	int	i;

	i = 0;
	while (i <= pipect)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
}

void	ft_execchildproc(t_cmd *fullcmds, char **argums, int **fds, char **envp, int i, int cmdct)
{
	//add error management everywhere
	int fdin;
	int fdout;
	if (fullcmds[i].rein.rein == 1)
	{
		fdin = open(fullcmds[i].rein.infile, O_RDONLY);
		dup2(fdin, STDIN_FILENO);
	}
	else if (fullcmds[i].rein.herein == 1)
	{
		fdin = ft_execheredoc(fullcmds[i].rein.heredel);
		dup2(fdin, STDIN_FILENO);
	}
	else if (i > 0)
	{
		dup2(fds[i][0], STDIN_FILENO);
	}
	if (fullcmds[i].reout.reout == 1)
	{
		fdout = open(fullcmds[i].reout.outfile, O_WRONLY | O_CREAT, 0644);
		dup2(fdout, STDOUT_FILENO);
	}
	else if (fullcmds[i].reout.reoutapp == 1)
	{
		fdout = open(fullcmds[i].reout.outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		dup2(fdout, STDOUT_FILENO);
	}
	else if (i < cmdct - 1)
	{
		dup2(fds[i + 1][1], STDOUT_FILENO);
	}
	ft_execcloseall(fds, cmdct);
	execve(argums[0], argums, envp);
}

void	ft_execparentproc(t_cmd *fullcmds, char **argums, int **fds, int childpid, int *i)
{
	(void)fullcmds;
	close(fds[*i][0]);
	close(fds[*i + 1][1]);
	waitpid(childpid, NULL, 0);
	ft_freesplit(argums);
	(*i)++;
}

int	ft_executer(t_cmd *fullcmds, int cmdct, char **env)
{
	int		childpid;
	int		**fds;
	char	**argums;
	int		i;

	fds = ft_execfdpairs(cmdct);
	if (!fds)
		return (-1); //give error message as well!!, maybe should be 0
	i = 0;
	while (i <= cmdct)
	{
		if (pipe(fds[i]) < 0)
			return (ft_frfds(fds, cmdct + 1), -1); //idem!!
		i++;
	}
	i = 0;
	while (i < cmdct)
	{
		argums = ft_execargums(fullcmds[i].cmdarg, env);
		childpid = fork();
		if (childpid == -1)
			return (-1); //idem!!
		if (childpid == 0)
			ft_execchildproc(fullcmds, argums, fds, env, i, cmdct);
		else
			ft_execparentproc(fullcmds, argums, fds, childpid, &i);
	}
	return (0);
}

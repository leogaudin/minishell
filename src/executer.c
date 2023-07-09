/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 11:52:45 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/09 13:03:38 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	**ft_execfdpairs(int cmdct)
{
	int	i;
	int	**fds;

	fds = malloc((cmdct + 1) * sizeof(int *));
	if (!fds)
	{
		ft_putendl_fd("Memory allocation failed\n", STDERR_FILENO);
		return (NULL);
	}
	if (fds)
	{
		i = 0;
		while (i <= cmdct)
		{
			fds[i] = malloc(2 * sizeof(int));
			if (!fds[i])
			{
				ft_putendl_fd("Memory allocation failed\n", STDERR_FILENO);
				return (ft_frfds(fds, i), NULL);
			}
			i++;
		}
	}
	return (fds);
}

char	*ft_execgetpathname(char *cmd)
{
	int		i;
	char	**paths;
	char	*cmdpath;

	i = 0;
	cmd = ft_strjoinfree("/", cmd, 2);
	paths = ft_split(getenv("PATH"), ':');
	cmdpath = ft_pathname(cmd, paths);
	if (cmdpath == NULL)
	{
		ft_printf("./pipex: %s: command not found\n", cmd + 1);//change error message
		free (cmd);
		ft_freesplit(paths);
		return (strdup(""));
	}
	free (cmd);
	ft_freesplit(paths);
	return (cmdpath);
}

char	**ft_execargums(char *cmdarg)
{
	char	**argums;

	argums = ft_splitnotstr(cmdarg, ' ');
	argums[0] = ft_execgetpathname(argums[0]);
	return (argums);
}

int	ft_execheredoc(char *delim)
{
	int		fdpair[2];
	int		same;
	char	*line;

	if (pipe(fdpair) < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	same = 1;
	while (same != 0)
	{
		ft_printf("here_doc > "); //change this maybe??
		line = get_next_line(0);
		same = ft_strncmp(delim, line, ft_strlen(line) - 1);//are strlen and strncmp protected for null arg?
		if (same != 0)
			write(fdpair[1], line, ft_strlen(line));//is strlen protected for null arg?
		free(line);
	}
	close(fdpair[1]);
	return (fdpair[0]);
}

int	ft_execcloseall(int **fd, int pipect)
{
	int	i;

	i = 0;
	while (i <= pipect)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
	return (0);
}

int	ft_execchildproc(t_cmd *fullcmds, char **argums, int **fds, int i, int cmdct)
{
	int fdin;
	int fdout;
	if (fullcmds[i].rein.rein == 1)
	{
		fdin = open(fullcmds[i].rein.infile, O_RDONLY);
		if (fdin < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (dup2(fdin, STDIN_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	else if (fullcmds[i].rein.herein == 1)
	{
		fdin = ft_execheredoc(fullcmds[i].rein.heredel);
		if (fdin < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (dup2(fdin, STDIN_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	else if (i > 0)
	{
		if (dup2(fds[i][0], STDIN_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	if (fullcmds[i].reout.reout == 1)
	{
		if (access(fullcmds[i].reout.outfile, F_OK) == 0)
		{
			if (unlink(fullcmds[i].reout.outfile) < 0)
				return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		}
		fdout = open(fullcmds[i].reout.outfile, O_WRONLY | O_CREAT, 0644);
		if (fdout < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (dup2(fdout, STDOUT_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	else if (fullcmds[i].reout.reoutapp == 1)
	{
		fdout = open(fullcmds[i].reout.outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fdout < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (dup2(fdout, STDOUT_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	else if (i < cmdct - 1)
	{
		if (dup2(fds[i + 1][1], STDOUT_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	ft_execcloseall(fds, cmdct);
	if (execve(argums[0], argums, NULL) < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	return (0);
}

int	ft_execparentproc(char **argums, int **fds, int childpid, int *i)
{
	int	childstatus;

	close(fds[*i][0]);
	close(fds[*i + 1][1]);
	waitpid(childpid, &childstatus, 0);
	ft_freesplit(argums);
	(*i)++;
	if (childstatus != 0)
		return (-1);
	return (0);
}

int	ft_executer(t_cmd *fullcmds, int cmdct)
{
	int		childpid;
	int		**fds;
	char	**argums;
	int		i;

	fds = ft_execfdpairs(cmdct);
	if (!fds)
		return (-1);
	i = 0;
	while (i <= cmdct)
	{
		if (pipe(fds[i]) < 0)
			return (ft_frfds(fds, cmdct + 1), ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		i++;
	}
	i = 0;
	while (i < cmdct)
	{
		argums = ft_execargums(fullcmds[i].cmdarg);
		childpid = fork();
		if (childpid == -1)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (childpid == 0)
		{
			if (ft_execchildproc(fullcmds, argums, fds, i, cmdct))
				return (ft_frfds(fds, cmdct + 1), ft_freesplit(argums), -1);
		}
		else
		{
			if (ft_execparentproc(argums, fds, childpid, &i))
				return (ft_frfds(fds, cmdct + 1), -1);
		}
	}
	ft_frfds(fds, cmdct + 1);
	return (0);
}

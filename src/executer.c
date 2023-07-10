/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 11:52:45 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/10 12:55:47 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	int		off;

	i = 0;
	if (ft_findchar(cmd, '/') == 0)
		cmd = ft_strjoinfree("/", cmd, 2);
	paths = ft_split(getenv("PATH"), ':');
	cmdpath = ft_pathname(cmd, paths);
	if (cmdpath == NULL)
	{
		off = 0;
		if (*cmd == '/')
			off = 1;
		ft_printf("minishell: %s: command not found\n", cmd + off);//change error message
		free (cmd);
		ft_freesplit(paths);
		return (strdup(""));
	}
	free (cmd);
	ft_freesplit(paths);
	return (cmdpath);
}

int	ft_isbuiltin(char *cmd)
{
	size_t	ln;

	ln = ft_strlen(cmd);
	if (!ft_strncmp(cmd, "echo", ft_strlen(cmd)) && ln == ft_strlen("echo"))
		return (0);
	if (!ft_strncmp(cmd, "cd", ft_strlen(cmd)) && ln == ft_strlen("cd"))
		return (0);
	if (!ft_strncmp(cmd, "pwd", ft_strlen(cmd)) && ln == ft_strlen("pwd"))
		return (0);
	if (!ft_strncmp(cmd, "export", ft_strlen(cmd)) && ln == ft_strlen("export"))
		return (0);
	if (!ft_strncmp(cmd, "unset", ft_strlen(cmd)) && ln == ft_strlen("unset"))
		return (0);
	if (!ft_strncmp(cmd, "env", ft_strlen(cmd)) && ln == ft_strlen("env"))
		return (0);
	if (!ft_strncmp(cmd, "exit", ft_strlen(cmd)) && ln == ft_strlen("exit"))
		return (0);
	return (1);
}

char	**ft_execargums(char *cmdarg)
{
	char	**argums;

	argums = ft_splitnotstr(cmdarg, ' ');
	if (ft_isbuiltin(argums[0]))
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
		ft_printf("here_doc> "); //change this maybe??
		line = get_next_line(0); //what if line is null bc ctrl-D?
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

int	ft_execute(t_fullcmd fullcmd)
{
	if (ft_isbuiltin(fullcmd.argums[0]))
	{
		if (execve(fullcmd.argums[0], fullcmd.argums, NULL) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	else if (!ft_strncmp(fullcmd.argums[0], "echo", ft_strlen(fullcmd.argums[0])))
	{
		if (ft_echo(fullcmd) < 0)
			return (-1);
	}
	else if (!ft_strncmp(fullcmd.argums[0], "pwd", ft_strlen(fullcmd.argums[0])))
	{
		if (ft_pwd() < 0)
			return (-1);
	}
	return (0);
}

int	ft_execchildproc(t_cmd *cmds, t_fullcmd fullcmd, int i)
{
	int fdin;
	int fdout;
	if (cmds[i].rein.rein == 1)
	{
		fdin = open(cmds[i].rein.infile, O_RDONLY);
		if (fdin < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (dup2(fdin, STDIN_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	else if (cmds[i].rein.herein == 1)
	{
		fdin = ft_execheredoc(cmds[i].rein.heredel);
		if (fdin < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (dup2(fdin, STDIN_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	else if (i > 0)
	{
		if (dup2(fullcmd.fds[i][0], STDIN_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	if (cmds[i].reout.reout == 1)
	{
		if (access(cmds[i].reout.outfile, F_OK) == 0)
		{
			if (unlink(cmds[i].reout.outfile) < 0)
				return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		}
		fdout = open(cmds[i].reout.outfile, O_WRONLY | O_CREAT, 0644);
		if (fdout < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (dup2(fdout, STDOUT_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	else if (cmds[i].reout.reoutapp == 1)
	{
		fdout = open(cmds[i].reout.outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fdout < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (dup2(fdout, STDOUT_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	else if (i < fullcmd.cmdct - 1)
	{
		if (dup2(fullcmd.fds[i + 1][1], STDOUT_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	ft_execcloseall(fullcmd.fds, fullcmd.cmdct);
	if (ft_execute(fullcmd) < 0)
		return (-1);
	return (0);
}

int	ft_execparentproc(t_fullcmd	fullcmd, int childpid, int *i)
{
	int	childstatus;

	close(fullcmd.fds[*i][0]);
	close(fullcmd.fds[*i + 1][1]);
	waitpid(childpid, &childstatus, 0);
	ft_freesplit(fullcmd.argums);
	(*i)++;
	if (childstatus != 0)
		return (-1);
	return (0);
}

int	ft_executer(t_cmd *cmds, int cmdct)
{
	int			childpid;
	t_fullcmd	fullcmd;
	//int		**fds;
	//char	**argums;
	int		i;

	fullcmd.cmds = cmds;
	fullcmd.cmdct = cmdct;
	fullcmd.fds = ft_execfdpairs(cmdct);
	if (!fullcmd.fds)
		return (-1);
	i = 0;
	while (i <= cmdct)
	{
		if (pipe(fullcmd.fds[i]) < 0)
			return (ft_frfds(fullcmd.fds, fullcmd.cmdct + 1), ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		i++;
	}
	i = 0;
	while (i < cmdct)
	{
		fullcmd.argums = ft_execargums(fullcmd.cmds[i].cmdarg);
		childpid = fork();
		if (childpid == -1)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (childpid == 0)
		{
			if (ft_execchildproc(fullcmd.cmds, fullcmd, i))
				return (ft_frfds(fullcmd.fds, fullcmd.cmdct + 1), ft_freesplit(fullcmd.argums), -1);
		}
		else
		{
			if (ft_execparentproc(fullcmd, childpid, &i))
				return (ft_frfds(fullcmd.fds, cmdct + 1), -1);
		}
	}
	ft_frfds(fullcmd.fds, fullcmd.cmdct + 1);
	return (0);
}

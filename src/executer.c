/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 11:52:45 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/18 08:25:53 by ysmeding         ###   ########.fr       */
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
	(void)i;
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

int	ft_getlen_str(char *str, int i)
{
	int	len;

	len = 0;
	while (str[i + len] && str[i + len] > 32)
	{
		if (str[i + len] == '\"' && ft_findchar(&str[i + len + 1], '\"'))
		{
			len++;
			while (str[i + len] != '\"')
				len++;
			len++;
		}
		else if (str[i + len] == '\'' && ft_findchar(&str[i + len + 1], '\''))
		{
			len++;
			while (str[i + len] != '\'')
				len++;
			len++;
		}
		else
			len++;
	}
	return (len);
}

char	**get_expand_noquote_str(char **cmd_parts, char *cmdarg, int i, int len)
{
	char	*part;
	char	**part_div;
	char	**expand_part;
	int		ast[2];
//need some malloc protection, return null when result is null.
	part = ft_substr(&cmdarg[i], 0, len);
	if (ft_findcharout(part, '*') == 1)
	{
		ast[0] = 0;
		ast[1] = 0;
		part_div = ft_pattern_pieces(part, ast);
		expand_part = ft_expandasterisk(part_div, ast);
		cmd_parts = ft_appendtoarr(cmd_parts, expand_part);
	}
	else
	{
		part = ft_strip_quotes(part);
		cmd_parts = ft_arrapp(cmd_parts, part);
	}
	return (cmd_parts);
}

char	**ft_splitcmdline(char *cmdarg)
{
	char	**cmd_parts;
	int		i;
	int		len;
//need some malloc protection, return null when result is null.
	i = 0;
	cmd_parts = NULL;
	while (cmdarg[i])
	{
		len = ft_getlen_str(cmdarg, i);
		if (len)
			cmd_parts = get_expand_noquote_str(cmd_parts, cmdarg, i, len);
		i += len;
		while (cmdarg[i] && cmdarg[i] <= 32)
			i++;
	}
	return (cmd_parts);
}

char	**ft_execargums(char *cmdarg)
{
	char	**argums;

	argums = ft_splitcmdline(cmdarg);
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

int	ft_exec_redirin(t_cmd *cmds, t_fullcmd fullcmd, int i, int fdin)
{
	if (cmds[i].rein.rein == 1)
	{
		fdin = open(cmds[i].rein.infile, O_RDONLY);
		if (fdin < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (dup2(fdin, STDIN_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		close(fdin);
	}
	else if (cmds[i].rein.herein == 1)
	{
		fdin = ft_execheredoc(cmds[i].rein.heredel);
		if (fdin < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (dup2(fdin, STDIN_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		close(fdin);
	}
	else if (i > 0)
	{
		if (dup2(fullcmd.fds[i][0], STDIN_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	return (0);
}

int	ft_exec_redirout(t_cmd *cmds, t_fullcmd fullcmd, int i, int fdo)
{
	if (cmds[i].reout.reout == 1)
	{
		fdo = open(cmds[i].reout.outfile, O_WRONLY | O_CREAT, 0644);
		if (fdo < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (dup2(fdo, STDOUT_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		close(fdo);
	}
	else if (cmds[i].reout.reoutapp == 1)
	{
		fdo = open(cmds[i].reout.outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fdo < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (dup2(fdo, STDOUT_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		close(fdo);
	}
	else if (i < fullcmd.cmdct - 1)
	{
		if (dup2(fullcmd.fds[i + 1][1], STDOUT_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	return (0);
}

int ft_exec_redir(t_cmd *cmds, t_fullcmd fullcmd, int i)
{
	int	fdin;
	int	fdout;

	fdin = -1;
	fdout = -1;
	if (ft_exec_redirin(cmds, fullcmd, i, fdin))
		return (-1);
	if (ft_exec_redirout(cmds, fullcmd, i, fdout))
		return (-1);
	return (0);
}

int	ft_choose_builtin(t_fullcmd fullcmd, char ***env)
{
	int	err;

	err = 0;
	if (!ft_strncmp(fullcmd.argums[0], "echo", \
		ft_strlen(fullcmd.argums[0])))
		err = ft_echo(fullcmd);
	else if (!ft_strncmp(fullcmd.argums[0], "pwd", \
			ft_strlen(fullcmd.argums[0])))
		err = ft_pwd(*env);
	else if (!ft_strncmp(fullcmd.argums[0], "export", \
			ft_strlen(fullcmd.argums[0])))
		err = ft_export(fullcmd, env);
	else if (!ft_strncmp(fullcmd.argums[0], "env", \
			ft_strlen(fullcmd.argums[0])))
		err = ft_env(fullcmd, env);
	else if (!ft_strncmp(fullcmd.argums[0], "unset", \
			ft_strlen(fullcmd.argums[0])))
		err = ft_unset(fullcmd, env);
	if (err)
		return (-1);
	return (0);
}

int	ft_execute(t_cmd *cmds, t_fullcmd fullcmd, int i, char ***env)
{
	int	stdincpy;
	int	stdoutcpy;
	int	err;

	err = 0;
	stdincpy = dup(STDIN_FILENO);
	stdoutcpy = dup(STDOUT_FILENO);
	if (ft_exec_redir(cmds, fullcmd, i))
		return (-1);
	close(fullcmd.fds[i][0]);
	close(fullcmd.fds[i + 1][1]);
	err = ft_choose_builtin(fullcmd, env);
	dup2(stdincpy, STDIN_FILENO);
	dup2(stdoutcpy, STDOUT_FILENO);
	close(stdincpy);
	close(stdoutcpy);
	if (err)
		return (-1);
	return (0);
}

int	ft_execchildproc(t_cmd *cmds, t_fullcmd fullcmd, int i, char ***env)
{
	int	fdin;
	int	fdout;

	fdin = -1;
	fdout = -1;
	if (ft_exec_redirin(cmds, fullcmd, i, fdin))
		exit(-1);
	if (ft_exec_redirout(cmds, fullcmd, i, fdout))
		exit(-1);
	ft_execcloseall(fullcmd.fds, fullcmd.cmdct);
	if (execve(fullcmd.argums[0], fullcmd.argums, *env) < 0)
	{
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		exit (-1);
	}
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

int	ft_executeempty(t_fullcmd fullcmd, int *i)
{
	ft_freesplit(fullcmd.argums);
	close(fullcmd.fds[*i][0]);
	close(fullcmd.fds[*i + 1][1]);
	(*i)++;
	return (-1);
}

int	ft_executeinfork(t_fullcmd fullcmd, int *i, char ***env)
{
	int	childpid;
	int	err;

	err = 0;
	childpid = fork();
	if (childpid == -1)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	if (childpid == 0)
		ft_execchildproc(fullcmd.cmds, fullcmd, *i, env);
	else
		err = ft_execparentproc(fullcmd, childpid, i);
	return (err);
}

int	ft_executebuiltin(t_fullcmd fullcmd, int *i, char ***env)
{
	int	err;

	err = ft_execute(fullcmd.cmds, fullcmd, *i, env);
	ft_freesplit(fullcmd.argums);
	close(fullcmd.fds[*i][0]);
	close(fullcmd.fds[*i + 1][1]);
	(*i)++;
	return (err);
}

int	ft_findexecmethod(t_fullcmd fullcmd, int *i, char ***env)
{
	int	err;

	if (*fullcmd.argums[0] == 0)
		err = ft_executeempty(fullcmd, i);
	else if (ft_isbuiltin(fullcmd.argums[0]))
		err = ft_executeinfork(fullcmd, i, env);
	else
		err = ft_executebuiltin(fullcmd, i, env);
	return (err);
}

int	ft_executer(t_cmd *cmds, int cmdct, char ***env)
{
	int			err;
	t_fullcmd	fullcmd;
	int			i;

	fullcmd.cmds = cmds;
	fullcmd.cmdct = cmdct;
	fullcmd.fds = ft_execfdpairs(cmdct);
	err = 0;
	if (!fullcmd.fds)
		return (-1);
	i = -1;
	while (++i <= cmdct)
	{
		if (pipe(fullcmd.fds[i]) < 0)
			return (ft_frfds(fullcmd.fds, fullcmd.cmdct + 1),
				ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	i = 0;
	while (i < cmdct)
	{
		fullcmd.argums = ft_execargums(fullcmd.cmds[i].cmdarg);
		err = ft_findexecmethod(fullcmd, &i, env);
	}
	ft_frfds(fullcmd.fds, fullcmd.cmdct + 1);
	return (err);
}

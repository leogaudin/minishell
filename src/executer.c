/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 11:52:45 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/26 12:47:14 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	**ft_execfdpairs(int cmdct)
{
	int	i;
	int	**fds;

	if (cmdct == 1)
		return (NULL);
	fds = malloc((cmdct - 1) * sizeof(int *));
	if (!fds)
	{
		ft_putendl_fd("Memory allocation failed.", STDERR_FILENO);
		return (NULL);
	}
	if (fds)
	{
		i = 0;
		while (i < cmdct - 1)
		{
			fds[i] = malloc(2 * sizeof(int));
			if (!fds[i])
			{
				ft_putendl_fd("Memory allocation failed.", STDERR_FILENO);
				return (ft_frfds(fds, i), NULL);
			}
			i++;
		}
	}
	return (fds);
}

char	*ft_execgetpathname(char *cmd, char ***env)
{
	int		i;
	char	**paths;
	char	*cmdpath;
	int		off;
	char	*pathvar;

	off = 0;
	i = 0;
	if (cmd && !*cmd)
		return (cmd);
	else if (ft_findchar(cmd, '/') != 0)
		off--;
	if (ft_findchar(cmd, '/') == 0)
		cmd = ft_strjoinfree("/", cmd, 2);
	if (!cmd)
		return (ft_putendl_fd("Memory allocation failed.", 2), NULL);
	pathvar = ft_getenv("PATH", *env);
	if (!pathvar)
		return (free(cmd), ft_putendl_fd("Memory allocation failed.", 2), NULL);
	paths = ft_split(pathvar, ':');
	if (!paths)
		return (free(cmd), free(pathvar), ft_putendl_fd("Memory allocation failed.", 2), NULL);
	cmdpath = ft_pathname(cmd, paths);
	if (cmdpath && !*cmdpath)
		ft_printf("Minishell: %s: command not found\n", cmd + (++off));//change error message
	free(cmd);
	free(pathvar);
	ft_freesplit(paths);
	return (cmdpath);
}

int	ft_isbuiltin(char *cmd)
{
	size_t	ln;

	ln = 0;
	(void) cmd;
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

	part = ft_substr(&cmdarg[i], 0, len);
	if (!part)
	{
		ft_putendl_fd("Memory allocation failed.", STDERR_FILENO);
		return (NULL);
	}
	if (ft_findcharout(part, '*') == 1)
	{
		ast[0] = 0;
		ast[1] = 0;
		part_div = ft_pattern_pieces(part, ast);
		expand_part = ft_expandasterisk(part_div, ast);
		if (!expand_part)
			return (free(part), NULL);
		if (expand_part && !expand_part[0])
		{
			part = ft_strip_quotes(part);
			if (!part)
				return (NULL);
			cmd_parts = ft_arrapp(cmd_parts, part);
		}
		else
		{
			cmd_parts = ft_appendtoarr(cmd_parts, expand_part);
			free(part);
			if (!cmd_parts)
			{
				ft_putendl_fd("Memory allocation failed.", STDERR_FILENO);
				return (free(part), NULL);
			}
		}
	}
	else
	{
		part = ft_strip_quotes(part);
		if (!part)
			return (NULL);
		cmd_parts = ft_arrapp(cmd_parts, part);
	}
	return (cmd_parts);
}

char	**ft_splitcmdline(char *cmdarg)
{
	char	**cmd_parts;
	int		i;
	int		len;

	i = 0;
	cmd_parts = NULL;
	if (cmdarg && !*cmdarg)
	{
		cmd_parts = ft_arrapp(cmd_parts, strdup(""));
		return (cmd_parts);
	}
	while (cmdarg[i])
	{
		len = ft_getlen_str(cmdarg, i);
		if (len)
		{
			cmd_parts = get_expand_noquote_str(cmd_parts, cmdarg, i, len);
			if (!cmd_parts)
				return (NULL);
		}
		i += len;
		while (cmdarg[i] && cmdarg[i] <= 32)
			i++;
	}
	return (cmd_parts);
}

char	**ft_execargums(char *cmdarg, char ***env)
{
	char	**argums;
	int		i;

	argums = ft_splitcmdline(cmdarg);
	if (argums && argums[0] && ft_isbuiltin(argums[0]))
	{
		argums[0] = ft_execgetpathname(argums[0], env);
		if (!argums[0])
		{
			i = 1;
			while (argums[i])
			{
				free(argums[i]);
				i++;
			}
			free(argums);
			return (NULL);
		}
	}
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
		ft_printf("\033[0;34m> \033[0m");
		line = get_next_line(0);
		//line = readline(">");
		if (!line)
			break ;
		else if (*line && *line != '\n')
			same = ft_strncmp(delim, line, ft_strlen(line) - 1);
		if (same != 0)
			write(fdpair[1], line, ft_strlen(line));
		free(line);
	}
	//write(1, "OUTSIDELOOP", ft_strlen("OUTSIDELOOP"));
	close(fdpair[1]);
	return (fdpair[0]);
}

/* int	ft_execheredoc(char *delim)
{
	int		fdpair[2];
	int		same;
	char	*line;
	int		childpid;

	if (pipe(fdpair) < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	childpid = fork();
	if (childpid < 0)
		return (-1);
	else if (childpid == 0)
	{
		same = 1;
		while (same != 0)
		{
			ft_printf("> ");
			line = get_next_line(0);
			//line = readline(">");
			//write(1, "ABC\n", 4);
			if (!line)
				break ;
			same = ft_strncmp(delim, line, ft_strlen(line) - 1);
			if (same != 0)
				write(fdpair[1], line, ft_strlen(line));
			free(line);
		}
		//write(1, "OUTSIDELOOP", ft_strlen("OUTSIDELOOP"));
		close(fdpair[1]);
		exit(fdpair[0]);
	}
	else
	{
		waitpid(childpid, NULL, 0);
		close(fdpair[1]);
		return (fdpair[0]);
	}
} */

int	ft_execcloseall(int **fd, int pipect)
{
	int	i;

	if (fd)
	{
		i = 0;
		while (i < pipect - 1)
		{
			close(fd[i][0]);
			close(fd[i][1]);
			i++;
		}
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
		g_exit_code = -15;
		fdin = ft_execheredoc(cmds[i].rein.heredel);
		if (fdin < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		if (dup2(fdin, STDIN_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		close(fdin);
	}
	else if (i > 0)
	{
		if (dup2(fullcmd.fds[i - 1][0], STDIN_FILENO) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	return (0);
}

int	ft_exec_redirout(t_cmd *cmds, t_fullcmd fullcmd, int i, int fdo)
{
	if (cmds[i].reout.reout == 1)
	{
		fdo = open(cmds[i].reout.outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
		if (dup2(fullcmd.fds[i][1], STDOUT_FILENO) < 0)
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
	else if (!ft_strncmp(fullcmd.argums[0], "cd", \
			ft_strlen(fullcmd.argums[0])))
		err = ft_cd(fullcmd, env);
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
	if (stdincpy < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	stdoutcpy = dup(STDOUT_FILENO);
	if (stdoutcpy < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	if (ft_exec_redir(cmds, fullcmd, i))
		return (-1);
	err = ft_choose_builtin(fullcmd, env);
	if (dup2(stdincpy, STDIN_FILENO) < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	if (dup2(stdoutcpy, STDOUT_FILENO) < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
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
		write(1, "BIG ERROR!\n", 11);
		//ft_putendl_fd(strerror(errno), STDERR_FILENO);
		exit (-1);
	}
	return (0);
}

int	ft_execparentproc(t_fullcmd	fullcmd, int childpid, int *i)
{
	int	childstatus;

	if (*i == 0 && fullcmd.cmdct > 1)
		close(fullcmd.fds[*i][1]);
	if (*i == fullcmd.cmdct - 1 && fullcmd.cmdct > 1)
		close(fullcmd.fds[*i - 1][0]);
	if (*i > 0 && *i < fullcmd.cmdct - 1)
	{
		close(fullcmd.fds[*i - 1][0]);
		close(fullcmd.fds[*i][1]);
	}
	waitpid(childpid, &childstatus, 0);
	ft_freearr(fullcmd.argums);
	(*i)++;
	if (childstatus != 0)
		return (-1);
	return (0);
}

int	ft_executeempty(t_fullcmd fullcmd, int *i)
{
	int	stdincpy;
	int	stdoutcpy;

	stdincpy = dup(STDIN_FILENO);
	if (stdincpy < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	stdoutcpy = dup(STDOUT_FILENO);
	if (stdoutcpy < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	if (ft_exec_redir(fullcmd.cmds, fullcmd, *i))
		return (g_exit_code = -1, -1);
	g_exit_code = 0;	
	if (dup2(stdincpy, STDIN_FILENO) < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	if (dup2(stdoutcpy, STDOUT_FILENO) < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	close(stdincpy);
	close(stdoutcpy);
	ft_freearr(fullcmd.argums);
	if (*i == 0 && fullcmd.cmdct > 1)
		close(fullcmd.fds[*i][1]);
	if (*i == fullcmd.cmdct - 1 && fullcmd.cmdct > 1)
		close(fullcmd.fds[*i - 1][0]);
	if (*i > 0 && *i < fullcmd.cmdct - 1)
	{
		close(fullcmd.fds[*i - 1][0]);
		close(fullcmd.fds[*i][1]);
	}
	(*i)++;
	return (-1);
}

void sigint_handler2(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', 1);
	rl_on_new_line();
	rl_replace_line("bye", 0);
	rl_redisplay();
}

int	ft_executeinfork(t_fullcmd fullcmd, int *i, char ***env)
{
	int	childpid;
	int	err;

	err = 0;
	childpid = fork();
	if (childpid == -1)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	g_exit_code = -14;
	if (childpid == 0)
	{
		ft_execchildproc(fullcmd.cmds, fullcmd, *i, env);
	}
	else
	{
		err = ft_execparentproc(fullcmd, childpid, i);
		g_exit_code = err;
	}
	return (err);
}

int	ft_executebuiltin(t_fullcmd fullcmd, int *i, char ***env)
{
	int	err;

	err = ft_execute(fullcmd.cmds, fullcmd, *i, env);
	ft_freearr(fullcmd.argums);
	if (*i == 0 && fullcmd.cmdct > 1)
	{
		close(fullcmd.fds[*i][1]);
	}
	if (*i == fullcmd.cmdct - 1 && fullcmd.cmdct > 1)
	{
	 	close(fullcmd.fds[*i - 1][0]);
	}
	if (*i > 0 && *i < fullcmd.cmdct - 1)
	{
		close(fullcmd.fds[*i - 1][0]);
		close(fullcmd.fds[*i][1]);
	}
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
	if (!fullcmd.fds && cmdct > 1)
		return (-1);
	i = -1;
	while (++i < cmdct - 1)
	{
		if (pipe(fullcmd.fds[i]) < 0)
			return (ft_frfds(fullcmd.fds, fullcmd.cmdct),
				ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	i = 0;
	while (i < cmdct)
	{
		fullcmd.argums = ft_execargums(fullcmd.cmds[i].cmdarg, env);
		if (!fullcmd.argums)
			return (ft_frfds(fullcmd.fds, fullcmd.cmdct), -1);
		err = ft_findexecmethod(fullcmd, &i, env);
	}
	ft_frfds(fullcmd.fds, fullcmd.cmdct);
	return (err);
}

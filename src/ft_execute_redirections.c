/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_redirections.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:23:23 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/08 13:59:43 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		if (!line)
			break ;
		else if (*line && *line != '\n')
			same = ft_strncmp(delim, line, ft_strlen(line) - 1);
		if (same != 0)
			write(fdpair[1], line, ft_strlen(line));
		free(line);
	}
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

int	ft_exec_redir(t_cmd *cmds, t_fullcmd fullcmd, int i)
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

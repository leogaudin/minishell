/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_redirections.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:23:23 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/20 13:05:53 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_execheredoc(char *delim, t_gen_info *info)
{
	int		fdpair[2];
	int		same;
	char	*line;
	int		stdincpy;

	stdincpy = dup(STDIN_FILENO);
	if (pipe(fdpair) < 0)
		return (ft_putstrerror("pipe: ", info), -1);
	same = 1;
	g_code *= 10;
	while (same != 0)
	{
		ft_printf("\033[0;34m> \033[0m");
		line = get_next_line(0);
		if (!line)
		{
			info->exit_code = 1;
			break ;
		}
		else if (*line && *line != '\n')
			same = ft_strncmp(delim, line, ft_strlen(line) - 1);
		if (same != 0)
			write(fdpair[1], line, ft_strlen(line));
		free(line);
	}
	if (same == 0)
		dup2(stdincpy, STDIN_FILENO);
	close(stdincpy);
	close(fdpair[1]);
	return (fdpair[0]);
}

int	ft_exec_redirin(t_cmd *cmds, t_fullcmd fullcmd, int i, t_gen_info *info)
{
	int	fdin;

	if (cmds[i].rein.rein == 1)
	{
		fdin = open(cmds[i].rein.infile, O_RDONLY);
		if (fd_dupin(fdin, info))
			return (-1);
	}
	else if (cmds[i].rein.herein == 1)
	{
		fdin = ft_execheredoc(cmds[i].rein.heredel, info);
		if (g_code >= 10)
			g_code /= 10;
		if (fdin < 0)
			return (-1);
		if (dup2(fdin, STDIN_FILENO) < 0)
			return (ft_putstrerror("dup2: ", info), -1);
		close(fdin);
	}
	else if (i > 0)
	{
		if (dup2(fullcmd.fds[i - 1][0], STDIN_FILENO) < 0)
			return (ft_putstrerror("dup2: ", info), -1);
	}
	return (0);
}

int	ft_dupout(int fdout, t_gen_info *info)
{
	if (fdout < 0)
		return (ft_putstrerror("open: ", info), -1);
	if (dup2(fdout, STDOUT_FILENO) < 0)
		return (ft_putstrerror("dup2: ", info), -1);
	close(fdout);
	return (0);
}

int	ft_exec_redirout(t_cmd *cmds, t_fullcmd fullcmd, int i, t_gen_info *info)
{
	int	fdo;

	if (cmds[i].reout.reout == 1)
	{
		fdo = open(cmds[i].reout.outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (ft_dupout(fdo, info))
			return (-1);
	}
	else if (cmds[i].reout.reoutapp == 1)
	{
		fdo = open(cmds[i].reout.outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (ft_dupout(fdo, info))
			return (-1);
	}
	else if (i < fullcmd.cmdct - 1)
	{
		if (dup2(fullcmd.fds[i][1], STDOUT_FILENO) < 0)
			return (ft_putstrerror("dup2: ", info), -1);
	}
	return (0);
}

int	ft_exec_redir(t_cmd *cmds, t_fullcmd fullcmd, int i, t_gen_info *info)
{
	if (ft_exec_redirin(cmds, fullcmd, i, info))
		return (-1);
	if (ft_exec_redirout(cmds, fullcmd, i, info))
		return (-1);
	return (0);
}

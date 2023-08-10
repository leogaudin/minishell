/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 11:52:45 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/09 07:36:39 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_closepipeends(t_fullcmd fullcmd, int *i)
{
	if (*i == 0 && fullcmd.cmdct > 1)
		close(fullcmd.fds[*i][1]);
	if (*i == fullcmd.cmdct - 1 && fullcmd.cmdct > 1)
		close(fullcmd.fds[*i - 1][0]);
	if (*i > 0 && *i < fullcmd.cmdct - 1)
	{
		close(fullcmd.fds[*i - 1][0]);
		close(fullcmd.fds[*i][1]);
	}
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
		return (g_exit_code = -1, -1);//changenumber
	//g_exit_code = 0;
	if (dup2(stdincpy, STDIN_FILENO) < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	if (dup2(stdoutcpy, STDOUT_FILENO) < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	close(stdincpy);
	close(stdoutcpy);
	ft_closepipeends(fullcmd, i);
	(*i)++;
	return (-1);
}

int	ft_findexecmethod(t_fullcmd fullcmd, int *i, char ***env)
{
	int	err;

	if (*fullcmd.argums[0] == 0)
	{
		fullcmd.childpid[*i] = -2;
		err = ft_executeempty(fullcmd, i);
	}
	else if (ft_isbuiltin(fullcmd.argums[0]))
	{
		err = 0;
		fullcmd.childpid[*i] = fork();
		if (fullcmd.childpid[*i] == -1)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		else if (fullcmd.childpid[*i] == 0)
			err = ft_execchildproc(fullcmd.cmds, fullcmd, *i, env);
		else
			(*i)++;
	}
	else
	{
		fullcmd.childpid[*i] = -2;
		err = ft_executebuiltin(fullcmd, i, env);
	}
	return (err);
}

int	ft_executer_loops(t_fullcmd	fullcmd, int cmdct, char ***env)
{
	int	i;
	int	j;
	int	err;

	i = 0;
	while (i < cmdct)
	{
		fullcmd.argums = ft_execargums(fullcmd.cmds[i].cmdarg, env);
		if (!fullcmd.argums)
			return (ft_frfds(fullcmd.fds, cmdct), free(fullcmd.childpid), -1);
		err = ft_findexecmethod(fullcmd, &i, env);
		ft_freearr(fullcmd.argums);
	}
	j = -1;
	while (++j < cmdct)
	{
		if (fullcmd.childpid[j] >= 0)
			err = ft_execparentproc(fullcmd, fullcmd.childpid[j], &j);
	}
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
	fullcmd.childpid = malloc(cmdct * sizeof(int));
	err = 0;
	if (!fullcmd.fds && cmdct > 1)
		return (-1);
	i = -1;
	while (++i < cmdct - 1)
	{
		if (pipe(fullcmd.fds[i]) < 0)
			return (ft_frfds(fullcmd.fds, cmdct), ft_putstrerror("pipe: "), -1);
	}
	err = ft_executer_loops(fullcmd, cmdct, env);
	ft_frfds(fullcmd.fds, fullcmd.cmdct);
	free(fullcmd.childpid);
	return (err);
}

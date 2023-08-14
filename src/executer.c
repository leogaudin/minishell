/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 11:52:45 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/13 11:20:14 by ysmeding         ###   ########.fr       */
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

int	ft_executeempty(t_fullcmd fullcmd, int *i, t_gen_info *info)
{
	int	stdincpy;
	int	stdoutcpy;

	stdincpy = dup(STDIN_FILENO);
	if (stdincpy < 0)
		return (ft_putstrerror("dup: ", info), info->exit_code);
	stdoutcpy = dup(STDOUT_FILENO);
	if (stdoutcpy < 0)
		return (ft_putstrerror("dup: ", info), info->exit_code);
	if (ft_exec_redir(fullcmd.cmds, fullcmd, *i, info))
		return (info->exit_code);
	if (dup2(stdincpy, STDIN_FILENO) < 0)
		return (ft_putstrerror("dup2: ", info), info->exit_code);
	if (dup2(stdoutcpy, STDOUT_FILENO) < 0)
		return (ft_putstrerror("dup2: ", info), info->exit_code);
	close(stdincpy);
	close(stdoutcpy);
	ft_closepipeends(fullcmd, i);
	(*i)++;
	return (info->exit_code);
}

int	ft_findexecmethod(t_fullcmd fullcmd, int *i, t_gen_info *info)
{
	if (*fullcmd.argums[0] == 0)
	{
		fullcmd.childpid[*i] = -2;
		info->exit_code = ft_executeempty(fullcmd, i, info);
	}
	else if (ft_isbuiltin(fullcmd.argums[0]))
	{
		fullcmd.childpid[*i] = fork();
		if (fullcmd.childpid[*i] == -1)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
		else if (fullcmd.childpid[*i] == 0)
			info->exit_code = ft_execchildproc(fullcmd.cmds, fullcmd, *i, info);
		else
			(*i)++;
	}
	else
	{
		fullcmd.childpid[*i] = -2;
		info->exit_code = ft_executebuiltin(fullcmd, i, info);
	}
	return (info->exit_code);
}

int	ft_executer_loops(t_fullcmd	fullcmd, int cmdct, t_gen_info *info)
{
	int	i;
	int	j;

	i = 0;
	while (i < cmdct)
	{
		fullcmd.argums = ft_execargums(fullcmd.cmds[i].cmdarg, info);
		if (!fullcmd.argums)
		{
			ft_frfds(fullcmd.fds, cmdct);
			return (free(fullcmd.childpid), info->exit_code);
		}
		ft_exportlastarg(fullcmd.argums, info);
		ft_findexecmethod(fullcmd, &i, info);
		ft_freearr(fullcmd.argums);
	}
	j = -1;
	while (++j < cmdct)
	{
		if (fullcmd.childpid[j] >= 0)
			info->exit_code = ft_execparentproc(fullcmd, fullcmd.childpid[j], \
			&j, info);
	}
	return (info->exit_code);
}

int	ft_executer(t_cmd *cmds, int cmdct, t_gen_info *info)
{
	t_fullcmd	fullcmd;
	int			i;

	fullcmd.cmds = cmds;
	fullcmd.cmdct = cmdct;
	fullcmd.fds = ft_execfdpairs(cmdct, info);
	fullcmd.childpid = malloc(cmdct * sizeof(int));
	if (!fullcmd.fds && cmdct > 1)
		return (info->exit_code);
	i = -1;
	while (++i < cmdct - 1)
	{
		if (pipe(fullcmd.fds[i]) < 0)
		{
			ft_frfds(fullcmd.fds, cmdct);
			return (ft_putstrerror("pipe: ", info), info->exit_code);
		}
	}
	info->exit_code = ft_executer_loops(fullcmd, cmdct, info);
	ft_frfds(fullcmd.fds, fullcmd.cmdct);
	free(fullcmd.childpid);
	return (info->exit_code);
}

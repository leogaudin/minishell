/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_methods.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 07:35:19 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/20 13:40:32 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_choose_builtin(t_fullcmd fullcmd, t_gen_info *info)
{
	if (!ft_strncmp(fullcmd.argums[0], "echo", \
		ft_strlen(fullcmd.argums[0])))
		info->exit_code = ft_echo(fullcmd, info);
	else if (!ft_strncmp(fullcmd.argums[0], "pwd", \
			ft_strlen(fullcmd.argums[0])))
		info->exit_code = ft_pwd(info);
	else if (!ft_strncmp(fullcmd.argums[0], "export", \
			ft_strlen(fullcmd.argums[0])))
		info->exit_code = ft_export(fullcmd, info);
	else if (!ft_strncmp(fullcmd.argums[0], "env", \
			ft_strlen(fullcmd.argums[0])))
		info->exit_code = ft_env(fullcmd, info);
	else if (!ft_strncmp(fullcmd.argums[0], "unset", \
			ft_strlen(fullcmd.argums[0])))
		info->exit_code = ft_unset(fullcmd, info);
	else if (!ft_strncmp(fullcmd.argums[0], "cd", \
			ft_strlen(fullcmd.argums[0])))
		info->exit_code = ft_cd(fullcmd, info);
	return (info->exit_code);
}

int	ft_execute(t_cmd *cmds, t_fullcmd fullcmd, int i, t_gen_info *info)
{
	int	stdincpy;
	int	stdoutcpy;

	stdincpy = dup(STDIN_FILENO);
	if (stdincpy < 0)
		return (ft_putstrerror("dup: ", info), info->exit_code);
	stdoutcpy = dup(STDOUT_FILENO);
	if (stdoutcpy < 0)
		return (ft_putstrerror("dup: ", info), info->exit_code);
	if (ft_exec_redir(cmds, fullcmd, i, info))
		return (info->exit_code);
	if (info->here_code == 0)
		info->exit_code = ft_choose_builtin(fullcmd, info);
	if (dup2(stdincpy, STDIN_FILENO) < 0)
		return (ft_putstrerror("dup2: ", info), info->exit_code);
	if (dup2(stdoutcpy, STDOUT_FILENO) < 0)
		return (ft_putstrerror("dup2: ", info), info->exit_code);
	close(stdincpy);
	close(stdoutcpy);
	info->here_code = 0;
	return (info->exit_code);
}

int	ft_executebuiltin(t_fullcmd fullcmd, int *i, t_gen_info *info)
{
	info->exit_code = ft_execute(fullcmd.cmds, fullcmd, *i, info);
	ft_closepipeends(fullcmd, i);
	(*i)++;
	return (info->exit_code);
}

int	ft_execchildproc(t_cmd *cmds, t_fullcmd fullcmd, int i, t_gen_info *info)
{
	if (ft_exec_redirin(cmds, fullcmd, i, info))
		exit(info->exit_code);
	if (ft_exec_redirout(cmds, fullcmd, i, info))
		exit(info->exit_code);
	ft_execcloseall(fullcmd.fds, fullcmd.cmdct);
	if (info->here_code == 1)
	{
		info->here_code = 0;
		exit(info->exit_code);
	}
	if (execve(fullcmd.argums[0], fullcmd.argums, info->env) < 0)
	{
		ft_putstrerror("execve: ", info);
		exit (info->exit_code);
	}
	return (info->exit_code);
}

int	ft_execparentproc(t_fullcmd	fullcmd, int childpid, int *i, t_gen_info *info)
{
	int	childstatus;

	ft_closepipeends(fullcmd, i);
	waitpid(childpid, &childstatus, 0);
	g_code = 1;
	if (WIFEXITED(childstatus))
		info->exit_code = WEXITSTATUS(childstatus);
	return (info->exit_code);
}

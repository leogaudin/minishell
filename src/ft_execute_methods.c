/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_methods.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 07:35:19 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/09 13:32:57 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		return (err);
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

int	ft_executebuiltin(t_fullcmd fullcmd, int *i, char ***env)
{
	int	err;

	err = ft_execute(fullcmd.cmds, fullcmd, *i, env);
	ft_closepipeends(fullcmd, i);
	(*i)++;
	return (err);
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
		ft_putstrerror("execve: ");
		exit (g_exit_code);
	}
	return (0);
}

int	ft_execparentproc(t_fullcmd	fullcmd, int childpid, int *i)
{
	int	childstatus;

	ft_closepipeends(fullcmd, i);
	waitpid(childpid, &childstatus, 0);
	if (WIFEXITED(childstatus))
		g_exit_code = WEXITSTATUS(childstatus);
	return (g_exit_code);
}

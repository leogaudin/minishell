/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getpathname.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 11:37:27 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/21 14:47:43 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* This function returns the minimum value of the two values given to it as
 arguments. */
int	ft_min(int a, int b)
{
	if (a > b)
		return (b);
	else
		return (a);
}

/* With this function, we check whether the command cmd exists in one of the
 paths stored as strings in the variable paths and if we have the right
 permissions to execute it. */
char	*ft_pathname(char *cmd, char **paths, int acc, t_gen_info *info)
{
	int		i;
	char	*cmdpath;

	i = -1;
	acc = ft_min(access(cmd, F_OK), access(cmd, X_OK));
	if (acc == 0)
		return (ft_strdup(cmd));
	acc = ft_min(access(cmd + 1, F_OK), access(cmd + 1, X_OK));
	if (acc == 0)
		return (ft_strdup(cmd + 1));
	while (paths[++i] && acc != 0)
	{
		cmdpath = ft_strjoin(paths[i], cmd);
		if (!cmdpath)
			return (ft_putstrerror("malloc: ", info), NULL);
		acc = ft_min(access(cmdpath, F_OK), access(cmdpath, X_OK));
		if (acc == 0)
			return (cmdpath);
		else if (cmdpath)
			free(cmdpath);
	}
	cmdpath = strdup("");
	if (!cmdpath)
		return (ft_putstrerror("malloc: ", info), NULL);
	return (cmdpath);
}

char	*ft_execpathname2(char *cmd, char **paths, int off, t_gen_info *info)
{
	char	*cmdpath;
	int		e;
	int		acc;

	acc = 0;
	cmdpath = ft_pathname(cmd, paths, acc, info);
	e = 127;
	if (cmdpath && !*cmdpath)
	{
		if (off == 0)
		{
			ft_puterror(": command not found", cmd + (++off), NULL);
			info->exit_code = e;
		}
		else
		{
			ft_puterror(": no such file or directory", cmd + (++off), NULL);
			info->exit_code = e;
		}
	}
	free(cmd);
	ft_freesplit(paths);
	return (cmdpath);
}

char	*ft_execgetpathname(char *cmd, t_gen_info *info)
{
	char	**paths;
	char	*cmdpath;
	int		off;
	char	*pathvar;

	off = 0;
	if (cmd && !*cmd)
		return (cmd);
	else if (ft_findchar(cmd, '/') != 0)
		off--;
	if (ft_findchar(cmd, '/') == 0)
		cmd = ft_strjoinfree("/", cmd, 2);
	if (!cmd)
		return (ftme(info), NULL);
	pathvar = ft_getenv("PATH", info);
	if (!pathvar)
		return (free(cmd), ftme(info), NULL);
	paths = ft_split(pathvar, ':');
	if (!paths)
		return (free(cmd), free(pathvar), ftme(info), NULL);
	cmdpath = ft_execpathname2(cmd, paths, off, info);
	free(pathvar);
	return (cmdpath);
}

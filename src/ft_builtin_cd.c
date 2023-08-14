/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:01:43 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/10 14:25:58 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	determine_path(char **path, char *home, t_fullcmd fullcmd, t_gen_info *info)
{
	(void)info;//remove later
	//malloc protections!!!
	if (fullcmd.argums[1] == NULL || !ft_strcmp(fullcmd.argums[1], "~"))
		*path = ft_strdup(home);
	else if (ft_strncmp(fullcmd.argums[1], "~/", 2) == 0)
		*path = ft_strjoin(home, fullcmd.argums[1] + 1);
	else if (ft_strcmp(fullcmd.argums[1], "-") == 0)
	{
		*path = ft_getenv("OLDPWD", info);
		ft_printf("%s\n", *path);
	}
	else
		*path = ft_strdup(fullcmd.argums[1]);
}

int	change_path(char *path, t_fullcmd fullcmd, t_gen_info *info)
{
	(void)info;//remove later
	if (chdir(path) == -1)
	{
		ft_printf("cd: %s: %s\n", strerror(errno), fullcmd.argums[1]);
		free(path);
		return (1);
	}
	free(path);
	return (0);
}

int	update_pwd(char *path, char *oldpath, t_gen_info *info)
{
	int	err;

	(void)info;//remove later
	err = 0;
	oldpath = ft_strjoinfree("export OLDPWD=", oldpath, 2);
	if (!oldpath)
		return (ft_putendl_fd("Memory allocation failed.", STDERR_FILENO), -1);
	path = ft_strjoinfree("export PWD=", path, 2);
	if (!path)
		return (ft_putendl_fd("Memory allocation failed.", STDERR_FILENO), -1);
	err = ft_parseandexec(oldpath, info);
	if (err)
		return (free(oldpath), free(path), -1);
	err = ft_parseandexec(path, info);
	free(oldpath);
	free(path);
	return (err);
}

int	ft_cd(t_fullcmd fullcmd, t_gen_info *info)
{
	char	*path;
	char	*oldpath;
	char	*home;

	info->exit_code = 0;
	path = NULL;
	home = ft_getenv("HOME", info);
	if (!home)
		return (-1);
	//oldpath = getcwd(NULL, 0);
	oldpath = ft_getenv("PWD", info);//changed this so that it works when you do cd .. after removing the directory you were in
	if (!oldpath)
		return (free (home), ft_putendl_fd("Memory allocation failed.", STDERR_FILENO), -1);
	determine_path(&path, home, fullcmd, info);
	if (path == NULL)
		return (free(oldpath), free(home), ft_printf("cd: HOME not set\n"), 1);//what about malloc error?
	if (change_path(path, fullcmd, info))
		return (free(oldpath), free(home), 1);
	path = getcwd(NULL, 0);
	if (update_pwd(path, oldpath, info))
		return (-1);
	return (free(home), 0);
}

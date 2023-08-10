/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:01:43 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/09 14:44:19 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	determine_path(char **path, char *home, t_fullcmd fullcmd, char ***env)
{
	//malloc protections!!!
	if (fullcmd.argums[1] == NULL || !ft_strcmp(fullcmd.argums[1], "~"))
		*path = ft_strdup(home);
	else if (ft_strncmp(fullcmd.argums[1], "~/", 2) == 0)
		*path = ft_strjoin(home, fullcmd.argums[1] + 1);
	else if (ft_strcmp(fullcmd.argums[1], "-") == 0)
	{
		*path = ft_getenv("OLDPWD", *env);
		ft_printf("%s\n", *path);
	}
	else
		*path = ft_strdup(fullcmd.argums[1]);
}

int	change_path(char *path, t_fullcmd fullcmd)
{
	if (chdir(path) == -1)
	{
		ft_printf("cd: %s: %s\n", strerror(errno), fullcmd.argums[1]);
		free(path);
		return (1);
	}
	free(path);
	return (0);
}

int	update_pwd(char *path, char *oldpath, char ***env)
{
	int	err;

	err = 0;
	oldpath = ft_strjoinfree("export OLDPWD=", oldpath, 2);
	if (!oldpath)
		return (ft_putendl_fd("Memory allocation failed.", STDERR_FILENO), -1);
	path = ft_strjoinfree("export PWD=", path, 2);
	if (!path)
		return (ft_putendl_fd("Memory allocation failed.", STDERR_FILENO), -1);
	err = ft_parseandexec(oldpath, env, NULL);
	if (err)
		return (free(oldpath), free(path), -1);
	err = ft_parseandexec(path, env, NULL);
	free(oldpath);
	free(path);
	return (err);
}

int	ft_cd(t_fullcmd fullcmd, char ***env)
{
	char	*path;
	char	*oldpath;
	char	*home;

	path = NULL;
	home = ft_getenv("HOME", *env);
	if (!home)
		return (-1);
	//oldpath = getcwd(NULL, 0);
	oldpath = ft_getenv("PWD", *env);//changed this so that it works when you do cd .. after removing the directory you were in
	if (!oldpath)
		return (free (home), ft_putendl_fd("Memory allocation failed.", STDERR_FILENO), -1);
	determine_path(&path, home, fullcmd, env);
	if (path == NULL)
		return (free(oldpath), free(home), ft_printf("cd: HOME not set\n"), 1);//what about malloc error?
	if (change_path(path, fullcmd))
		return (free(oldpath), free(home), 1);
	path = getcwd(NULL, 0);
	if (update_pwd(path, oldpath, env))
		return (-1);
	return (free(home), 0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 11:22:17 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/26 12:31:38 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_getquestionmark(void)
{
	char	*value;

	value = ft_itoa(g_exit_code);
	if (!value)
	{
		ft_putendl_fd("Memory allocation failed.", STDERR_FILENO);
		return (NULL);
	}
	return (value);
}

char	*ft_getenv(char *var, char **env)
{
	int		i;
	char	*value;

	i = 0;
	if (*var == '?')
		return (ft_getquestionmark());
	while (env[i])
	{
		if (ft_strncmp(var, env[i], ft_strlen(var)) == 0 && env[i][ft_strlen(var)] == '=')
		{
			value = ft_strdup(&env[i][ft_strlen(var) + 1]);
			if (!value)
				return (ft_putendl_fd(strerror(errno), STDERR_FILENO), NULL);
			return (value);
		}
		i++;
	}
	value = ft_strdup("");
	if (!value)
	{
		ft_putendl_fd("Memory allocation failed.", STDERR_FILENO);
		return (NULL);
	}
	return (value);
}

int	ft_existenv(char *var, char **env)
{
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(var, env[i], ft_strlen(var)) == 0
			&& env[i][ft_strlen(var)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	ft_echo(t_fullcmd fullcmd)
{
	int	i;
	int	flag;

	flag = 0;
	if (fullcmd.argums[1])
	{
		if (!ft_strncmp(fullcmd.argums[1], "-n", ft_strlen(fullcmd.argums[1])) && ft_strlen(fullcmd.argums[1]) == 2)
			flag = 1;
		i = 1 + flag;
		while (fullcmd.argums[i])
		{
			if (write(STDOUT_FILENO, fullcmd.argums[i], ft_strlen(fullcmd.argums[i])) < 0)
				return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
			i++;
			if (fullcmd.argums[i])
			{
				if (write(STDOUT_FILENO, " ", 1) < 0)
					return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
			}
		}
	}
	if (flag == 0)
	{
		if (write(STDOUT_FILENO, "\n", 1) < 0)
			return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	}
	return (0);
}

int	ft_pwd(char **env)
{
	char	*wd;

	wd = ft_getenv("PWD", env);
	if (write(STDOUT_FILENO, wd, ft_strlen(wd)) < 0)
		return (free(wd), ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	if (write(STDOUT_FILENO, "\n", 1) < 0)
		return (free(wd), ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	return (free(wd), 0);
}

void	ft_exportnoarg(char ***env)
{
	int i;

	i = 0;
	while ((*env)[i])
	{
		if (ft_findchar((*env)[i], '='))
			printf("declare -x %s\n", (*env)[i]);
		i++;
	}
}

int	ft_export(t_fullcmd fullcmd, char ***env)
{
	int		i;
	int		len;
	char	*varname;
	int		ok;
	int		varpos;

	//malloc protections!!
	if (!fullcmd.argums[1])
		ft_exportnoarg(env);
	i = 1;
	while (fullcmd.argums[i])
	{
		ok = 0;
		len = 0;
		while (fullcmd.argums[i][len] && fullcmd.argums[i][len] != '=')
		{
			if (ft_isalnum(fullcmd.argums[i][len]) == 0 && fullcmd.argums[i][len] != '_')
			{
				ok = 1;
				ft_putendl_fd("Variable names must only contain alphanumeric characters or underscores.", STDERR_FILENO);//not exactly the same error message, change it?
				break ;
			}
			len++;
		}
		if (ok == 0)
		{
			varname = ft_substr(fullcmd.argums[i], 0, len);
			varpos = ft_existenv(varname, *env);
			free(varname);
			varname = ft_strdup(fullcmd.argums[i]);
			if (varpos >= 0)
			{
				free((*env)[varpos]);
				(*env)[varpos] = varname;
			}
			else
			{
				*env = ft_arrapp(*env, varname);
			}
		}
		i++;
	}
	return (0);
}

char	**ft_arrremove(char **env, int pos)
{
	char	**new;
	int		i;
	int		len;

	len = ft_arrlen(env);
	if (!env || pos >= len || pos < 0)
		return (env);
	new = NULL;
	new = malloc(len * sizeof(char *));
	if (!new)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), NULL);
	i = 0;
	while (i < pos)
	{
		new[i] = env[i];
		i++;
	}
	free(env[i++]);
	while (env[i])
	{
		new[i - 1] = env[i];
		i++;
	}
	new[len - 1] = NULL;
	free(env);
	return (new);
}

int	ft_unset(t_fullcmd fullcmd, char ***env)
{
	int		i;
	int		ok;
	int		len;
	int		varpos;
	char	*varname;

	i = 0;
	ok = 0;
	while (fullcmd.argums[++i])
	{
		len = -1;
		while (fullcmd.argums[i][++len])
		{
			if (ft_isalnum(fullcmd.argums[i][len]) == 0
				&& fullcmd.argums[i][len] != '_')
			{
				ok = 1;
				ft_putendl_fd("Variable names must only contain alphanumeric\
 characters or underscores.", STDERR_FILENO);
				break ;
			}
		}
		if (ok == 0)
		{
			varname = ft_substr(fullcmd.argums[i], 0, len);
			varpos = ft_existenv(varname, *env);
			free(varname);
			*env = ft_arrremove(*env, varpos);
		}
	}
	return (0);
}

int	ft_env(t_fullcmd fullcmd, char ***env)
{
	int	i;

	i = 0;
	if (fullcmd.argums[1] != NULL)
		return (ft_putendl_fd("The builtin env does not take arguments or\
 options.", STDERR_FILENO), -1);
	while ((*env)[i])
	{
		if (ft_findchar((*env)[i], '='))
			printf("%s\n", (*env)[i]);
		i++;
	}
	return (0);
}

void determine_path(char **path, char *home, t_fullcmd fullcmd, char ***env)
{
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
		return (ft_putendl_fd("Memory allocation failed2.", STDERR_FILENO), -1);
	path = ft_strjoinfree("export PWD=", path, 2);
	if (!path)
		return (ft_putendl_fd("Memory allocation failed3.", STDERR_FILENO), -1);
	err = ft_parseandexec(oldpath, env);
	if (err)
		return (free(oldpath), free(path), -1);
	err = ft_parseandexec(path, env);
	free(oldpath);
	free(path);
	return (err);
}

int ft_cd(t_fullcmd fullcmd, char ***env)
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
		return (free (home), ft_putendl_fd("Memory allocation failed1.", STDERR_FILENO), -1);
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

void ft_exit(char *cmd, char ***env)
{
	int i;
	char **argums;

	argums = ft_execargums(cmd, env);
	free(cmd);
	ft_freearr(*env);
	i = 0;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (argums[1] != NULL && argums[2] != NULL)
		return (ft_freearr(argums), ft_putendl_fd("exit: too many arguments", STDERR_FILENO), (void)0);
	if (argums[1] != NULL)
	{
		while (argums[1][i])
		{
			if (ft_isdigit(argums[1][i]) == 0)
			{
				ft_putendl_fd("exit: numeric argument required", STDERR_FILENO);
				ft_freearr(argums);
				exit(255);
			}
			i++;
		}
		g_exit_code = ft_atoi(argums[1]);
		ft_freearr(argums);
		exit(g_exit_code);
	}
	ft_freearr(argums);
	exit(g_exit_code);
}

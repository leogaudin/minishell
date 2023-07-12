/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 11:22:17 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/12 12:38:32 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_getenv(char *var, char **env)
{
	int		i;
	char	*value;

	i = 0;
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
	return (ft_strdup(NULL));//for now I return an empty string when it doesn't find the variable to diff between malloc error and not finding the var.
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
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	if (write(STDOUT_FILENO, "\n", 1) < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	return (0);
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

int	ft_export(t_fullcmd fullcmd, char ***env)
{
	int		i;
	int		len;
	char	*varname;
	int		ok;
	int		varpos;

	//malloc protections!!
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

	if (!env)
		return (env);
	len = ft_arrlen(env);
	if (pos >= len || pos < 0)
		return (env);
	new = malloc(len * sizeof(char *));
	if (!new)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), NULL);
	i = 0;
	while (i < pos)
	{
		new[i] = env[i];
		i++;
	}
	i++;
	while (env[i])
	{
		new[i - 1] = env[i];
		i++;
	}
	new[len] = NULL;
	return (new);
}

int	ft_unset(t_fullcmd fullcmd, char ***env)
{
	int		i;
	char	*varname;
	int		ok;
	int		len;
	int		varpos;

	i = 0;
	while (fullcmd.argums[i])
	{
		len = 0;
		while (fullcmd.argums[i][len] && fullcmd.argums[i][len] != '=')
		{
			if (ft_isalnum(fullcmd.argums[i][len]) == 0 && fullcmd.argums[i][len] != '_')
			{
				ok = 1;
				ft_putendl_fd("Variable names must only contain alphanumeric characters or underscores.", STDERR_FILENO);
				break ;
			}
			len++;
		}
		if (ok == 0)
		{
			varname = ft_substr(fullcmd.argums[i], 0, len);
			varpos = ft_existenv(varname, *env);
			free(varname);
			if (varpos >= 0)
			{
				*env = ft_arrremove(*env, varpos);
			}
		}
		i++;
	}
	return (0);
}

int	ft_env(t_fullcmd fullcmd, char ***env)
{
	int	i;

	i = 0;
	if (fullcmd.argums[1] != NULL)
		return (ft_putendl_fd("The builtin env does not take arguments or options.", STDERR_FILENO), -1);
	while ((*env)[i])
	{
		//if (getenv()) add condition to not print when not set
		printf("%s\n", (*env)[i]);
		i++;
	}
	ret
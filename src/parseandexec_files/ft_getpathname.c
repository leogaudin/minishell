/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getpathname.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 11:37:27 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/07 12:57:25 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "temp.h"

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
char	*ft_pathname(char *cmd, char **paths)
{
	int		i;	
	char	*cmdpath;
	int		acc;

	i = 0;
	acc = ft_min(access(cmd, F_OK), access(cmd, X_OK));
	if (acc == 0)
		return (ft_strdup(cmd));
	while (paths[i] && acc != 0)
	{
		cmdpath = ft_strjoin(paths[i], cmd);
		acc = ft_min(access(cmdpath, F_OK), access(cmdpath, X_OK));
		i++;
		if (acc == 0)
			return (cmdpath);
		else if (cmdpath)
			free(cmdpath);
	}
	return (NULL);
}

/* In this function, we first store all the available paths in which we can
 look for commands in the paths variable. We obtain them from the environmemt
 variable PATH. If the command cannot be found, we print an error and set the
 error variable to 1. We also free the allocated memory that we no longer
 need. */
/* char	*ft_getpathname(char *cmd, t_args *args, char **envp)
{
	int		i;
	char	**paths;
	char	*cmdpath;

	i = 0;
	while (strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	cmd = ft_strjoinfree("/", cmd, 2);
	paths = ft_split(envp[i] + 5, ':');
	cmdpath = ft_pathname(cmd, paths);
	if (cmdpath == NULL)
	{
		ft_printf("./pipex: %s: command not found\n", cmd + 1);
		free (cmd);
		ft_freesplit(paths);
		return ((*args).error = 1, strdup(""));
	}
	free (cmd);
	ft_freesplit(paths);
	return (cmdpath);
} */

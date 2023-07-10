/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getpathname.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 11:37:27 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/10 12:52:18 by ysmeding         ###   ########.fr       */
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
char	*ft_pathname(char *cmd, char **paths)
{
	int		i;	
	char	*cmdpath;
	int		acc;

	i = 0;
	acc = ft_min(access(cmd, F_OK), access(cmd, X_OK));
	if (acc == 0)
		return (ft_strdup(cmd));
	acc = ft_min(access(cmd + 1, F_OK), access(cmd + 1, X_OK));
	if (acc == 0)
		return (ft_strdup(cmd + 1));
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

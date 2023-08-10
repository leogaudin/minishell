/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 07:33:23 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/09 07:33:47 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	**ft_execfdpairs(int cmdct)
{
	int	i;
	int	**fds;

	if (cmdct == 1)
		return (NULL);
	fds = malloc((cmdct - 1) * sizeof(int *));
	if (!fds)
		return (ft_putstrerror("malloc: "), NULL);
	if (fds)
	{
		i = 0;
		while (i < cmdct - 1)
		{
			fds[i] = malloc(2 * sizeof(int));
			if (!fds[i])
				return (ft_putstrerror("malloc: "), ft_frfds(fds, i), NULL);
			i++;
		}
	}
	return (fds);
}

int	ft_isbuiltin(char *cmd)
{
	size_t	ln;

	ln = 0;
	(void) cmd;
	ln = ft_strlen(cmd);
	if (!ft_strncmp(cmd, "echo", ft_strlen(cmd)) && ln == ft_strlen("echo"))
		return (0);
	if (!ft_strncmp(cmd, "cd", ft_strlen(cmd)) && ln == ft_strlen("cd"))
		return (0);
	if (!ft_strncmp(cmd, "pwd", ft_strlen(cmd)) && ln == ft_strlen("pwd"))
		return (0);
	if (!ft_strncmp(cmd, "export", ft_strlen(cmd)) && ln == ft_strlen("export"))
		return (0);
	if (!ft_strncmp(cmd, "unset", ft_strlen(cmd)) && ln == ft_strlen("unset"))
		return (0);
	if (!ft_strncmp(cmd, "env", ft_strlen(cmd)) && ln == ft_strlen("env"))
		return (0);
	if (!ft_strncmp(cmd, "exit", ft_strlen(cmd)) && ln == ft_strlen("exit"))
		return (0);
	return (1);
}

int	ft_execcloseall(int **fd, int pipect)
{
	int	i;

	if (fd)
	{
		i = 0;
		while (i < pipect - 1)
		{
			close(fd[i][0]);
			close(fd[i][1]);
			i++;
		}
	}
	return (0);
}

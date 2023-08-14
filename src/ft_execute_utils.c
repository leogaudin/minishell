/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 07:33:23 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/11 10:43:16 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	**ft_execfdpairs(int cmdct, t_gen_info *info)
{
	int	i;
	int	**fds;

	if (cmdct == 1)
		return (NULL);
	fds = malloc((cmdct - 1) * sizeof(int *));
	if (!fds)
		return (ft_putstrerror("malloc: ", info), NULL);
	if (fds)
	{
		i = 0;
		while (i < cmdct - 1)
		{
			fds[i] = malloc(2 * sizeof(int));
			if (!fds[i])
				return (ftme(info), ft_frfds(fds, i), NULL);
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

int	fd_dupin(int fdin, t_gen_info *info)
{
	if (fdin < 0)
		return (ft_putstrerror("open: ", info), -1);
	if (dup2(fdin, STDIN_FILENO) < 0)
		return (ft_putstrerror("dup2: ", info), -1);
	close(fdin);
	return (0);
}

void	ft_exportlastarg(char **argums, t_gen_info *info)
{
	int		len;
	char	*last;
	int		pos;

	len = ft_arrlen(argums);
	last = argums[len - 1];
	pos = ft_existenv("_", info);
	if (pos >= 0)
	{
		free(info->env[pos]);
		info->env[pos] = ft_strjoin("_=", last);
	}
}

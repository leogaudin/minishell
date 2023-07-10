/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 11:22:17 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/10 12:54:42 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	exit(0);
}

int	ft_pwd(void)
{
	char	*wd;

	wd = getenv("PWD");
	if (write(STDOUT_FILENO, wd, ft_strlen(wd)) < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	if (write(STDOUT_FILENO, "\n", 1) < 0)
		return (ft_putendl_fd(strerror(errno), STDERR_FILENO), -1);
	exit(0);
}

/* int ft_cd(t_fullcmd fullcmd)
{

} */
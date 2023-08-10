/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_echo.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:00:34 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/08 12:41:59 by ysmeding         ###   ########.fr       */
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
		if (!ft_strncmp(fullcmd.argums[1], "-n", ft_strlen(fullcmd.argums[1]))
			&& ft_strlen(fullcmd.argums[1]) == 2)
			flag = 1;
		i = 1 + flag;
		while (fullcmd.argums[i])
		{
			write(STDOUT_FILENO, fullcmd.argums[i], \
			ft_strlen(fullcmd.argums[i]));
			i++;
			if (fullcmd.argums[i])
				write(STDOUT_FILENO, " ", 1);
		}
	}
	if (flag == 0 && write(STDOUT_FILENO, "\n", 1) < 0)
		return (ft_putstrerror("write: "), g_exit_code);
	return (g_exit_code);
}

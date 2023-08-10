/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:05:22 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/08 11:54:13 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_env(t_fullcmd fullcmd, char ***env)
{
	int	i;

	i = 0;
	if (fullcmd.argums[1] != NULL)
		return (ft_putendl_fd("The builtin env does not take arguments or\
 options.", STDERR_FILENO), -1);//necessary?
	while (*env && (*env)[i])
	{
		if (ft_findchar((*env)[i], '='))
			printf("%s\n", (*env)[i]);
		i++;
	}
	return (0);
}

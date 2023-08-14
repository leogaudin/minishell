/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:05:22 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/10 14:26:55 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_env(t_fullcmd fullcmd, t_gen_info *info)
{
	int	i;

	i = 0;
	info->exit_code = 0;
	if (fullcmd.argums[1] != NULL)
		return (ft_putendl_fd("The builtin env does not take arguments or\
 options.", STDERR_FILENO), -1);//necessary?
	while (info->env && info->env[i])
	{
		if (ft_findchar(info->env[i], '='))
			printf("%s\n", info->env[i]);
		i++;
	}
	return (info->exit_code);
}

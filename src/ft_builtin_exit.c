/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_exit.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:06:05 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/12 09:53:37 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_exitwitharg(char **argums, t_gen_info *info)
{
	int	i;

	i = 0;
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
	info->exit_code = ft_atoi(argums[1]);
	ft_freearr(argums);
	exit((unsigned char) info->exit_code);
}

void	ft_exit(char *cmd, t_gen_info *info)
{
	char	**argums;

	argums = ft_execargums(cmd, info);
	free(cmd);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (argums[1] != NULL && argums[2] != NULL)
		return (ft_freearr(argums), ft_putendl_fd("exit: too many arguments", \
		STDERR_FILENO), (void)0);
	ft_freearr(info->env);
	destroy_node(info->root);
	if (argums[1] != NULL)
		ft_exitwitharg(argums, info);
	ft_freearr(argums);
	exit((unsigned char) info->exit_code);
}

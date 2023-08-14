/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_pwd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:02:47 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/11 08:59:06 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(t_gen_info *info)
{
	char	*wd;

	info->exit_code = 0;
	wd = ft_getenv("PWD", info);
	if (!wd)
		return (info->exit_code);
	write(STDOUT_FILENO, wd, ft_strlen(wd));
	write(STDOUT_FILENO, "\n", 1);
	return (free(wd), info->exit_code);
}

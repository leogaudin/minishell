/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_pwd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:02:47 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/08 11:20:23 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(char **env)
{
	char	*wd;

	wd = ft_getenv("PWD", env);
	if (!wd)
		return (g_exit_code);
	if (write(STDOUT_FILENO, wd, ft_strlen(wd)) < 0)
		return (free(wd), ft_putstrerror("write: "), g_exit_code);
	if (write(STDOUT_FILENO, "\n", 1) < 0)
		return (free(wd), ft_putstrerror("write: "), g_exit_code);
	return (free(wd), g_exit_code);
}

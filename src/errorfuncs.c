/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorfuncs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 09:19:52 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/10 09:11:13 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_putstrerror(char *str)
{
	g_exit_code = errno;
	printf("g_exit_code now is -> %i", g_exit_code);
	write(2, "Minishell: ", ft_strlen("Minishell: "));
	if (str)
	{
		write(2, str, ft_strlen(str));
	}
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
}

void	ft_puterror(char *error, int code, char *str1, char *str2)
{
	g_exit_code = code;
	write(2, "Minishell: ", ft_strlen("Minishell: "));
	if (str1)
		write(2, str1, ft_strlen(str1));
	if (str2)
		write(2, str2, ft_strlen(str2));
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
}

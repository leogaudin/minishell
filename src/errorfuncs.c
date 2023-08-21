/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errorfuncs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 09:19:52 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/21 14:47:47 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_putstrerror(char *str, t_gen_info *info)
{
	info->exit_code = errno;
	write(2, "Minishell: ", ft_strlen("Minishell: "));
	if (str)
	{
		write(2, str, ft_strlen(str));
	}
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
}

void	ft_puterror(char *error, char *str1, char *str2)
{
	write(2, "Minishell: ", ft_strlen("Minishell: "));
	if (str1)
		write(2, str1, ft_strlen(str1));
	if (str2)
		write(2, str2, ft_strlen(str2));
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
}

int	ft_put_error(char *error, int code, char *str1, char *str2)
{
	write(2, "Minishell: ", ft_strlen("Minishell: "));
	if (str1)
		write(2, str1, ft_strlen(str1));
	if (str2)
		write(2, str2, ft_strlen(str2));
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
	return (code);
}

void	ftme(t_gen_info *info)
{
	ft_putstrerror("malloc: ", info);
}

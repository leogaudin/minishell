/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 11:22:17 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/11 09:20:04 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_getquestionmark(t_gen_info *info)
{
	char	*value;

	value = ft_itoa(info->exit_code);
	if (!value)
	{
		return (ft_putstrerror("malloc: ", info), NULL);
	}
	return (value);
}

char	*ft_getenv(char *var, t_gen_info *info)
{
	int		i;
	char	*value;

	i = 0;
	if (*var == '?')
		return (ft_getquestionmark(info));
	while (info->env[i])
	{
		if (ft_strncmp(var, info->env[i], ft_strlen(var)) == 0 && \
		info->env[i][ft_strlen(var)] == '=')
		{
			value = ft_strdup(&(info->env[i][ft_strlen(var) + 1]));
			if (!value)
				return (ft_putstrerror("malloc: ", info), NULL);
			return (value);
		}
		i++;
	}
	value = ft_strdup("");
	if (!value)
	{
		return (ft_putstrerror("malloc: ", info), NULL);
	}
	return (value);
}

int	ft_existenv(char *var, t_gen_info *info)
{
	int		i;

	i = 0;
	if (!var)
		return (-1);
	while (info->env[i])
	{
		if (ft_strncmp(var, info->env[i], ft_strlen(var)) == 0
			&& info->env[i][ft_strlen(var)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

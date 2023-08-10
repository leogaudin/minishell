/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 11:22:17 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/08 11:56:56 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_getquestionmark(void)
{
	char	*value;

	value = ft_itoa((unsigned char) g_exit_code);
	if (!value)
	{
		return (ft_putstrerror("malloc: "), NULL);
	}
	return (value);
}

char	*ft_getenv(char *var, char **env)
{
	int		i;
	char	*value;

	i = 0;
	if (*var == '?')
		return (ft_getquestionmark());
	while (env[i])
	{
		if (ft_strncmp(var, env[i], ft_strlen(var)) == 0 && \
		env[i][ft_strlen(var)] == '=')
		{
			value = ft_strdup(&env[i][ft_strlen(var) + 1]);
			if (!value)
				return (ft_putstrerror("malloc: "), NULL);
			return (value);
		}
		i++;
	}
	value = ft_strdup("");
	if (!value)
	{
		return (ft_putstrerror("malloc: "), NULL);
	}
	return (value);
}

int	ft_existenv(char *var, char **env)
{
	int		i;

	i = 0;
	if (!var)
		return (-1);
	while (env[i])
	{
		if (ft_strncmp(var, env[i], ft_strlen(var)) == 0
			&& env[i][ft_strlen(var)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 08:49:52 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/12 10:14:51 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_findchar(char *str, char c)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	ft_findcharout(char *str, char c)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
			return (1);
		else if (str[i] == '\"' && ft_findchar(&str[i + 1], '\"'))
		{
			i++;
			while (str[i] != '\"')
				i++;
		}
		else if (str[i] == '\'' && ft_findchar(&str[i + 1], '\''))
		{
			i++;
			while (str[i] != '\'')
				i++;
		}
		i++;
	}
	return (0);
}

char	*ft_straddfree(char *str, char chr, t_gen_info *info)
{
	char	*new;
	int		len;
	int		i;

	if (!str)
		len = 0;
	else
		len = ft_strlen(str);
	new = malloc((len + 2) * sizeof(char));
	if (!new)
	{
		ft_putstrerror("malloc: ", info);
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		new[i] = str[i];
		i++;
	}
	new[i] = chr;
	new[++i] = 0;
	if (str)
		free(str);
	return (new);
}

void	ft_freearr(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	ft_freearr2(char **arr, char **newarr)
{
	if (arr)
		free(arr);
	ft_freearr(newarr);
}

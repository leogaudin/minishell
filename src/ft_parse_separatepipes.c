/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_separatepipes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 08:55:33 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/10 09:51:14 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_getlen_pipe(char *block, int i)
{
	int	len;

	len = 0;
	while (block[i + len] && block[i + len] != '|')
	{
		if ((block[i + len] == '\"') && ft_findchar(&block[i + len + 1], '\"') \
		== 1)
		{
			len++;
			while (block[i + len] != '\"')
				len++;
		}
		else if ((block[i + len] == '\'') && ft_findchar(&block[i + len + 1], \
		'\'') == 1)
		{
			len++;
			while (block[i + len] != '\'')
				len++;
		}
		len++;
	}
	return (len);
}

char	**ft_separatepipes(char *block)
{
	char	**blocksep;
	int		i;
	int		len;
	char	*str;

	i = 0;
	blocksep = NULL;
	while (block[i])
	{
		len = ft_getlen_pipe(block, i);
		str = ft_substr(&block[i], 0, len);
		if (!str)
		{
			if (blocksep)
				ft_freearr(blocksep);
			ft_putendl_fd("Memory allocation failed\n", STDERR_FILENO);
			return (NULL);
		}
		blocksep = ft_arrapp(blocksep, str);
		i += len;
		if (block[i] == '|')
			i++;
	}
	return (blocksep);
}

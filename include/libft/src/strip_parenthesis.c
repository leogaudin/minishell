/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_parenthesis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 09:32:47 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/18 15:12:31 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*strip_parenthesis(char *str)
{
	int i;
	int start;
	int end;
	char *new;

	i = 0;
	start = 0;
	end = ft_strlen(str);
	while (str[i])
	{
		if (str[i] == '(')
			start = i + 1;
		if (str[i] == ')')
			end = i;
		i++;
	}
	new = ft_substr(str, start, end - start);
	if (!new)
		return (ft_malloc_error());
	return (new);
}

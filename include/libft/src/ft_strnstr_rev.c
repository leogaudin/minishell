/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr_rev.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 14:25:07 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/01 14:25:21 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr_rev(const char *big, const char *little, size_t len)
{
	int	i;
	int	j;

	if (little[0] == '\0' || (len == 0 && !big))
		return ((char *)big);
	i = ft_strlen(big) - 1;
	while (big[i] != '\0' && (size_t)i >= 0)
	{
		j = 0;
		while (little[j] != '\0' && (size_t)i + j < len)
		{
			if (big[i + j] == little[j])
			{
				j++;
			}
			else
				break ;
		}
		if (little[j] == '\0')
		{
			return ((char *)big + i);
		}
		i--;
	}
	return (0);
}

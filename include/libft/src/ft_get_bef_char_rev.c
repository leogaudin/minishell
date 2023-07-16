/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_bef_char_rev.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 14:13:14 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/13 15:26:00 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_get_bef_char_rev(char *str, char c)
{
	int		i;
	char	*new;

	i = ft_strlen(str) - 1;
	while (i >= 0 && str[i] != c)
		i--;
	if (i <= 0)
		return (NULL);
	new = ft_strndup(str, i);
	return (new);
}

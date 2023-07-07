/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinfree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 12:47:11 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/07 12:51:18 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "temp.h"

/* This function does a strjoin with the two strings passed to it and then
 dependent on the int passed to it it might free the first, the second or both
 strings. */
char	*ft_strjoinfree(char *begin, char *end, int num)
{
	char	*new;

	new = ft_strjoin(begin, end);
	if (num == 1 || num == 3)
		free(begin);
	if (num == 2 || num == 3)
		free(end);
	return (new);
}

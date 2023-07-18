/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_between_parenthesis.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 20:06:41 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/17 20:32:59 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	is_between_parenthesis(char *str, int index)
{
	int		i;
	int	opened_parenthesis;
	int	closing_parenthesis;

	i = index;
	opened_parenthesis = 0;
	closing_parenthesis = 0;
	while (i >= 0)
	{
		if (str[i] == '(')
			opened_parenthesis++;
		if (str[i] == ')' || str[i] == '(')
			break ;
		i--;
	}
	i = index;
	while (str[i])
	{
		if (str[i] == ')')
			closing_parenthesis++;
		if (str[i] == ')' || str[i] == '(')
			break ;
		i++;
	}
	return (opened_parenthesis > 0 && closing_parenthesis > 0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exists_outside_parenthesis.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 15:39:00 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/18 15:39:19 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	exists_out_parenthesis(char *command, char *token)
{
	int	index;

	index = ft_strnstr_index(command, token, ft_strlen(command));
	if (index && (!is_between_parenthesis(command, index)
			|| exists_out_parenthesis(command + index + ft_strlen(token),
				token)))
		return (true);
	return (false);
}

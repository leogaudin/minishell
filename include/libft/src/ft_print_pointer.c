/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 12:54:07 by lgaudin           #+#    #+#             */
/*   Updated: 2023/06/28 10:05:56 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_pointer(unsigned long long p)
{
	int	count;

	count = 0;
	count += ft_print_string("0x");
	count += ft_print_hexa(p, 'x');
	return (count);
}

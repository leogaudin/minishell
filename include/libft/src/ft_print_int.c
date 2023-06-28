/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 12:54:20 by lgaudin           #+#    #+#             */
/*   Updated: 2023/06/28 10:05:56 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_int(int n)
{
	int	count;

	count = 0;
	if (n == -2147483648)
	{
		ft_putstr_fd("-2147483648", 1);
		return (11);
	}
	if (n < 0)
	{
		ft_putchar_fd('-', 1);
		count++;
		n = -n;
	}
	if (n >= 10)
		count += ft_print_int(n / 10);
	ft_putchar_fd(n % 10 + '0', 1);
	count++;
	return (count);
}

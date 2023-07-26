/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_freefuncs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 11:34:22 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/19 12:17:51 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* This function frees all the memory allocated for the output of the ft_split
 function. */
void	ft_freesplit(char **arrstr)
{
	int	i;

	i = 0;
	while (arrstr[i])
	{
		free(arrstr[i]);
		i++;
	}
	free (arrstr);
}

/* This function frees the memory allocated to the first i entries of the double
 pointer fds and then frees the memory allocated in fds. */
void	ft_frfds(int **fds, int i)
{
	int	n;

	if (fds == NULL)
		return ;
	n = 0;
	while (n < i - 1)
	{
		free(fds[n]);
		n++;
	}
	free (fds);
}

/* This function calls both the ft_frfds function and the ft_freesplit
 function. */
void	ft_frall(char **arrstr, int **fds, int i)
{
	ft_frfds(fds, i);
	ft_freesplit(arrstr);
}

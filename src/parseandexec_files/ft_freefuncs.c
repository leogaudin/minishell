/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_freefuncs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 11:34:22 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/07 12:50:59 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "temp.h"

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
	while (i > 0)
	{
		free(fds[i - 1]);
		i--;
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

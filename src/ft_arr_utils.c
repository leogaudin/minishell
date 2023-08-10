/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arr_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:07:39 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/08 13:59:22 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_arrlen(char **arr)
{
	int	i;

	i = 0;
	if (arr == NULL)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

char	**ft_arrapp(char **arr, char *app)
{
	int		i;
	char	**newarr;

	newarr = malloc((ft_arrlen(arr) + 2) * sizeof(char *));
	if (!newarr)
	{
		ft_freearr(arr);
		ft_putendl_fd("Memory allocation failed.", STDERR_FILENO);
	}
	if (newarr)
	{
		i = 0;
		while (i < ft_arrlen(arr))
		{
			newarr[i] = arr[i];
			i++;
		}
		newarr[i] = app;
		newarr[i + 1] = NULL;
	}
	if (arr)
		free(arr);
	return (newarr);
}

char	**ft_arrremove(char **env, int pos)
{
	char	**new;
	int		i;
	int		len;

	len = ft_arrlen(env);
	if (!env || pos >= len || pos < 0)
		return (env);
	new = NULL;
	new = malloc(len * sizeof(char *));
	if (!new)
		return (ft_putstrerror("malloc: "), NULL);
	i = -1;
	while (++i < pos)
		new[i] = env[i];
	free(env[i++]);
	while (env[i])
	{
		new[i - 1] = env[i];
		i++;
	}
	new[len - 1] = NULL;
	free(env);
	return (new);
}

/* This function appends a null terminated array of strings to another null
 terminated array of strings and null therminates the resulting array of
 strings. */
char	**ft_appendtoarr(char **arr, char **app)
{
	char	**newarr;
	int		lenarr;
	int		lenapp;
	int		i;

	if (arr == NULL)
		return (app);
	lenarr = ft_arrlen(arr);
	lenapp = ft_arrlen(app);
	newarr = malloc((lenarr + lenapp + 1) * sizeof(char *));
	if (!newarr)
		return (ft_freearr(app), ft_freearr(arr), NULL);
	i = 0;
	while (i < lenarr)
	{
		newarr[i] = arr[i];
		i++;
	}
	while (i < lenarr + lenapp)
	{
		newarr[i] = app[i - lenarr];
		i++;
	}
	newarr[i] = NULL;
	return (free(arr), free(app), newarr);
}

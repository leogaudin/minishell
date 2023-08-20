/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arr_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:07:39 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/20 14:24:26 by ysmeding         ###   ########.fr       */
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

char	**ft_arrapp(char **arr, char *app, t_gen_info *info)
{
	int		i;
	char	**newarr;

	newarr = malloc((ft_arrlen(arr) + 2) * sizeof(char *));
	if (!newarr)
	{
		ft_freearr(arr);
		ft_putstrerror("malloc: ", info);
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

char	**ft_arrremove(char **env, int pos, t_gen_info *info)
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
		return (ft_putstrerror("malloc: ", info), NULL);
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
char	**ft_appendtoarr(char **arr, char **app, t_gen_info *info)
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
		return (ft_freearr(app), ft_freearr(arr), ftme(info), NULL);
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

char	**ft_arrdup(char **arr, t_gen_info *info)
{
	int		i;
	int		j;
	char	**new;

	i = 0;
	while (arr[i])
		i++;
	new = malloc((i + 1) * sizeof(char *));
	if (!new)
		return (ft_putstrerror("malloc: ", info), NULL);
	j = 0;
	while (j < i)
	{
		new[j] = ft_strdup(arr[j]);
		if (!new[j])
			return (ft_freearr(new), ft_putstrerror("malloc: ", info), NULL);
		j++;
	}
	new[j] = NULL;
	return (new);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitnotstr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 11:23:10 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/08 12:40:33 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* This function counts the number of arrays in a null termineted array of
 arrays. */
/* int	ft_arrlen(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
} */

/* This function extracts the part of the string between " or ' characters and
 appends it to the array of strings arrstr. */
/* char	**ft_getstring(char **str, char **arrstr)
{
	int		i;
	char	*substr;
	char	**subarr;
	char	quote;

	quote = **str;
	(*str)++;
	i = 0;
	while ((*str)[i] != quote && (*str)[i])
		i++;
	substr = ft_substr((*str), 0, i);
	subarr = malloc(2 * sizeof(char *));
	if (subarr)
	{
		subarr[0] = substr;
		subarr[1] = NULL;
		arrstr = ft_appendtoarr(arrstr, subarr);
	}
	(*str) = (*str) + i + 1;
	return (arrstr);
} */

/* This function splits a given string into an array of strings which are the
 part of the original string in between two c characters (or the begining of
 end of the srting) but does not split the parts of the string in between " or
 ' characters. */
/* char	**ft_splitnotstr(char *str, char c)
{
	int		i;
	char	*substr;
	char	**arrstr;

	arrstr = NULL;
	while (*str)
	{
		i = 0;
		while ((str[i] != '\'' || (str[i] == '\'' && \
				!ft_findchar(&str[i + 1], '\''))) && (str[i] != '\"' || \
				(str[i] == '\"' && !ft_findchar(&str[i + 1], '\"'))) && str[i])
			i++;
		if (i != 0)
		{
			substr = ft_substr(str, 0, i);
			arrstr = ft_appendtoarr(arrstr, ft_split(substr, c));
			free(substr);
			str = str + i;
		}
		if (*str)
			arrstr = ft_getstring(&str, arrstr);
	}
	return (arrstr);
} */

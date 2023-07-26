/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strip_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 19:03:35 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/18 09:53:49 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_has_closing_quote(char *str, int i)
{
	int	j;

	j = i + 1;
	while (str[j] && str[j] != str[i])
		j++;
	return (str[j] == str[i]);
}

static int	ft_count_quotes(char *str, int i, int *count)
{
	static bool	dbl_open;
	static bool	sgl_open;

	while (str[++i])
	{
		if (str[i] == '"')
		{
			if (!dbl_open && sgl_open)
				(*count)++;
			else if (!dbl_open && ft_has_closing_quote(str, i) && !sgl_open)
				dbl_open = true;
			else if (dbl_open && !sgl_open)
				dbl_open = false;
		}
		else if (str[i] == '\'')
		{
			if (!sgl_open && dbl_open)
				(*count)++;
			else if (!sgl_open && ft_has_closing_quote(str, i) && !dbl_open)
				sgl_open = true;
			else if (sgl_open && !dbl_open)
				sgl_open = false;
		}
	}
	return (*count);
}

void static	ft_copy_quotes(char *str, int i, char **new, int *j)
{
	static bool	dbl_open;
	static bool	sgl_open;

	if (str[i] == '"')
	{
		if (!dbl_open && sgl_open)
			(*new)[(*j)++] = str[i];
		else if (!dbl_open && ft_has_closing_quote(str, i) && !sgl_open)
			dbl_open = true;
		else if (dbl_open && !sgl_open)
			dbl_open = false;
	}
	else if (str[i] == '\'')
	{
		if (!sgl_open && dbl_open)
			(*new)[(*j)++] = str[i];
		else if (!sgl_open && ft_has_closing_quote(str, i) && !dbl_open)
			sgl_open = true;
		else if (sgl_open && !dbl_open)
			sgl_open = false;
	}
	else
		(*new)[(*j)++] = str[i];
}

char	*ft_strip_quotes(char *str)
{
	int		i;
	int		j;
	char	*new;
	int		count;

	i = 0;
	j = 0;
	count = 0;
	if (!str)
		return (str);
	ft_count_quotes(str, i - 1, &count);
	new = (char *)malloc(sizeof(char) * (ft_strlen(str) - count + 1));
	if (!new)
		return (free(str), ft_malloc_error());
	while (str[i])
	{
		ft_copy_quotes(str, i, &new, &j);
		i++;
	}
	new[j] = '\0';
	free(str);
	return (new);
}

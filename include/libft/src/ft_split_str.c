/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 13:01:52 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/01 15:07:58 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	strings_count(char const *str, char *token)
{
	int		strings_count;
	char	*str_copy;

	strings_count = 1;
	str_copy = (char *)str;
	while (*str_copy != '\0')
	{
		if (ft_strnstr(str_copy, token, ft_strlen(str_copy)) == str_copy)
		{
			strings_count++;
			str_copy += ft_strlen(token);
		}
		else
			str_copy++;
	}
	return (strings_count);
}

static int	string_length(char const *s, char *token, int i)
{
	int		length;
	char	*strnstr;
	char	*str;

	if (i < 0 || i > (int)ft_strlen(s) || !s || !token)
		return (0);
	length = 0;
	str = (char *)s + i;
	strnstr = ft_strnstr(str, token, ft_strlen(s));
	if (strnstr == NULL)
		return (ft_strlen(str));
	if (strnstr == str || ft_strlen(strnstr) == ft_strlen(token))
		return (ft_strlen(str) - ft_strlen(token));
	length = ft_strlen(str) - ft_strlen(strnstr);
	return (length);
}

static void	free_all(char **result, int index)
{
	while (index-- > 0)
		free(result[index]);
	free(result);
}

char	**ft_split_str(char const *s, char *token)
{
	int		index;
	char	**result;
	int		result_index;
	int		token_length;

	index = 0;
	result_index = -1;
	token_length = ft_strlen(token);
	result = malloc((strings_count(s, token) + 1) * sizeof(char *));
	if (!result)
		return (0);
	while (++result_index < strings_count(s, token))
	{
		result[result_index] = ft_substr(s, index, string_length(s, token,
					index));
		if (!(result[result_index]))
		{
			free_all(result, result_index);
			return (0);
		}
		index += string_length(s, token, index) + token_length;
	}
	result[result_index] = 0;
	return (result);
}

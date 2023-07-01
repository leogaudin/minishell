/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_str_once.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 13:01:52 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/01 15:52:19 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	strings_count(char const *str, char *token)
{
	if (ft_strnstr(str, token, ft_strlen(str)))
		return (2);
	return (1);
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

// static void	free_all(char **result, int index)
// {
// 	while (index-- > 0)
// 		free(result[index]);
// 	free(result);
// }

char	**ft_split_str_once(char const *s, char *token)
{
	char	**result;
	int		result_index;
	int		token_length;

	if (!s || !token)
		return (0);
	result_index = 0;
	token_length = ft_strlen(token);
	result = malloc((strings_count(s, token) + 1) * sizeof(char *));
	if (!result)
		return (0);
	if (strings_count(s, token) == 1)
		result[0] = ft_strdup((char *)s);
	else
	{
		result[0] = ft_substr(s, 0, string_length(s, token, 0));
		result[1] = ft_substr(s, ft_strlen(result[0]) + token_length,
				ft_strlen(s) - ft_strlen(result[0]));
		result_index++;
	}
	result[result_index + 1] = 0;
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_str_once.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 13:01:52 by lgaudin           #+#    #+#             */
/*   Updated: 2023/08/08 11:59:29 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	strings_count(char const *str, char *token)
{
	if (ft_strnstr_index(str, token, ft_strlen(str)) != 0)
		return (2);
	return (1);
}

static int	string_length(char const *s, char *token)
{
	int		end_of_s;
	char	*str;

	if (!s || !token)
		return (0);
	str = (char *)s;
	end_of_s = ft_strnstr_index(str, token, ft_strlen(s));
	if (is_between_parenthesis(str, end_of_s))
	{
		return (end_of_s + ft_strlen(token) + string_length(str + end_of_s
				+ ft_strlen(token), token));
	}
	if (end_of_s == 0)
		return (ft_strlen(str));
	if (end_of_s == (int)ft_strlen(token))
		return (ft_strlen(str) - ft_strlen(token));
	return (end_of_s);
}

char	**ft_split_str_once(char const *s, char *token)
{
	char	**result;

	if (!s || !token)
		return (0);
	result = malloc((strings_count(s, token) + 1) * sizeof(char *));
	if (!result)
		return (ft_malloc_error());
	if (strings_count(s, token) == 1)
		result[0] = ft_strdup((char *)s);
	else
	{
		result[0] = ft_substr(s, 0, string_length(s, token));
		result[1] = ft_substr(s, string_length(s, token)
				+ ft_strlen(token), ft_strlen(s) - ft_strlen(result[0]));
	}
	result[2] = 0;
	return (result);
}

// int	main(void)
// {
// 	char	*s;
// 	char	**result;

// 	s = "     ls       &&     (    pwd    &&    echo    )     ";
// 	result = ft_split_str_once(s, "&&");
// 	ft_printf("%s\n", result[0]);
// 	ft_printf("%s\n", result[1]);
// 	return (0);
// }

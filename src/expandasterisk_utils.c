/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandasterisk_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 11:18:42 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/10 13:22:53 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ft_check_empty_pattern(char **pattern_pieces, t_gen_info *info)
{
	char	*piece;

	if (!pattern_pieces)
	{
		piece = strdup("");
		if (!piece)
		{
			ft_putstrerror("malloc: ", info);
			return (NULL);
		}
		pattern_pieces = ft_arrapp(pattern_pieces, piece, info);
	}
	return (pattern_pieces);
}

void	ft_set_ast(char *str, int **ast)
{
	if (*str == '*')
		ast[0][0] = 1;
	if (str[strlen(str) - 1] == '*')
		ast[0][1] = 1;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_checkredir_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 14:41:29 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/09 14:42:20 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_namefile_complete(char **asterisk_exp_arr)
{
	char	*name_complete;

	name_complete = NULL;
	if (ft_arrlen(asterisk_exp_arr) == 1)
	{
		name_complete = ft_strdup(asterisk_exp_arr[0]);
		if (!name_complete)
		{
			ft_putendl_fd("Minishell: Memory allocation failed.", 2);
			return (ft_freearr(asterisk_exp_arr), NULL);
		}
	}
	else if (ft_arrlen(asterisk_exp_arr) == 0)
		ft_putendl_fd("Minishell: name: No such file or directory.", 2);//cmt1
	else if (ft_arrlen(asterisk_exp_arr) > 1)
		ft_putendl_fd("Minishell: name: Ambiguous redirect.", 2);//cmt2
	return (name_complete);
}
//cmt1: -->change so that name is the actual name
//cmt2: -->change so that name is the actual name

char	*ft_namefile(char *name)
{
	char	**name_div;
	char	*name_complete;
	char	**asterisk_exp_arr;
	int		ast[2];

	ast[0] = 0;
	ast[1] = 0;
	if (ft_findcharout(name, '*') == 1)
	{
		name_div = ft_pattern_pieces(name, ast);
		asterisk_exp_arr = ft_expandasterisk(name_div, ast);
		if (!asterisk_exp_arr)
			return (free(name), NULL);
		name_complete = ft_namefile_complete(asterisk_exp_arr);
	}
	else
		name_complete = ft_strip_quotes(name);
	return (name_complete);
}

int	ft_open_notlast_heredoc(char *delim, char *pipe_block, int j)
{
	int	fd;

	if (ft_findcharout(&pipe_block[j], '<') == 1)
	{
		fd = ft_execheredoc(delim);
		if (fd >= 0)
			close (fd);
	}
	return (0);
}

int	ft_check_heredoc_or_infile(int here, char **name, char *pipe_block, int *j)
{
	if (here == 1)
	{
		if (ft_open_notlast_heredoc(name[0], pipe_block, *j))
			return (-1);
	}
	else
	{
		name[0] = ft_namefile(name[0]);
		if (!name[0] || (name[0] && ft_checkinfile(name[0])))
			return (-1);
	}
	return (0);
}

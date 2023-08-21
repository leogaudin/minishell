/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_checkredir_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 14:41:29 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/21 14:50:22 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_namefile_complete(char **asterisk_exp_arr, t_gen_info *info)
{
	char	*name_complete;

	name_complete = NULL;
	if (ft_arrlen(asterisk_exp_arr) == 1)
	{
		name_complete = ft_strdup(asterisk_exp_arr[0]);
		if (!name_complete)
		{
			ft_putstrerror("malloc: ", info);
			return (ft_freearr(asterisk_exp_arr), NULL);
		}
	}
	else if (ft_arrlen(asterisk_exp_arr) == 0)
	{
		ft_putendl_fd("Minishell: No such file or directory.", 2);
		info->exit_code = 1;
	}
	else if (ft_arrlen(asterisk_exp_arr) > 1)
	{
		ft_putendl_fd("Minishell: Ambiguous redirect.", 2);
		info->exit_code = 1;
	}
	return (name_complete);
}

char	*ft_namefile(char *name, t_gen_info *info)
{
	char	**name_div;
	char	*name_complete;
	char	**asterisk_exp_arr;
	int		ast[2];

	ast[0] = 0;
	ast[1] = 0;
	if (ft_findcharout(name, '*') == 1)
	{
		name_div = ft_pattern_pieces(name, ast, info);
		asterisk_exp_arr = ft_expandasterisk(name_div, ast, info);
		if (!asterisk_exp_arr)
			return (free(name), NULL);
		name_complete = ft_namefile_complete(asterisk_exp_arr, info);
	}
	else
		name_complete = ft_strip_quotes(name);
	return (name_complete);
}

int	ft_open_notlast_heredoc(char *delim, char *block, t_gen_info *info)
{
	int	fd;

	if (ft_findcharout(block, '<') == 1)
	{
		fd = ft_execheredoc(delim, info);
		if (fd >= 0)
			close (fd);
	}
	return (0);
}

int	ft_check_hereorin(int here, char **name, char *pipe_block, t_gen_info *info)
{
	if (here == 1)
	{
		if (ft_open_notlast_heredoc(name[0], pipe_block, info)
			|| info->here_code == 1)
		{
			info->here_code = 0;
			return (free(name[0]), -1);
		}
	}
	else
	{
		name[0] = ft_namefile(name[0], info);
		if (!name[0] || (name[0] && ft_checkinfile(name[0], info)))
			return (free(name[0]), -1);
	}
	return (0);
}

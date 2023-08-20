/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_redirections_heredoc_utils.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 14:10:20 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/20 14:21:39 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_heredocloop(int fdpairwrite, char *delim, t_gen_info *info)
{
	int		same;
	char	*line;

	same = 1;
	while (same != 0)
	{
		ft_printf("\033[0;34m> \033[0m");
		line = get_next_line(0);
		if (!line)
		{
			info->exit_code = 1;
			info->here_code = 1;
			break ;
		}
		else if (*line && *line != '\n')
			same = ft_strncmp(delim, line, ft_strlen(line) - 1);
		if (same != 0)
			write(fdpairwrite, line, ft_strlen(line));
		free(line);
	}
	return (same);
}

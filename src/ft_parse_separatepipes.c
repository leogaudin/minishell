/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_separatepipes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 08:55:33 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/21 17:29:02 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_getlen_pipe(char *block, int i)
{
	int	len;

	len = 0;
	while (block[i + len] && block[i + len] != '|')
	{
		if ((block[i + len] == '\"') && ft_findchar(&block[i + len + 1], '\"') \
		== 1)
		{
			len++;
			while (block[i + len] != '\"')
				len++;
		}
		else if ((block[i + len] == '\'') && ft_findchar(&block[i + len + 1], \
		'\'') == 1)
		{
			len++;
			while (block[i + len] != '\'')
				len++;
		}
		len++;
	}
	return (len);
}

char	**ft_separatepipes(char *block, t_gen_info *info)
{
	char	**blocksep;
	int		i;
	int		len;
	char	*str;

	i = 0;
	blocksep = NULL;
	while (block[i])
	{
		len = ft_getlen_pipe(block, i);
		str = ft_substr(&block[i], 0, len);
		if (!str)
		{
			if (blocksep)
				ft_freearr(blocksep);
			ft_putstrerror("malloc: ", info);
			return (NULL);
		}
		blocksep = ft_arrapp(blocksep, str, info);
		i += len;
		if (block[i] == '|')
			i++;
	}
	return (blocksep);
}

/* Function actually used in ft_parse_checkredir.c and
 ft_parse_structcmds_utils.c */
int	ft_check_redir_inandout_loop(char **block, t_gen_info *info)
{
	int	err;
	int	j;

	err = 0;
	j = 0;
	while (block[0][j])
	{
		ft_skip_str(block, 0, &j);
		if (block[0][j] == '<')
			err = ft_check_redirin(block[0], &j, info);
		else if (block[0][j] == '>')
			err = ft_check_redirout(block[0], &j, info);
		else if (block[0][j] != 0)
			j++;
		if (err)
		{
			free(block[0]);
			block[0] = ft_strdup("");
			return (err);
		}
	}
	return (err);
}

void	ft_get_chars(char *line, int *i, char **new, t_gen_info *info)
{
	if (line[*i] == '"' && ft_findchar(&line[*i + 1], '\"'))
		*i += ft_addwhilenotchar(&line[*i], new, '\"', info);
	else if (line[*i] == '\'' && ft_findchar(&line[*i + 1], '\''))
		*i += ft_addwhilenotchar(&line[*i], new, '\'', info);
	else
	{
		*new = ft_straddfree(*new, line[*i], info);
		(*i)++;
	}
}

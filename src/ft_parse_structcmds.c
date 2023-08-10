/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_structcmds.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:26:47 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/10 09:51:07 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_skip_str(char **blocksep, int i, int *j)
{
	if ((blocksep[i][*j] == '\"') && \
	ft_findchar(&blocksep[i][*j + 1], '\"') == 1)
	{
		(*j)++;
		while (blocksep[i][*j] != '\"')
			(*j)++;
		(*j)++;
	}
	else if ((blocksep[i][*j] == '\'') && \
	ft_findchar(&blocksep[i][*j + 1], '\'') == 1)
	{
		(*j)++;
		while (blocksep[i][*j] != '\'')
			(*j)++;
		(*j)++;
	}
}

char	*ft_putinstruct_in(t_cmd *cmd, char **blocksep, int i, int *j)
{
	int		len;
	char	*name;

	if (*j > 0 && blocksep[i][*j - 1] == '<')
		cmd->rein.herein = 1;
	else
		cmd->rein.rein = 1;
	(*j)++;
	while ((blocksep[i][*j] == ' ' || (blocksep[i][*j] >= 9
		&& blocksep[i][*j] <= 13)))
		(*j)++;
	len = 0;
	name = ft_strip_quotes(ft_getstr(&blocksep[i][*j], &len));
	if (!name)
		return (NULL);
	if (cmd->rein.herein == 1)
		cmd->rein.heredel = name;
	else if (cmd->rein.rein == 1)
		cmd->rein.infile = name;
	*j += len;
	return (name);
}

char	*ft_putinstruct_out(t_cmd *cmd, char **blocksep, int i, int *j)
{
	int		len;
	char	*name;

	if (*j > 0 && blocksep[i][*j - 1] == '>')
		cmd->reout.reoutapp = 1;
	else
		cmd->reout.reout = 1;
	(*j)++;
	while ((blocksep[i][*j] == ' ' || (blocksep[i][*j] >= 9
		&& blocksep[i][*j] <= 13)))
		(*j)++;
	len = 0;
	name = ft_strip_quotes(ft_getstr(&blocksep[i][*j], &len));
	if (!name)
		return (NULL);
	cmd->reout.outfile = name;
	*j += len;
	return (name);
}

int	ft_putinstruct_block(t_cmd **cmds, char **blocksep, int i)
{
	int		j;
	char	*name;

	j = 0;
	while (blocksep[i][j])
	{
		ft_skip_str(blocksep, i, &j);
		if (blocksep[i][j] == '<' && ft_findcharout(&blocksep[i][j + 1], '<')
			== 0)
		{
			name = ft_putinstruct_in(&(cmds[0][i]), blocksep, i, &j);
			if (!name)
				return (-1);
		}
		else if (blocksep[i][j] == '>' && \
				ft_findcharout(&blocksep[i][j + 1], '>') == 0)
		{
			name = ft_putinstruct_out(&(cmds[0][i]), blocksep, i, &j);
			if (!name)
				return (-1);
		}
		else if (blocksep[i][j] != 0)
			j++;
	}
	return (0);
}

t_cmd	*ft_putinstruct(char **blocksep)
{
	int		i;
	t_cmd	*cmds;

	cmds = malloc(ft_arrlen(blocksep) * sizeof(t_cmd));
	if (!cmds)
		return (ft_putendl_fd("Memory allocation failed.", 2), NULL);
	cmds = ft_initcmds(blocksep, cmds);
	if (!cmds)
		return (NULL);
	i = -1;
	while (blocksep[++i])
	{
		if (ft_putinstruct_block(&cmds, blocksep, i))
			return (ft_freecmds(cmds, ft_arrlen(blocksep)), NULL);
	}
	return (cmds);
}
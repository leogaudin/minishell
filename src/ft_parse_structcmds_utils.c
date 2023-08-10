/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_structcmds_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 08:57:05 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/10 09:51:08 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_addwhilenotchar(char *line, char *name, int *len, char c)
{
	name = ft_straddfree(name, line[*len]);
	if (!name)
		return (NULL);
	(*len)++;
	while (line[*len] != c)
	{
		name = ft_straddfree(name, line[*len]);
		if (!name)
			return (NULL);
		(*len)++;
	}
	name = ft_straddfree(name, line[*len]);
	if (!name)
		return (NULL);
	(*len)++;
	return (name);
}

char	*ft_getstr(char *line, int *len)
{
	char	*name;

	name = NULL;
	while ((line[*len] > 32) && (line[*len] != '<' && line[*len] != '>'))
	{
		if (line[*len] == '\"' && ft_findchar(&line[*len + 1], '\"'))
			name = ft_addwhilenotchar(line, name, len, '\"');
		else if (line[*len] == '\'' && ft_findchar(&line[*len + 1], '\''))
			name = ft_addwhilenotchar(line, name, len, '\'');
		else
		{
			name = ft_straddfree(name, line[*len]);
			(*len)++;
		}
		if (!name)
			return (NULL);
	}
	return (name);
}

void	ft_skip_name(char *line, int *i)
{
	if (line[*i] == '\"' && ft_findchar(&line[*i + 1], '\"'))
	{
		(*i)++;
		while (line[*i] != '\"')
			(*i)++;
		(*i)++;
	}
	else if (line[*i] == '\'' && ft_findchar(&line[*i + 1], '\''))
	{
		(*i)++;
		while (line[*i] != '\'')
			(*i)++;
		(*i)++;
	}
	else
		(*i)++;
}

char	*ft_get_cmdarg(char *line)
{
	int		i;
	char	*new;

	i = 0;
	new = strdup("");
	if (!new)
		return (NULL);
	while (line[i])
	{
		while (line[i] && line[i] != '<' && line[i] != '>')
		{
			new = ft_straddfree(new, line[i]);
			if (!new)
				return (NULL);
			i++;
		}
		if (line[i] == '<' || line[i] == '>')
		{
			while (line[i] <= 32 || line[i] == '>' || line[i] == '<')
				i++;
			while (line[i] > 32 && line[i] != '>' && line[i] != '<')
				ft_skip_name(line, &i);
		}
	}
	return (new);
}

t_cmd	*ft_initcmds(char **blocksep, t_cmd *cmds)
{
	int	i;
	int	j;

	i = 0;
	while (blocksep[i])
	{
		cmds[i].rein.herein = 0;
		cmds[i].rein.rein = 0;
		cmds[i].rein.heredel = NULL;
		cmds[i].rein.infile = NULL;
		cmds[i].reout.reoutapp = 0;
		cmds[i].reout.reout = 0;
		cmds[i].reout.outfile = NULL;
		cmds[i].cmdarg = ft_get_cmdarg(blocksep[i]);
		if (!cmds[i].cmdarg)
		{
			j = 0;
			while (j < i)
				free(cmds[j++].cmdarg);
			free(cmds);
			return (NULL);
		}
		i++;
	}
	return (cmds);
}

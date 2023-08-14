/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_structcmds_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 08:57:05 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/13 13:40:39 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_addwhilenotchar(char *line, char **name, char c, t_gen_info *info)
{
	int	len;

	len = 0;
	*name = ft_straddfree(*name, line[len], info);
	if (!*name)
		return (0);
	len++;
	while (line[len] != c)
	{
		*name = ft_straddfree(*name, line[len], info);
		if (!*name)
			return (0);
		len++;
	}
	*name = ft_straddfree(*name, line[len], info);
	if (!*name)
		return (0);
	len++;
	return (len);
}

char	*ft_getstr(char *line, int *len, t_gen_info *info)
{
	char	*name;

	name = NULL;
	while ((line[*len] > 32) && (line[*len] != '<' && line[*len] != '>'))
	{
		if (line[*len] == '\"' && ft_findchar(&line[*len + 1], '\"'))
			*len += ft_addwhilenotchar(line + *len, &name, '\"', info);
		else if (line[*len] == '\'' && ft_findchar(&line[*len + 1], '\''))
			*len += ft_addwhilenotchar(line + *len, &name, '\'', info);
		else
		{
			name = ft_straddfree(name, line[*len], info);
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

char	*ft_get_cmdarg(char *line, t_gen_info *info)
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
			ft_get_chars(line, &i, &new, info);
			if (!new)
				return (NULL);
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

t_cmd	*ft_initcmds(char **blocksep, t_cmd *cmds, t_gen_info *info)
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
		cmds[i].cmdarg = ft_get_cmdarg(blocksep[i], info);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_getargums.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 07:32:02 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/11 09:47:02 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_getlen_str(char *str, int i)
{
	int	len;

	len = 0;
	while (str[i + len] && str[i + len] > 32)
	{
		if (str[i + len] == '\"' && ft_findchar(&str[i + len + 1], '\"'))
		{
			len++;
			while (str[i + len] != '\"')
				len++;
			len++;
		}
		else if (str[i + len] == '\'' && ft_findchar(&str[i + len + 1], '\''))
		{
			len++;
			while (str[i + len] != '\'')
				len++;
			len++;
		}
		else
			len++;
	}
	return (len);
}

char	**get_exp(char **cmd_parts, char *part, t_gen_info *info)
{
	int		ast[2];
	char	**part_div;
	char	**expand_part;

	ast[0] = 0;
	ast[1] = 0;
	part_div = ft_pattern_pieces(part, ast, info);
	expand_part = ft_expandasterisk(part_div, ast, info);
	if (!expand_part)
		return (free(part), NULL);
	if (expand_part && !expand_part[0])
	{
		part = ft_strip_quotes(part);
		if (!part)
			return (NULL);
		cmd_parts = ft_arrapp(cmd_parts, part, info);
	}
	else
	{
		cmd_parts = ft_appendtoarr(cmd_parts, expand_part, info);
		free(part);
		if (!cmd_parts)
			return (ftme(info), free(part), NULL);
	}
	return (cmd_parts);
}

char	**ft_exp_noq(char **cmd_parts, char *cmdarg, int len, t_gen_info *info)
{
	char	*part;

	part = ft_substr(cmdarg, 0, len);
	if (!part)
		return (ftme(info), NULL);
	if (ft_findcharout(part, '*') == 1)
		cmd_parts = get_exp(cmd_parts, part, info);
	else
	{
		part = ft_strip_quotes(part);
		if (!part)
			return (NULL);
		cmd_parts = ft_arrapp(cmd_parts, part, info);
	}
	return (cmd_parts);
}

char	**ft_splitcmdline(char *cmdarg, t_gen_info *info)
{
	char	**cmd_parts;
	int		i;
	int		len;

	i = 0;
	cmd_parts = NULL;
	if (cmdarg && !*cmdarg)
	{
		cmd_parts = ft_arrapp(cmd_parts, strdup(""), info);
		return (cmd_parts);
	}
	while (cmdarg[i])
	{
		len = ft_getlen_str(cmdarg, i);
		if (len)
		{
			cmd_parts = ft_exp_noq(cmd_parts, &cmdarg[i], len, info);
			if (!cmd_parts)
				return (NULL);
		}
		i += len;
		while (cmdarg[i] && cmdarg[i] <= 32)
			i++;
	}
	return (cmd_parts);
}

char	**ft_execargums(char *cmdarg, t_gen_info *info)
{
	char	**argums;
	int		i;

	argums = ft_splitcmdline(cmdarg, info);
	if (argums && argums[0] && ft_isbuiltin(argums[0]))
	{
		argums[0] = ft_execgetpathname(argums[0], info);
		if (!argums[0])
		{
			i = 1;
			while (argums[i])
			{
				free(argums[i]);
				i++;
			}
			free(argums);
			return (NULL);
		}
	}
	return (argums);
}

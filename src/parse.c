/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:26:47 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/09 09:36:54 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



int	ft_arrlen(char **arr)
{
	int	i;

	i = 0;
	if (arr == NULL)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

int	ft_findchar(char *str, char c)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

void	ft_freearr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	ft_freearr2(char **arr, char **newarr)
{
	if (arr)
		free(arr);
	ft_freearr(newarr);
}

char	**ft_arrapp(char **arr, char *app)
{
	int		i;
	char	**newarr;

	newarr = malloc((ft_arrlen(arr) + 2) * sizeof(char *));
	if (!newarr)
		ft_freearr(arr);
	if (newarr)
	{
		i = 0;
		while (i < ft_arrlen(arr))
		{
			newarr[i] = arr[i];
			i++;
		}
		newarr[i] = app;
		newarr[i + 1] = NULL;
	}
	if (arr)
		free(arr);
	return (newarr);
}

char	**ft_separatepipes(char *block)
{
	char 	**blocksep;
	int		i;
	int		len;
	int		let;
	int		opens;
	int		opend;
	char	*str;

	i = 0;
	opens = 0;
	opend = 0;
	blocksep = NULL;
	while (block[i])
	{
		len = 0;
		while (block[i + len] && block[i + len] != '|')
		{
			if ((block[i + len] == '\"') && ft_findchar(&block[i + len + 1], '\"') == 1)
			{
				len++;
				while (block[i + len] != '\"')
					len++;
			}
			else if ((block[i + len] == '\'') && ft_findchar(&block[i + len + 1], '\'') == 1)
			{
				len++;
				while (block[i + len] != '\'')
					len++;
			}
			len++;
		}
		str = ft_substr(&block[i], 0, len);
		blocksep = ft_arrapp(blocksep, str);
		i += len;
		if (block[i] == '|')
			i++;
	}
	return (blocksep);
}

char	*ft_getname(char *line, int *len)
{
	int i;
	int j;
	char *name;

	if (line[*len] == '\"' && ft_findchar(&line[*len + 1], '\"'))
	{
		(*len)++;
		i = 0;
		while (line[*len + i] != '\"')
			i++;
		name = malloc((i + 1) * sizeof(char));
		if (!name)
			return (NULL);
		j = 0;
		while (j < i)
		{
			name[j] = line[*len + j];
			j++;
		}
		name[j] = 0;
		(*len) += i + 1;
	}
	else if (line[*len] == '\'' && ft_findchar(&line[*len + 1], '\''))
	{
		(*len)++;
		i = 0;
		while (line[*len + i] != '\'')
			i++;
		name = malloc((i + 1) * sizeof(char));
		if (!name)
			return (NULL);
		j = 0;
		while (j < i)
		{
			name[j] = line[*len + j];
			j++;
		}
		name[j] = 0;
		(*len) += i + 1;
	}
	else
	{
		i = 0;
		while ((line[i] > 32) && (line[i] != '<' && line[i] != '>' && line[i] != '*' && line[i] != '|' && line[i] != '\'' && line[i] != '"'))
			i++;
		name = malloc((i + 1) * sizeof(char));
		if (!name)
			return (NULL);
		j = 0;
		while (j < i)
		{
			name[j] = line[*len + j];
			j++;
		}
		name[j] = 0;
		(*len) += i;
	}
	return (name);
}

char *straddfree(char *str, char chr)
{
	char *new;
	int len;
	int i;

	if (!str)
		len = 0;
	else
		len = ft_strlen(str);
	new = malloc((len + 2) * sizeof(char));
	if (!new)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new[i] = str[i];
		i++;
	}
	new[i] = chr;
	new[++i] = 0;
	if (str)
		free(str);
	return (new);
}

char *ft_get_cmdarg(char *line)
{
	int i;
	char *new;

	i = 0;
	new = NULL;
	while (line[i])
	{
		while (line[i] && line[i] != '<' && line[i] != '>')
		{
			new = straddfree(new, line[i]);
			i++;
		}
		if (line[i] == '<' || line[i] == '>')
		{
			while (line[i] == ' ' || (line[i] >= 9 && line[i] <= 13) || line[i] == '>' || line[i] == '<')
				i++;
			if (line[i] == '\"' && ft_findchar(&line[i + 1], '\"'))
			{
				i++;
				while (line[i] != '\"')
					i++;
				i++;
			}
			else if (line[i] == '\'' && ft_findchar(&line[i + 1], '\''))
			{
				i++;
				while (line[i] != '\'')
					i++;
				i++;
			}
			else
			{
				while ((line[i] > 32) && (line[i] != '<' && line[i] != '>' && line[i] != '*' && line[i] != '|' && line[i] != '\'' && line[i] != '"'))//what if " is part of string?
					i++;
			}
		}
	}
	return (new);
}

t_cmd *ft_putinstruct(char **blocksep)
{
	int		i;
	int 	j;
	int		len;
	t_cmd	*cmds;
	char *name;

	cmds = malloc(ft_arrlen(blocksep) * sizeof(t_cmd));
	//add malloc protection!!!
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
		j = 0;
		while (blocksep[i][j])
		{
			if (blocksep[i][j] == '<' && ft_findchar(&blocksep[i][j + 1], '<') == 0)
			{
				if (j > 0 && blocksep[i][j - 1] == '<')
				{
					cmds[i].rein.herein = 1;
					j++;
					while ((blocksep[i][j] == ' ' || (blocksep[i][j] >= 9 && blocksep[i][j] <= 13)) && (blocksep[i][j] != '<' && blocksep[i][j] != '>' && blocksep[i][j] != '*' && blocksep[i][j] != '|'))
						j++;
					len = 0;
					name = ft_getname(&blocksep[i][j], &len);
					cmds[i].rein.heredel = name;
					j += len;
				}
				else
				{
					cmds[i].rein.rein = 1;
					j++;
					while ((blocksep[i][j] == ' ' || (blocksep[i][j] >= 9 && blocksep[i][j] <= 13)) && (blocksep[i][j] != '<' && blocksep[i][j] != '>' && blocksep[i][j] != '*' && blocksep[i][j] != '|'))
						j++;
					len = 0;
					name = ft_getname(&blocksep[i][j], &len);
					cmds[i].rein.infile = name;
					j += len;
				}
			}
			else if (blocksep[i][j] == '>' && ft_findchar(&blocksep[i][j + 1], '>') == 0)
			{
				if (j > 0 && blocksep[i][j - 1] == '>')
				{
					cmds[i].reout.reoutapp = 1;
					j++;
					while ((blocksep[i][j] == ' ' || (blocksep[i][j] >= 9 && blocksep[i][j] <= 13)) && (blocksep[i][j] != '<' && blocksep[i][j] != '>' && blocksep[i][j] != '*' && blocksep[i][j] != '|'))
						j++;
					len = 0;
					name = ft_getname(&blocksep[i][j], &len);
					cmds[i].reout.outfile = name;
					j += len;
				}
				else
				{
					cmds[i].reout.reout = 1;
					j++;
					while ((blocksep[i][j] == ' ' || (blocksep[i][j] >= 9 && blocksep[i][j] <= 13)) && (blocksep[i][j] != '<' && blocksep[i][j] != '>' && blocksep[i][j] != '*' && blocksep[i][j] != '|'))
						j++;
					len = 0;
					name = ft_getname(&blocksep[i][j], &len);
					cmds[i].reout.outfile = name;
					j += len;
				}
			}
			else
				j++;
		}
		i++;
	}
	return (cmds);
}

void	ft_parseexec(char *line, char **env)
{
	char **res;
	t_cmd	*cmds;

	res = ft_separatepipes(line);
	cmds = ft_putinstruct(res);
	ft_executer(cmds, ft_arrlen(res), env);
}

/* int	main(int argc, char **argv, char **env)
{
	ft_parseexec(argv[1], env);
} */

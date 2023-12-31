/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandvar.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 08:27:23 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/21 14:48:57 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * @param line : the line in which we want to replace avariable name by its
 *  value.
 * @param varname : the variable name that we want to replace by its value.
 * @param pos : the position of the $ preceeding the varaible name we want to
 *  replace.
 * @param len : the length of the variable name.
 *
 * In this function we replace a variable name by its value. We first put the
 *  part of the string that comes before the variable in the string variable
 *  begin, then we get the string value of the variable with the getenv function
 * 	We join these strings together and then join the result with the part of the
 * 	line that comes after the variable name. This final string is the result
 * 	that this function returns.
 */
char	*ft_replace(char *line, char *varname, int *pos, t_gen_info *info)
{
	char	*begin;
	char	*var;
	char	*beginvar;
	char	*end;
	char	*beginvarend;

	begin = ft_substr(line, 0, *pos - 1);
	if (!begin)
		return (free(line), ftme(info), NULL);
	var = ft_getenv(varname, info);
	if (!var)
		return (free(line), free(begin), NULL);
	beginvar = ft_strjoin(begin, var);
	if (!beginvar)
		return (free (var), free(line), free(begin), ftme(info), NULL);
	end = ft_substr(line, *pos + ft_strlen(varname), \
	ft_strlen(line) - *pos - ft_strlen(varname));
	if (!end)
		return (free (var), free(line), free(begin), free(beginvar), NULL);
	beginvarend = ft_strjoin(beginvar, end);
	*pos = ft_strlen(begin) + ft_strlen (var) -1;
	free(beginvar);
	return (free (var), free(line), free(begin), free(end), beginvarend);
}

char	*ft_replacewave(char *line, int *pos, t_gen_info *info)
{
	char	*begin;
	char	*var;
	char	*beginvar;
	char	*end;
	char	*beginvarend;

	begin = ft_substr(line, 0, *pos);
	if (!begin)
		return (free(line), ftme(info), NULL);
	var = ft_getenv("HOME", info);
	if (!var)
		return (free(line), free(begin), NULL);
	beginvar = ft_strjoin(begin, var);
	if (!beginvar)
		return (free (var), free(line), free(begin), ftme(info), NULL);
	end = ft_substr(line, *pos + 1, \
	ft_strlen(line) - *pos - 1);
	if (!end)
		return (free (var), free(line), free(begin), free(beginvar), NULL);
	beginvarend = ft_strjoin(beginvar, end);
	*pos = ft_strlen(begin) + ft_strlen (var) - 1;
	free(beginvar);
	return (free (var), free(line), free(begin), free(end), beginvarend);
}

/**
 * @param i : pointer to int that initially holds the position where a '$' was
 *  encountered, at the end we set it.............
 *
 * In this function we count the number of alphanumeric characters and
 *  underscores after the dollar sign as variable names can only contain those
 *  characters. We then make a sub string of the line only containing the
 *  variable name. We then replace expline by the result of the ft_replace
 *  function that replaces the variable name by its value. This string is also
 *  the return value of this function.
 */
char	*ft_getvar_and_replace(int *i, char *expline, t_gen_info *info)
{
	int		len;
	char	*var;

	(*i)++;
	len = 0;
	if (expline[*i + len] == '?')
		var = ft_substr(&expline[*i], 0, 1);
	else
	{
		while ((expline[*i + len] >= '0' && expline[*i + len] <= '9') || \
				(expline[*i + len] >= 'a' && expline[*i + len] <= 'z') || \
				(expline[*i + len] >= 'A' && expline[*i + len] <= 'Z') || \
				expline[*i + len] == '_')
			len++;
		var = ft_substr(&expline[*i], 0, len);
	}
	if (!var)
		return (free(expline), ft_putstrerror("malloc: ", info), NULL);
	else if (!*var)
		return ((*i)--, expline);
	expline = ft_replace(expline, var, i, info);
	free(var);
	return (expline);
}

int	ft_expandvar_loopfunc(char **line, int *i, t_openq *open, t_gen_info *info)
{
	if (line[0][*i] == '\'' && open->opens == 0 && open->opend == 0 && \
		ft_findchar(&line[0][*i + 1], '\'') == 1)
		open->opens = 1;
	else if (line[0][*i] == '\'' && open->opens == 1)
		open->opens = 0;
	if (line[0][*i] == '\"' && open->opens == 0 && open->opend == 0 && \
		ft_findchar(&line[0][*i + 1], '\"') == 1)
		open->opend = 1;
	else if (line[0][*i] == '\"' && open->opend == 1)
		open->opend = 0;
	if (line[0][*i] == '$' && open->opens != 1)
	{
		line[0] = ft_getvar_and_replace(i, line[0], info);
		if (!line[0])
			return (-1);
	}
	else if (line[0][*i] == '~' && (line[0][*i + 1] <= 32 || \
	line[0][*i + 1] == '/') && open->opens == 0 && open->opend == 0)
	{
		line[0] = ft_replacewave(line[0], i, info);
		if (!line[0])
			return (-1);
	}
	return (0);
}

/**
 * @param line : line in which we want to replace variable names by their
 *  corresponding string value.
 *
 * In this function we replace all variable names by their string value unless
 *  it is inside a string delimited by single quotation marks. We duplicate the
 *  original line and traverse it until we find a '$' character. If it is not
 *  inside a single quoted string, we call the function ft_getvar_and_replace to
 *  replace the variable by its name inside the line.
 */
char	*ft_expandvar(char *line, t_gen_info *info)
{
	int		i;
	t_openq	open;
	char	*expline;

	i = -1;
	open.opens = 0;
	open.opend = 0;
	expline = ft_strdup(line);
	while (expline && expline[++i])
	{
		if (ft_expandvar_loopfunc(&expline, &i, &open, info))
			break ;
	}
	free(line);
	return (expline);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandasterisk.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 08:26:47 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/18 08:26:53 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * @param filename : name of the file that we want to match with a certain
 *  pattern.
 * @param pattern : pattern we want to try and find in the name of a file.
 * 
 * We first split the pattern into its parts separated by '*'. Then we use the
 *  function ft_strnstr to see if the different parts of the pattern can be
 *  found in the given name in the order that they appear in the pattern.
 */
int	ft_patternfound(char *filename, char **pattern_pieces, int *ast)
{
	int		m;
	char	*found;

	m = 0;
	found = filename;
	while (found && *found && pattern_pieces[m])
	{
		if ((m == 0 && ast[0] == 0) && ft_strncmp(filename, pattern_pieces[m],\
			ft_strlen(pattern_pieces[m])))
			return (0);
		else if ((pattern_pieces[m + 1] == NULL && ast[1] == 0) && \
				(ft_strlen(filename) - ft_strlen(pattern_pieces[m]) < 0 || \
				ft_strncmp(&filename[ft_strlen(filename) - \
				ft_strlen(pattern_pieces[m])], pattern_pieces[m], \
				ft_strlen(pattern_pieces[m]))))
			return (0);
		found = ft_strnstr(found, pattern_pieces[m], ft_strlen(found));
		if (found == NULL)
			return (0);
		found = found + ft_strlen(pattern_pieces[m]);
		m++;
	}
	if (pattern_pieces[m] != NULL)
		return (0);
	return (1);
}

/**
 * @param pattern : the pattern we are looking for in the filenames of the
 *  current directory.
 * 
 * We use the functions opendir, readir and closedir to find the files and
 *  directories in the current directory that have a name that matches with the
 *  pattren passed to the function as argument. The names of files and
 *  directories that match are put in an array and returned by the function.
 */
char	**ft_expandasterisk(char **pattern_pieces, int *ast)
{
	DIR				*dirp;
	char			**files;
	char			*filename;
	struct dirent	*result;

	dirp = opendir(".");
	result = readdir(dirp);
	files = NULL;
	while (result != NULL)
	{
		filename = result->d_name;
		if (ft_patternfound(filename, pattern_pieces, ast) == 1)
			files = ft_arrapp(files, filename);
		result = readdir(dirp);
	}
	closedir(dirp);
	return (files);
}

int	ft_getlenast(char *str, int i)
{
	int	len;

	len = 0;
	while (str[i + len] && str[i + len] != '*')
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

char **ft_pattern_pieces(char *str, int *ast)
{
	int		i;
	int		len;
	char	**pattern_pieces;
	char	*piece;

	i = 0;
	pattern_pieces = NULL;
	if (*str == '*')
		ast[0] = 1;
	if (str[strlen(str) - 1] == '*')
		ast[1] = 1;
	while (str[i])
	{
		len = ft_getlenast(str, i);
		if (len)
		{
			piece = ft_strip_quotes(ft_substr(&str[i], 0, len));
			pattern_pieces = ft_arrapp(pattern_pieces, piece);
		}
		i += len;
		while (str[i] == '*')
			i++;
	}
	return (pattern_pieces);
}
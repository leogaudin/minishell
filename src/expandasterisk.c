/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandasterisk.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 08:26:47 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/09 12:50:06 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * @param 
 * @param 
 * 
 * 
 */
int	ft_patternfound(char *filename, char **pattern_pieces, int *ast)
{
	int		m;
	char	*found;

	m = 0;
	found = filename;
	while (found && *found && pattern_pieces[m])
	{
		if ((m == 0 && ast[0] == 0) && ft_strncmp(filename, pattern_pieces[m], \
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

char	**ft_addfilename_to_arr(char **files, char *filename, DIR *dirp)
{
	char			*filename_dup;

	filename_dup = ft_strdup(filename);
	if (!filename_dup)
	{
		ft_putendl_fd("Minishell: Memory allocation failed.", 2);
		return (ft_freearr(files), NULL);
	}
	files = ft_arrapp(files, filename_dup);
	if (!files)
	{
		closedir(dirp);
		return (NULL);
	}
	return (files);
}

/**
 * @param pattern : 
 * 
 * 
 */
char	**ft_expandasterisk(char **pattern_pieces, int *ast)
{
	DIR				*dirp;
	char			**files;
	char			*filename;
	struct dirent	*result;

	if (pattern_pieces == NULL)
		return (NULL);
	dirp = opendir(".");
	result = readdir(dirp);
	files = ft_calloc(1, sizeof(char *));
	if (!files)
		return (ft_freearr(pattern_pieces), files);
	while (result != NULL)
	{
		filename = result->d_name;
		if (ft_patternfound(filename, pattern_pieces, ast) == 1)
		{
			files = ft_addfilename_to_arr(files, filename, dirp);
			if (!files)
				return (ft_freearr(pattern_pieces), NULL);
		}
		result = readdir(dirp);
	}
	closedir(dirp);
	return (ft_freearr(pattern_pieces), files);
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

char	**ft_pattern_pieces(char *str, int *ast)
{
	int		i;
	int		len;
	char	**pattern_pieces;
	char	*piece;

	i = 0;
	pattern_pieces = NULL;
	ft_set_ast(str, &ast);
	while (str[i])
	{
		len = ft_getlenast(str, i);
		if (len)
		{
			piece = ft_strip_quotes(ft_substr(&str[i], 0, len));
			pattern_pieces = ft_arrapp(pattern_pieces, piece);
			if (!pattern_pieces)
				return (NULL);
		}
		i += len;
		while (str[i] == '*')
			i++;
	}
	pattern_pieces = ft_check_empty_pattern(pattern_pieces);
	return (pattern_pieces);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandasterisk.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 08:26:47 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/10 13:32:10 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * @param 
 * @param 
 * 
 * 
 */
int	ft_patternfound(char *filename, char **pieces, int *ast)
{
	int		m;
	char	*found;

	m = 0;
	found = filename;
	while (found && *found && pieces[m])
	{
		if ((m == 0 && ast[0] == 0) && ft_strncmp(filename, pieces[m], \
			ft_strlen(pieces[m])))
			return (0);
		else if ((pieces[m + 1] == NULL && ast[1] == 0) && \
				(ft_strlen(filename) - ft_strlen(pieces[m]) < 0 || \
				ft_strncmp(&filename[ft_strlen(filename) - \
				ft_strlen(pieces[m])], pieces[m], \
				ft_strlen(pieces[m]))))
			return (0);
		found = ft_strnstr(found, pieces[m], ft_strlen(found));
		if (found == NULL)
			return (0);
		found = found + ft_strlen(pieces[m]);
		m++;
	}
	if (pieces[m] != NULL)
		return (0);
	return (1);
}

char	**ft_addfile_arr(char **files, char *file, DIR *dirp, t_gen_info *info)
{
	char			*file_dup;

	file_dup = ft_strdup(file);
	if (!file_dup)
	{
		ft_putstrerror("malloc: ", info);
		return (ft_freearr(files), NULL);
	}
	files = ft_arrapp(files, file_dup, info);
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
char	**ft_expandasterisk(char **pieces, int *ast, t_gen_info *info)
{
	DIR				*dirp;
	char			**files;
	char			*filename;
	struct dirent	*result;

	if (pieces == NULL)
		return (NULL);
	dirp = opendir(".");
	result = readdir(dirp);
	files = ft_calloc(1, sizeof(char *));
	if (!files)
		return (ft_freearr(pieces), ft_putstrerror("malloc: ", info), files);
	while (result != NULL)
	{
		filename = result->d_name;
		if (ft_patternfound(filename, pieces, ast) == 1)
		{
			files = ft_addfile_arr(files, filename, dirp, info);
			if (!files)
				return (ft_freearr(pieces), NULL);
		}
		result = readdir(dirp);
	}
	closedir(dirp);
	return (ft_freearr(pieces), files);
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

char	**ft_pattern_pieces(char *str, int *ast, t_gen_info *info)
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
			pattern_pieces = ft_arrapp(pattern_pieces, piece, info);
			if (!pattern_pieces)
				return (NULL);
		}
		i += len;
		while (str[i] == '*')
			i++;
	}
	pattern_pieces = ft_check_empty_pattern(pattern_pieces, info);
	return (pattern_pieces);
}

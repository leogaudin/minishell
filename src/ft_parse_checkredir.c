/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_checkredir.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 14:39:37 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/09 14:44:45 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_checkinfile(char *name)
{
	if (access(name, F_OK) != 0)
		return (ft_putendl_fd(strerror(errno), 2), -1);
	if (access(name, R_OK) != 0)
		return (ft_putendl_fd(strerror(errno), 2), -1);
	free(name);
	return (0);
}

int	ft_check_redirin(char *pipe_block, int *j)
{
	int		n;
	char	*name;
	int		here;

	n = 0;
	if (pipe_block[*j + 1] == '<')
		n = 1;
	here = n;
	(*j)++;
	while (pipe_block[*j + n] && (pipe_block[*j + n] <= 32))
		n++;
	if (!pipe_block[*j + n] || pipe_block[*j + n] == '<'
		|| pipe_block[*j + n] == '>')
		return (ft_putendl_fd("Minishell: Syntax error near unexpected token\
 '<'.", 2), -1);
	name = ft_getstr(&pipe_block[*j], &n);
	*j += n;
	if (!name)
		return (-1);
	if (ft_check_heredoc_or_infile(here, &name, pipe_block, j))
		return (-1);
	return (0);
}

int	ft_checkoutfile(char *name, int app)
{
	int	fd;

	if (app == 0 && access(name, F_OK) == 0)
		fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(name, O_WRONLY | O_CREAT, 0644);
	if (fd < 0)
	{
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (free(name), -1);
	}
	close (fd);
	free(name);
	return (0);
}

int	ft_check_redirout(char *pipe_block, int *j)
{
	int		n;
	int		app;
	char	*name;

	n = 0;
	app = 0;
	if (pipe_block[*j + 1] == '>')
	{
		n = 1;
		app = 1;
	}
	(*j)++;
	while (pipe_block[*j + n] && (pipe_block[*j + n] <= 32))
		n++;
	if (!pipe_block[*j + n] || pipe_block[*j + n] == '<'
		|| pipe_block[*j + n] == '>')
		return (ft_putendl_fd("Minishell: Syntax error near unexpected token\
 '>'.", 2), -1);
	name = ft_getstr(&pipe_block[*j], &n);
	*j += n;
	name = ft_namefile(name);
	if (!name || (name && ft_checkoutfile(name, app)))
		return (-1);
	return (0);
}

int	ft_check_redir_inandout(char **separate_pipe, char	***env)
{
	int		i;
	int		j;
	int		err;

	i = -1;
	err = 0;
	while (separate_pipe[++i])
	{
		separate_pipe[i] = ft_expandvar(separate_pipe[i], env);
		j = -1;
		while (separate_pipe[i][++j])
		{
			if (separate_pipe[i][j] == '<')
				err = ft_check_redirin(separate_pipe[i], &j);
			else if (separate_pipe[i][j] == '>')
				err = ft_check_redirout(separate_pipe[i], &j);
			if (err)
				separate_pipe[i] = ft_strdup("");
		}
	}
	return (0);
}

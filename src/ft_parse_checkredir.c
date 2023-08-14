/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_checkredir.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 14:39:37 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/13 13:39:44 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_checkinfile(char *name, t_gen_info *info)
{
	char	*ename;

	ename = ft_strjoin(name, ": ");
	if (access(name, F_OK) != 0)
	{
		ft_putstrerror(ename, info);
		info->exit_code = 1;
		return (free(ename), info->exit_code);
	}
	if (access(name, R_OK) != 0)
		return (ft_putstrerror(ename, info), free(ename), info->exit_code);
	free(name);
	free(ename);
	return (0);
}

int	ft_check_redirin(char *pipe_block, int *j, t_gen_info *info)
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
	{
		info->exit_code = ft_put_error("Syntax error near unexpected token \
'<'.", 258, NULL, NULL);
		return (-1);
	}
	name = ft_getstr(&pipe_block[*j], &n, info);
	*j += n;
	if (!name)
		return (-1);
	if (ft_check_hereorin(here, &name, &pipe_block[*j], info))
		return (-1);
	return (0);
}

int	ft_checkoutfile(char *name, int app, t_gen_info *info)
{
	int	fd;

	name = ft_namefile(name, info);
	if (!name)
		return (-1);
	if (app == 0 && access(name, F_OK) == 0)
		fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(name, O_WRONLY | O_CREAT, 0644);
	if (fd < 0)
	{
		ft_putstrerror("open: ", info);
		return (free(name), -1);
	}
	close (fd);
	free(name);
	return (0);
}

int	ft_check_redirout(char *pipe_block, int *j, t_gen_info *info)
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
	{
		info->exit_code = ft_put_error("Syntax error near unexpected token \
'>'.", 258, NULL, NULL);
		return (-1);
	}
	name = ft_getstr(&pipe_block[*j], &n, info);
	*j += n;
	return (ft_checkoutfile(name, app, info));
}

int	ft_check_redir_inandout(char **separate_pipe, t_gen_info *info)
{
	int		i;

	i = -1;
	while (separate_pipe[++i])
	{
		separate_pipe[i] = ft_expandvar(separate_pipe[i], info);
		ft_check_redir_inandout_loop(&separate_pipe[i], info);
	}
	return (0);
}

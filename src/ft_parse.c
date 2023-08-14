/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 08:31:35 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/14 10:27:21 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_first_last_char(char *line, t_gen_info *info)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(line);
	while (len > 0 && line[len] <= 32)
		len--;
	while (line[i] && line[i] <= 32)
		i++;
	if (line[i] == '|' || line[len] == '|')
	{
		info->exit_code = ft_put_error("Syntax error near unexpected token\
		'|'.", 258, NULL, NULL);
		return (-1);
	}
	else if (line[i] == '&' || line[len] == '&')
	{
		info->exit_code = ft_put_error("Syntax error near unexpected token\
		'&'.", 258, NULL, NULL);
		return (-1);
	}
	else if (line[i] == 0)
		return (1);
	return (0);
}

char	*ft_get_pipe_block(char *line, int i)
{
	int		len;
	char	*pipe_block;

	len = 0;
	while (line[i + len] && line[i + len] != '|')
	{
		if ((line[i + len] == '\"') && \
			ft_findchar(&line[i + len + 1], '\"') == 1)
		{
			len++;
			while (line[i + len] != '\"')
				len++;
		}
		else if ((line[i + len] == '\'') && \
				ft_findchar(&line[i + len + 1], '\'') == 1)
		{
			len++;
			while (line[i + len] != '\'')
				len++;
		}
		len++;
	}
	pipe_block = ft_substr(&line[i], 0, len);
	return (pipe_block);
}

char	**ft_separate_pipe(char *line, t_gen_info *info)
{
	char	**separate_pipe;
	char	*pipe_block;
	int		i;

	i = 0;
	separate_pipe = NULL;
	while (line[i])
	{
		pipe_block = ft_get_pipe_block(line, i);
		if (!pipe_block)
		{
			if (separate_pipe)
				ft_freearr(separate_pipe);
			ft_putstrerror("malloc: ", info);
			return (NULL);
		}
		separate_pipe = ft_arrapp(separate_pipe, pipe_block, info);
		if (!separate_pipe)
			return (NULL);
		i += ft_strlen(pipe_block);
		if (line[i] == '|')
			i++;
	}
	return (separate_pipe);
}

int	ft_check_exit(char **separate_pipe, t_gen_info *info)
{
	char	*cmd;
	int		i;

	i = 0;
	cmd = ft_get_cmdarg(separate_pipe[0], info);
	if (!cmd)
		return (ft_putstrerror("malloc: ", info), info->exit_code);
	while (cmd[i] && cmd[i] <= 32)
		i++;
	if (ft_strncmp(&cmd[i], "exit", 4) == 0 && (cmd[i + 4] < 33))
	{
		ft_freearr(separate_pipe);
		ft_exit(cmd, info);
		info->exit_code = 1;
		return (1);
	}
	else
	{
		free(cmd);
	}
	return (0);
}

int	ft_parseandexec(char *line, t_gen_info *info)
{
	char	**separate_pipe;
	int		err;
	int		len;
	t_cmd	*cmds;

	err = check_first_last_char(line, info);
	if (err < 0)
		return (258);//-->error code when syntax error
	else if (err > 0)
		return (0);//-->command line with only whitespace chars, should be exit?
	separate_pipe = ft_separate_pipe(line, info);
	if (!separate_pipe)
		return (-1);
	len = ft_arrlen(separate_pipe);
	ft_check_redir_inandout(separate_pipe, info);
	if (len == 1)
	{
		if (ft_check_exit(separate_pipe, info))
			return (info->exit_code);
	}
	cmds = ft_putinstruct(separate_pipe, info);
	if (!cmds)
		return (ft_freearr(separate_pipe), -1);
	ft_executer(cmds, ft_arrlen(separate_pipe), info);
	return (ft_freearr(separate_pipe), ft_freecmds(cmds, len), info->exit_code);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newparse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 08:31:35 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/26 12:49:29 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_first_last_char(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] <= 32)
		i++;
	if (line[i] == '|')
		return (ft_putendl_fd("Minishell: Syntax error near unexpected token\
 '|'.", STDERR_FILENO), -1);
	else if (line[i] == '&')
		return (ft_putendl_fd("Minishell: Syntax error near unexpected token\
 '&'.", STDERR_FILENO), -1);
	else if (line[i] == 0)
		return (1);
	i = ft_strlen(line);
	while (i >= 0 && line[i] <= 32)
		i--;
	if (line[i] == '|')
		return (ft_putendl_fd("Minishell: Syntax error near unexpected token\
 '|'.", STDERR_FILENO), -1);
	else if (line[i] == '&')
		return (ft_putendl_fd("Minishell: Syntax error near unexpected token\
 '&'.", STDERR_FILENO), -1);
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

char	**ft_separate_pipe(char *line)
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
			ft_putendl_fd("Minishell: Memory allocation failed.", 2);
			return (NULL);
		}
		separate_pipe = ft_arrapp(separate_pipe, pipe_block);
		if (!separate_pipe)
			return (NULL);
		i += ft_strlen(pipe_block);
		if (line[i] == '|')
			i++;
	}
	return (separate_pipe);
}

char	*ft_namefile_complete(char **asterisk_exp_arr)
{
	char	*name_complete;

	name_complete = NULL;
	if (ft_arrlen(asterisk_exp_arr) == 1)
	{
		name_complete = ft_strdup(asterisk_exp_arr[0]);
		if (!name_complete)
		{
			ft_putendl_fd("Minishell: Memory allocation failed.", 2);
			return (ft_freearr(asterisk_exp_arr), NULL);
		}
	}
	else if (ft_arrlen(asterisk_exp_arr) == 0)
		ft_putendl_fd("Minishell: name: No such file or directory.", 2);//-->change so that name is the actual name
	else if (ft_arrlen(asterisk_exp_arr) > 1)
		ft_putendl_fd("Minishell: name: Ambiguous redirect.", 2);//-->change so that name is the actual name
	return (name_complete);
}

char	*ft_namefile(char *name)
{
	char	**name_div;
	char	*name_complete;
	char	**asterisk_exp_arr;
	int		ast[2];

	ast[0] = 0;
	ast[1] = 0;
	if (ft_findcharout(name, '*') == 1)
	{
		name_div = ft_pattern_pieces(name, ast);
		asterisk_exp_arr = ft_expandasterisk(name_div, ast);
		if (!asterisk_exp_arr)
			return (free(name), NULL);
		name_complete = ft_namefile_complete(asterisk_exp_arr);
	}
	else
		name_complete = ft_strip_quotes(name);
	return (name_complete);
}

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
	if (here == 1)
		return (0);
	name = ft_namefile(name);
	if (!name || (name && ft_checkinfile(name)))
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

void	ft_check_exit(char **separate_pipe, char ***env, t_node *root)
{
	char *cmd = ft_get_cmdarg(separate_pipe[0]);

	int i = 0;
	while (cmd[i] && cmd[i] <= 32)
		i++;
	if (ft_strncmp(&cmd[i], "exit", 4) == 0)
	{
		ft_freearr(separate_pipe);
		destroy_node(root);
		ft_exit(cmd, env);
	}
	free(cmd);
	return ;
}

//int	ft_parse_and_execute(char *line, char ***env)
int	ft_parseandexec(char *line, char ***env, t_node *root)
{
	char	**separate_pipe;
	int		err;
	int		len;
	t_cmd	*cmds;

	err = check_first_last_char(line);
	if (err < 0)
		return (258);//-->error code when syntax error
	else if (err > 0)
		return (0);//-->command line with only whitespace chars
	separate_pipe = ft_separate_pipe(line);
	if (!separate_pipe)
		return (-1);
	len = ft_arrlen(separate_pipe);
	ft_check_redir_inandout(separate_pipe, env);
	if (len == 1)
		ft_check_exit(separate_pipe, env, root);
	cmds = ft_putinstruct(separate_pipe);
	if (!cmds)
		return (ft_freearr(separate_pipe), -1);
	if (ft_executer(cmds, ft_arrlen(separate_pipe), env))
		return (ft_freearr(separate_pipe), ft_freecmds(cmds, len), -1);
	return (ft_freearr(separate_pipe), ft_freecmds(cmds, len), 0);
	(void)cmds;
	return (0);
}

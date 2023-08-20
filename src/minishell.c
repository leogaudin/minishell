/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:26:43 by lgaudin           #+#    #+#             */
/*   Updated: 2023/08/20 14:29:32 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_prompt(t_gen_info *info)
{
	char	*user;

	user = ft_getenv("USER", info);
	info->prompt = ft_strjoinfree(user, " > \033[0m", 1);
	info->prompt = ft_strjoinfree("\033[1;32mminishell\033[0m\033[1;34m@\033[0m\
\033[1;32m", info->prompt, 2);
	return (info->prompt);
}

void	sigint_handler(int sig)
{
	(void)sig;
	if (g_code == 42)
	{
		write(1, "\033[K\n", 5);
		rl_replace_line("", 0);
		g_code = 1;
	}
	else if (g_code == 420 || g_code == 10)
	{
		write(1, "\033[K\n", ft_strlen("\033[K\n"));
		close(0);
		g_code /= 10;
	}
	else
	{
		rl_on_new_line();
		rl_redisplay();
		rl_replace_line("", 0);
		write(1, "\033[K\n", 5);
		rl_on_new_line();
		rl_redisplay();
		rl_replace_line("", 0);
	}
}

void	sigquit_handler(int sig)
{
	(void)sig;
	return ;
}

int	ft_changeshlvl(t_gen_info *info)
{
	int		shlvl;
	int		shlvlpos;
	char	*shlvlval;

	shlvlval = ft_getenv("SHLVL", info);
	if (!shlvlval)
		return (-1);
	shlvl = ft_atoi(shlvlval) + 1;
	shlvlpos = ft_existenv("SHLVL", info);
	free(shlvlval);
	shlvlval = ft_itoa(shlvl);
	if (!shlvlval)
		return (ft_putstrerror("malloc: ", info), -1);
	free ((info->env)[shlvlpos]);
	info->env[shlvlpos] = ft_strjoin("SHLVL=", shlvlval);
	free(shlvlval);
	if (!info->env[shlvlpos])
		return (ft_putstrerror("malloc: ", info), -1);
	return (0);
}

void	ft_dothingswithline(char *line, t_gen_info *info)
{
	add_history(line);
	info->root = generate_node_from_command(line);
	execute_node(info->root, info);
	destroy_node(info->root);
	info->root = NULL;
	free(line);
}

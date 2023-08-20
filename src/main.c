/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 14:27:49 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/20 14:33:59 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_code = 1;

void	ft_init_info(t_gen_info *info, char **env)
{
	info->env = NULL;
	info->exit_code = 0;
	info->here_code = 0;
	info->root = NULL;
	info->prompt = NULL;
	info->env = ft_arrdup(env, info);
}

int	main(int argc, char **argv, char **env)
{
	char		*line;
	t_gen_info	info;

	(void)argc;
	(void)argv;
	ft_init_info(&info, env);
	if (info.env == NULL)
		return (info.exit_code);
	if (ft_changeshlvl(&info))
		return (info.exit_code);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline(ft_prompt(&info));
		free (info.prompt);
		if (!line)
		{
			rl_clear_history();
			ft_exit(ft_strdup("exit"), &info);//cmt1
		}
		if (line && *line)
			ft_dothingswithline(line, &info);
	}
	return (0);
}

//cmt1: use NULL instead of strdup and change ft_exit accordingly
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:26:43 by lgaudin           #+#    #+#             */
/*   Updated: 2023/08/20 13:44:56 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int g_code = 1;

// INFO: This function is not part of the project, it is only used to test the
// AST generation and execution.
/* int	main(int argc, char **argv)
{
	t_node	*root;
	int		exit_status;

	(void)argc;
	root = generate_node_from_command(argv[1]);
	exit_status = execute_node(root);
	ft_printf("\nExit status: %d\n", exit_status);
	ft_printf("\n");
	print_ast(root, 0);
	ft_printf("\n");
	destroy_node(root);
	return (0);
} */

char	**ft_arrdup(char **arr, t_gen_info *info)
{
	int		i;
	int		j;
	char	**new;

	i = 0;
	while (arr[i])
		i++;
	new = malloc((i + 1) * sizeof(char *));
	if (!new)
		return (ft_putstrerror("malloc: ", info), NULL);
	j = 0;
	while (j < i)
	{
		new[j] = ft_strdup(arr[j]);
		if (!new[j])
			return (ft_freearr(new), ft_putstrerror("malloc: ", info), NULL);
		j++;
	}
	new[j] = NULL;
	return (new);
}

char	*ft_prompt(t_gen_info *info)
{
	char	*user;

	user = ft_getenv("USER", info);
	info->prompt = ft_strjoinfree(user, " > \033[0m", 1);
	info->prompt = ft_strjoinfree("\033[1;32mminishell\033[0m\033[1;34m@\033[0m\
\033[1;32m", info->prompt, 2);
	return (info->prompt);
}

void sigint_handler(int sig)
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
		write(1, "normal\n", 7);
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

/* void check_leaks(void)
{
system("lsof  -p $$");
system("leaks minishell");
} */

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

int main(int argc, char **argv, char **env)
{
	char		*line;
	t_gen_info	info;

	(void)argc;
	(void)argv;
	info.env = NULL;
	info.exit_code = 0;
	info.here_code = 0;
	info.root = NULL;
	info.prompt = NULL;
	env = ft_arrdup(env, &info);
	if (env == NULL)
		return (info.exit_code);
	info.env = env;
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
			ft_exit(ft_strdup("exit"), &info);//use NULL instead of strdup and change ft_exit accordingly
		}
		if (line && *line)
			ft_dothingswithline(line, &info);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:26:43 by lgaudin           #+#    #+#             */
/*   Updated: 2023/08/08 09:17:25 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int g_exit_code = 0;
//int g_other = 0;

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

char	**ft_arrdup(char **arr)
{
	int		i;
	int		j;
	char	**new;

	i = 0;
	while (arr[i])
		i++;
	new = malloc((i + 1) * sizeof(char *));
	if (!new)
		return (ft_putendl_fd("Memory allocation failed.", STDERR_FILENO), NULL);
	j = 0;
	while (j < i)
	{
		new[j] = ft_strdup(arr[j]);
		if (!new[j])
			return (ft_freearr(new), ft_putendl_fd("Memory allocation failed.", STDERR_FILENO), NULL);
		j++;
	}
	new[j] = NULL;
	return (new);
}

char *ft_prompt(void)
{
	return ("\033[0;32mminishell!> \033[0m");
}

void sigint_handler(int sig)
{
	//char	c;

	(void)sig;
	// if (g_other == -14)
	// {
	// 	//c = 4;
	// 	//write(1, &c, 1);
	// 	write(1, "\b\b\033[K\n", 5);
	// 	//rl_on_new_line();
	// 	//rl_redisplay();
	// 	//rl_replace_line("", 0);
	// }
	/* else if (g_exit_code == -15)
	{
		write(1, "\b\b\033[K", 5);
	} */
	//else
	//{
		rl_on_new_line();
		rl_redisplay();
		rl_replace_line("", 0);
		write(1, "\033[K\n", 5);
		rl_on_new_line();
		rl_redisplay();
		//ft_putchar_fd('\n', 1);
		//rl_replace_line("", 0);
	//}
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

int	ft_changeshlvl(char ***env)
{
	int		shlvl;
	int		shlvlpos;
	char	*shlvlval;

	shlvlval = ft_getenv("SHLVL", *env);
	if (!shlvlval)
		return (ft_putendl_fd("Memory allocation failed.", STDERR_FILENO), -1);
	shlvl = ft_atoi(shlvlval) + 1;
	shlvlpos = ft_existenv("SHLVL", *env);
	free(shlvlval);
	shlvlval = ft_itoa(shlvl);
	if (!shlvlval)
		return (ft_putendl_fd("Memory allocation failed.", STDERR_FILENO), -1);
	free ((*env)[shlvlpos]);
	(*env)[shlvlpos] = ft_strjoin("SHLVL=", shlvlval);
	free(shlvlval);
	if (!env[shlvlpos])
		return (ft_putendl_fd("Memory allocation failed.", STDERR_FILENO), -1);
	return (0);
}

void	ft_dothingswithline(char *line, char ***env)
{
	t_node	*root;
	int		exit_code;

	add_history(line);
	root = generate_node_from_command(line);
	exit_code = execute_node(root, env, root);
	destroy_node(root);
	free(line);
}

int main(int argc, char **argv, char **env)
{
	char	*line;

	(void)argc;
	(void)argv;
	env = ft_arrdup(env);
	if (env == NULL)
		return (g_exit_code);
	if (ft_changeshlvl(&env))
		return (g_exit_code);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline(ft_prompt());
		if (!line)
		{
			rl_clear_history();
			//ft_freearr(env);
			ft_exit(ft_strdup("exit"), &env);//use NULL instead of strdup and change ft_exit accordingly
			//exit(g_exit_code);
		}
		if (line && *line)
			ft_dothingswithline(line, &env);
	}
	return (g_exit_code);
}

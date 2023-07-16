/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:26:43 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/16 14:48:02 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		return (ft_putendl_fd("Memory allocation failed\n", STDERR_FILENO), NULL);
	j = 0;
	while (j < i)
	{
		new[j] = ft_strdup(arr[j]);
		//malloc protection
		j++;
	}
	new[j] = NULL;
	return (new);
}

char *ft_prompt(void)
{
	return ("\033[0;32mminishell> \033[0m");
}

void sigint_handler(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

// void check_leaks(void)
// {
// 	system("leaks minishell");
// }

int main(int argc, char **argv, char **env)
{
	char	*line;
	t_node	*root;
	int		exit_status;

	(void)argc;
	(void)argv;
	// atexit(check_leaks);
	env = ft_arrdup(env);
	signal(SIGINT, sigint_handler);
	while (1)
	{
		line = readline(ft_prompt());
		if (!line)
			exit(0);
		if (line && *line)
		{
			add_history(line);
			root = generate_node_from_command(line);
			exit_status = execute_node(root, &env);
		}
	}
	return (exit_status);
}

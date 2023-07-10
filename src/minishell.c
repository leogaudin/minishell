/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:26:43 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/10 16:23:56 by ysmeding         ###   ########.fr       */
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

int main(int argc, char **argv, char **env)
{
	char	*line;
	t_node	*root;
	int		exit_status;

	argc = 0;
	argv = NULL;
	env = ft_arrdup(env);
	while (1)
	{
		line = readline("minishell> ");
		if (line && *line)
		{
			add_history(line);
			root = generate_node_from_command(line);
			exit_status = execute_node(root, env);
		}
	}
}

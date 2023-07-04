/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:26:43 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/04 09:49:13 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// INFO: This function is not part of the project, it is only used to test the
// AST generation and execution.
int	main(int argc, char **argv)
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
}

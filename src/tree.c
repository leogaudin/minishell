/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 15:31:52 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/01 16:23:55 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_node	*create_node(char *operator, char * operand)
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	if (new_node == NULL)
	{
		ft_putendl_fd("Memory allocation failed\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	new_node->operator= operator;
	new_node->operand = operand;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

t_node	*create_operand_node(char *operand)
{
	return (create_node(NULL, operand));
}

t_node	*create_operator_node(char *operator)
{
	return (create_node(operator, NULL));
}

void	destroy_node(t_node *t_node)
{
	if (t_node == NULL)
		return ;
	if (t_node->left)
		destroy_node(t_node->left);
	if (t_node->right)
		destroy_node(t_node->right);
	free(t_node);
}

void	print_AST(t_node *root, int indent)
{
	if (root == NULL)
		return ;
	for (int i = 0; i < indent; i++)
		ft_printf("\t");
	if (root->operator!= NULL)
		ft_printf("%s\n", root->operator);
	if (root->operand != NULL)
		ft_printf("%s\n", root->operand);
	print_AST(root->left, indent + 1);
	print_AST(root->right, indent + 1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dev_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 16:29:16 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/01 16:34:15 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * @brief    Prints a visual representation of the abstract syntax tree.
 *
 * @param    root      The root node.
 * @param    indent    Indent for recursive calls (set to 0 for first call).
 */
void	print_ast(t_node *root, int indent)
{
	int	i;

	i = 0;
	if (root == NULL)
		return ;
	while (i++ < indent)
		ft_printf("\t");
	if (root->operator != NULL)
		ft_printf("%s\n", root->operator);
	if (root->operand != NULL)
		ft_printf("%s\n", root->operand);
	print_ast(root->left, indent + 1);
	print_ast(root->right, indent + 1);
}

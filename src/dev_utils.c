/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dev_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 16:29:16 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/01 16:31:30 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

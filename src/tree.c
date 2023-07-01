/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 15:31:52 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/01 16:31:46 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_node	*create_node(char *operator, char *operand)
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	if (new_node == NULL)
	{
		ft_putendl_fd("Memory allocation failed\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	new_node->operator = operator;
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

t_node	*generate_node_from_command(const char *command)
{
	t_node	*root;
	char	*token;
	char	**commands;

	root = NULL;
	token = NULL;
	commands = NULL;
	if (ft_strnstr(command, "||", ft_strlen(command)))
		token = "||";
	else if (ft_strnstr(command, "&&", ft_strlen(command)))
		token = "&&";
	if (token)
	{
		commands = ft_split_str_once(command, token);
		root = create_operator_node(token);
		root->left = generate_node_from_command(commands[0]);
		root->right = generate_node_from_command(commands[1]);
	}
	else
		root = create_operand_node((char *)command);
	// TODO: LEAKS - free commands[] without erasing them for after
	return (root);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 16:39:10 by lgaudin           #+#    #+#             */
/*   Updated: 2023/08/08 09:16:01 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief    Creates a node for the abstract syntax tree.
 *
 * @param    operator  The operator defining the relationship between the two
 *                   children nodes.
 * @param    operand   The operand of the node.
 * @return   t_node*   The newly created node.
 */
t_node	*create_node(char *operator, char * operand)
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	if (new_node == NULL)
		return (ft_malloc_error());
	new_node->operator = operator;
	new_node->operand = operand;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

/**
 * @brief    Creates an operand node object
 *
 * @param    operand   The operand of the node.
 * @return   t_node*   The newly created node.
 */
t_node	*create_operand_node(char *operand)
{
	t_node	*new_node;

	operand = strip_parenthesis(operand);
	if (exists_out_parenthesis(operand, "||") || exists_out_parenthesis(operand, "&&"))
	{
		new_node = generate_node_from_command(operand);
		free(operand);
		return (new_node);
	}
	else
		return (create_node(NULL, operand));
}

/**
 * @brief    Creates an operator node object
 *
 * @param    operand   The operator of the node.
 * @return   t_node*   The newly created node.
 */
t_node	*create_operator_node(char *operator)
{
	return (create_node(operator, NULL));
}

/**
 * @brief    Destroys a node and its recursive children.
 *
 * @param    t_node    The node to destroy.
 */
void	destroy_node(t_node *t_node)
{
	if (t_node == NULL)
		return ;
	if (t_node->operand)
		free(t_node->operand);
	if (t_node->operator)
		free(t_node->operator);
	if (t_node->left)
		destroy_node(t_node->left);
	if (t_node->right)
		destroy_node(t_node->right);
	free(t_node);
}

/**
 * @brief    Generates an abstract syntax tree from a command.
 *           The command is split into two parts, the left and right children.
 *           The left child is the command before the operator, and the right
 *           child is the command after the operator.
 *           If the command does not contain an operator, the node is an
 *           operand node.
 *           If the command contains an operator, the node is an operator node.
 *           The function is recursive, and will generate the tree from the
 *           bottom up.
 *
 * @param    command   The command to generate the tree from.
 * @return   t_node*   The root node of the tree.
 */
t_node	*generate_node_from_command(const char *command)
{
	t_node	*root;
	char	*token;
	char	**commands;

	root = NULL;
	token = NULL;
	commands = NULL;
	if (exists_out_parenthesis((char *)command, "||"))//this should actually just be the one it encounters first unless parenthesis.
		token = "||";
	else if (exists_out_parenthesis((char *)command, "&&"))
		token = "&&";
	if (token)
	{
		commands = ft_split_str_once(command, token);
		root = create_operator_node(ft_strdup(token));
		root->left = generate_node_from_command(commands[0]);
		root->right = generate_node_from_command(commands[1]);
		ft_free_tab(commands);
	}
	else
		root = create_operand_node((char *)command);
	return (root);
}

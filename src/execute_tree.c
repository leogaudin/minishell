/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 09:08:11 by lgaudin           #+#    #+#             */
/*   Updated: 2023/07/10 16:14:40 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <string.h>

/**
 * @brief    Executes a command using the execvp system call.
 *           This function is called in a child process.
 *
 * @param    command   The command to execute.
 * @return   int       The exit status of the command.
 * TODO: Remove this function and use Yolanthe's executer instead.
 */
int	execute_command(const char *command)
{
	int		i;
	char	*token;
	char	*args[64];

	i = 0;
	token = strtok((char *)command, " ");
	while (token != NULL)
	{
		args[i++] = token;
		token = strtok(NULL, " ");
	}
	args[i] = NULL;
	execvp(args[0], args);
	perror("execvp");
	exit(EXIT_FAILURE);
}

/**
 * @brief Executes an operator node in the abstract syntax tree (AST) based on
 * the exit status of the left child.
 *
 * If the operator is "&&" and the left command succeeded, the right child
 * node is executed.
 * If the operator is "||" and the left command failed, the right child node
 * is executed.
 *
 * @param    node        The operator node to execute.
 * @param    left_status The exit status of the left child node.
 * @return   int         The exit status of the executed node.
 */
int	execute_operator_node(t_node *node, int left_status, char **env)
{
	if (ft_strcmp(node->operator, "&&") == 0 && left_status == 0)
		return (execute_node(node->right, env));
	else if (ft_strcmp(node->operator, "||") == 0 && left_status != 0)
		return (execute_node(node->right, env));
	return (left_status);
}

/**
 * @brief Executes an operand node in the AST by forking a child process and
 * executing the command.
 *
 * In the child process, the command is executed using the execute_command
 * function.
 * In the parent process, the function waits for the child process to complete
 * and returns the exit status.
 *
 * @param    node      The operand node to execute.
 * @return   int       The exit status of the executed node.
 */
int	execute_operand_node(t_node *node, char **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		exit(ft_parseandexec(node->operand, env));
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (1);
	}
}

/**
 * @brief Executes a node in the abstract syntax tree (AST) by determining its
 * type and calling the corresponding execution function.
 *
 * If the node is an operator node, the execute_operator_node function is
 * called.
 * If the node is an operand node, the execute_operand_node function is
 * called.
 *
 * @param    node      The node to execute.
 * @return   int       The exit status of the executed node.
 */
int	execute_node(t_node *node, char **env)
{
	int	left_status;

	if (node == NULL)
		return (0);
	if (node->operator)
	{
		left_status = execute_node(node->left, env);
		return (execute_operator_node(node, left_status, env));
	}
	else
		return (execute_operand_node(node, env));
}

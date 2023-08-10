/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_export.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:03:28 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/08 11:34:25 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_exportnoarg(char ***env)
{
	int	i;

	i = 0;
	while ((*env)[i])
	{
		if (ft_findchar((*env)[i], '='))
			printf("declare -x %s\n", (*env)[i]);
		i++;
	}
}

int	ft_export_checkname(t_fullcmd fullcmd, int i, int *len)
{
	(*len) = 0;
	while (fullcmd.argums[i][(*len)] && fullcmd.argums[i][(*len)] != '=')
	{
		if ((ft_isalnum(fullcmd.argums[i][(*len)]) == 0 && \
		fullcmd.argums[i][(*len)] != '_') || ((*len) == 0 && \
		ft_isdigit(fullcmd.argums[i][(*len)]) == 1))
		{
			ft_puterror(": not a valid identifier", 1, "export: ", \
			fullcmd.argums[i]);
			return (1);
		}
		(*len)++;
	}
	return (0);
}

int	ft_export_replaceoraddvar(t_fullcmd fullcmd, char ***env, int i, int len)
{
	char	*varname;
	int		varpos;

	varname = ft_substr(fullcmd.argums[i], 0, len);
	varpos = ft_existenv(varname, *env);
	if (varname)
		free(varname);
	varname = ft_strdup(fullcmd.argums[i]);
	if (!varname)
		return (ft_putstrerror("malloc: "), g_exit_code);
	if (varpos >= 0)
	{
		free((*env)[varpos]);
		(*env)[varpos] = varname;
	}
	else
	{
		*env = ft_arrapp(*env, varname);//cmt1
		if (!*env)
			return (g_exit_code);
	}
	return (0);
}

int	ft_export(t_fullcmd fullcmd, char ***env)
{
	int		i;
	int		len;
	int		ok;

	if (!fullcmd.argums[1])
		ft_exportnoarg(env);
	i = 1;
	while (fullcmd.argums[i])
	{
		ok = ft_export_checkname(fullcmd, i, &len);
		if (ok == 0)
		{
			if (ft_export_replaceoraddvar(fullcmd, env, i, len))
				return (g_exit_code);
		}
		i++;
	}
	return (0);
}
//cmt1: what if null, because then no more env
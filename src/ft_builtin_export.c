/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_export.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:03:28 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/10 14:24:41 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_exportnoarg(t_gen_info *info)
{
	int	i;

	i = 0;
	while (info->env[i])
	{
		if (ft_findchar(info->env[i], '='))
			printf("declare -x %s\n", info->env[i]);
		i++;
	}
}

int	ft_export_checkname(t_fullcmd fullcmd, int i, int *len, t_gen_info *info)
{
	(*len) = 0;
	while (fullcmd.argums[i][(*len)] && fullcmd.argums[i][(*len)] != '=')
	{
		if ((ft_isalnum(fullcmd.argums[i][(*len)]) == 0 && \
		fullcmd.argums[i][(*len)] != '_') || ((*len) == 0 && \
		ft_isdigit(fullcmd.argums[i][(*len)]) == 1))
		{
			info->exit_code = ft_put_error(": not a valid identifier", 1, \
			"export: ", fullcmd.argums[i]);
			return (1);
		}
		(*len)++;
	}
	return (0);
}

int	ft_export_replaceaddvar(t_fullcmd fullcmd, t_gen_info *info, int i, int len)
{
	char	*varname;
	int		varpos;

	varname = ft_substr(fullcmd.argums[i], 0, len);
	if (!varname)
		return (ft_putstrerror("malloc: ", info), info->exit_code);
	varpos = ft_existenv(varname, info);
	if (varname)
		free(varname);
	varname = ft_strdup(fullcmd.argums[i]);
	if (!varname)
		return (ft_putstrerror("malloc: ", info), info->exit_code);
	if (varpos >= 0)
	{
		free(info->env[varpos]);
		info->env[varpos] = varname;
	}
	else
	{
		info->env = ft_arrapp(info->env, varname, info);//cmt1
		if (!info->env)
			return (info->exit_code);
	}
	return (0);
}

int	ft_export(t_fullcmd fullcmd, t_gen_info *info)
{
	int		i;
	int		len;
	int		ok;

	if (!fullcmd.argums[1])
		ft_exportnoarg(info);
	i = 1;
	info->exit_code = 0;
	while (fullcmd.argums[i])
	{
		ok = ft_export_checkname(fullcmd, i, &len, info);
		if (ok == 0)
		{
			if (ft_export_replaceaddvar(fullcmd, info, i, len))
				return (info->exit_code);
		}
		i++;
	}
	return (info->exit_code);
}
//cmt1: what if null, because then no more env
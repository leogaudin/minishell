/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:04:35 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/21 14:46:41 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_unset_checkname(t_fullcmd fullcmd, int i, int *len, t_gen_info *info)
{
	while (fullcmd.argums[i][++(*len)])
	{
		if (ft_isalnum(fullcmd.argums[i][(*len)]) == 0
			&& fullcmd.argums[i][(*len)] != '_')
		{
			info->exit_code = ft_put_error(": not a valid identifier", 1, \
			"unset: ", fullcmd.argums[i]);
			return (1);
		}
	}
	return (0);
}

int	ft_unset(t_fullcmd fullcmd, t_gen_info *info)
{
	int		i;
	int		len;
	int		varpos;
	char	*varname;

	i = 0;
	info->exit_code = 0;
	while (fullcmd.argums[++i])
	{
		len = -1;
		if (ft_unset_checkname(fullcmd, i, &len, info) == 0)
		{
			varname = ft_substr(fullcmd.argums[i], 0, len);
			if (!varname)
				return (ft_putstrerror("malloc: ", info), info->exit_code);
			varpos = ft_existenv(varname, info);
			free(varname);
			info->env = ft_arrremove(info->env, varpos, info);
			if (!info->env)
				return (info->exit_code);
		}
	}
	return (info->exit_code);
}

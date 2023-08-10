/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:04:35 by ysmeding          #+#    #+#             */
/*   Updated: 2023/08/08 11:21:58 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_unset_checkname(t_fullcmd fullcmd, int i, int *len)
{
	while (fullcmd.argums[i][++(*len)])
	{
		if (ft_isalnum(fullcmd.argums[i][(*len)]) == 0
			&& fullcmd.argums[i][(*len)] != '_')
		{
			ft_puterror(": not a valid identifier", 1, "export: ", \
			fullcmd.argums[i]);
			return (1);
		}
	}
	return (0);
}

int	ft_unset(t_fullcmd fullcmd, char ***env)
{
	int		i;
	int		ok;
	int		len;
	int		varpos;
	char	*varname;

	i = 0;
	while (fullcmd.argums[++i])
	{
		len = -1;
		ok = ft_unset_checkname(fullcmd, i, &len);
		if (ok == 0)
		{
			varname = ft_substr(fullcmd.argums[i], 0, len);
			if (!varname)
				return (ft_putstrerror("malloc: "), g_exit_code);
			varpos = ft_existenv(varname, *env);
			free(varname);
			*env = ft_arrremove(*env, varpos);//cmt1
			if (!*env)
				return (g_exit_code);
		}
	}
	return (0);
}
//cmt1: what if null, because then no more env
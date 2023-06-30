/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:26:37 by lgaudin           #+#    #+#             */
/*   Updated: 2023/06/28 14:34:38 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/src/libft.h"
# include <stdbool.h>

// This is just a starter, I don't understand lexers that much so it can totally change.
// However, I think it's a good and clean start.
typedef enum e_token_kind
{
	TOKEN_END,
	TOKEN_COMMAND,
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_SEMICOLON,
	TOKEN_GREAT,
	TOKEN_LESS,
	TOKEN_DGREAT,
	TOKEN_DLESS,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_NEWLINE,
	TOKEN_EOF,
	TOKEN_NONE
}					t_token_kind;

// typedef enum e_commands
// {
// 	CMD_ECHO,
// 	CMD_CD,
// 	CMD_PWD,
// 	CMD_EXPORT,
// 	CMD_UNSET,
// 	CMD_ENV,
// 	CMD_EXIT
// }					t_commands;

typedef struct s_token
{
	t_token_kind	kind;
	char			*value;
}					t_token;

typedef struct s_line
{
	t_token			**tokens;
}					t_line;

#endif

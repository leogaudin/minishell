/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 15:26:37 by lgaudin           #+#    #+#             */
/*   Updated: 2023/06/27 16:33:52 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"

// This is just a starter, I don't understand lexers that much so it can totally change.
// However, I think it's a good and clean start.
typedef enum e_token_kind
{
	TOKEN_END,
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

typedef struct s_token
{
	t_token_kind	kind;
	char			*value;
}					t_token;

#endif

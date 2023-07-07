/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysmeding <ysmeding@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 08:24:03 by ysmeding          #+#    #+#             */
/*   Updated: 2023/07/07 12:44:39 by ysmeding         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../pipex_files/libft/libft.h"

/* readline printf perror */
# include <stdio.h>
/* malloc free exit getenv */
# include <stdlib.h>
/* write access read close fork getcwd chdir unlink execve dup dup2 pipe isatty
 ttyname ttyslot*/
# include <unistd.h>
/* wait waitpid wait3 wait4 */
# include <sys/wait.h>
/* open */
# include <fcntl.h>
/* signal sigaction sigemptyset sigaddset kill */
# include <signal.h>
/* strerror */
# include <string.h>
/* errno */
# include <errno.h>
/* stat lstat fstat */
# include <sys/stat.h>
/* readline  rl_clear_history rl_on_new_line rl_replace_line rl_redisplay */
# include <readline/readline.h>
/* readline add_history */
# include <readline/history.h>
/* opendir readdir closedir */
# include <dirent.h>
/* opendir closedir */
# include <sys/types.h>
/* ioctl */
# include <sys/ioctl.h>
/* tcsetattr tcgetattr */
# include <termios.h>
/* tgetent tgetflag tgetnum tgetstr tgoto tputs */
# include <curses.h>
/* tgetent tgetflag tgetnum tgetstr tgoto tputs */
# include <term.h>

/**
 * Structure that holds the information about redirections.
 * 
 * @param reout 	0 if output not redirected, 1 if output redirected
 * @param reoutap 	0 if output not redirected in append mode, 1 if output
 *  redirected in append mode
 * @param outfile 	path to outfile
 */
typedef struct s_reout
{
	int		reout;
	int		reoutapp;
	char	*outfile;
}				t_reout;

/**
 * Structure that holds the information about redirections.
 * 
 * @param rein 		0 if input not redirected, 1 if input redirected
 * @param herein 	0 if input not from given lines, 1 if input from given
 *  line, i.e. if <<
 * @param heredel 	delimiter of << given right after
 * @param infile 	path to infile
 */
typedef struct s_rein
{
	int		rein;
	int		herein;
	char	*heredel;
	char	*infile;
}				t_rein;

/**
 * Structure that holds the command with it arguments and a redirection
 * structure.
 * 
 * @param redir		a redirection structure
 * @param cmdarg	a string containing the command as first word and then its
 *  arguments.
 */
typedef struct s_cmd
{
	t_rein	rein;
	t_reout	reout;
	char	*cmdarg;

}			t_cmd;

#endif
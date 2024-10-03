/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:38:44 by msavelie          #+#    #+#             */
/*   Updated: 2024/10/03 11:42:59 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <stdio.h>
# include <fcntl.h>
# include <sys/types.h>
# include <stdbool.h>
# include "libft_updated/libft.h"

typedef struct s_pipex
{
	int		fd_in[2];
	int		fd_out[2];
	bool	is_here_doc;
	char	**in_args;
	char	**out_args;
}	t_pipex;

void	parse_args(char **argv, t_pipex *pip);
char	**split_and_check(char *str, char del, t_pipex *pip);

#endif

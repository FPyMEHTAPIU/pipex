/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:38:44 by msavelie          #+#    #+#             */
/*   Updated: 2024/10/30 11:52:40 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <stdio.h>
# include <fcntl.h>
# include <sys/types.h>
# include <errno.h>
# include <stdbool.h>
# include <sys/wait.h>
# include "../libft_updated/libft.h"

typedef struct s_pipex
{
	int		pipfd[2];
	int		fd_in;
	int		fd_out;
	bool	is_here_doc;
	char	**in_args;
	char	**out_args;
	char	**paths;
}	t_pipex;

char	*parse_args(char **argv, t_pipex *pip);
char	**split_and_check(char *str, char del, t_pipex *pip);
void	clean_pip(t_pipex *pip);
char	**fetch_paths(char **envp);
int		count_strs(char **strs);
char	*check_paths_access(char **paths, char **args);

#endif

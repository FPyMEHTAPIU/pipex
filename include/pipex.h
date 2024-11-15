/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:38:44 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/15 10:07:33 by msavelie         ###   ########.fr       */
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
	char	**in_args;
	char	**out_args;
	char	**paths;
	char	*path;
	int		exit_code;
}	t_pipex;

void	parse_args(char **argv, t_pipex *pip);
char	**split_and_check(char *str, char del, t_pipex *pip);
void	clean_pip(t_pipex *pip);
char	**fetch_paths(char **envp);
int		count_strs(char **strs);
char	*check_paths_access(char **paths, char **args, char *arg, t_pipex *pip);
void	first_child(t_pipex *pip, char **argv, pid_t p);
void	last_child(t_pipex *pip, char **argv, pid_t p);
int		error_ret(int type, char *arg);
void	free_path(char *path);
char	**check_args(char **args);
bool	is_space_only(char *str);
void	print_exit(char *mes, char *cmd, int exit_code);
char	*check_paths(char **paths, char **args);
void	exit_child(t_pipex *pip, char *arg, int exit_code);

#endif

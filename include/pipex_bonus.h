/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:38:44 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/28 15:45:27 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H
# include <stdio.h>
# include <fcntl.h>
# include <sys/types.h>
# include <errno.h>
# include <stdbool.h>
# include <sys/wait.h>
# include "../libft_updated/libft.h"

typedef struct s_pipex
{
	int		**pipfd;
	int		fd_in;
	int		fd_out;
	char	**args;
	char	**paths;
	char	*path;
	int		exit_code;
	int		is_heredoc;
	int		mid_args;
	int		allocated_pipes;
	int		pipe_index;
}	t_pipex;

char	**split_and_check(char *str, char del, t_pipex *pip);
void	clean_pip(t_pipex *pip);
char	**fetch_paths(char **envp);
int		count_strs(char **strs);
char	*check_paths_access(char **paths, char **args, char *arg, t_pipex *pip);
int		error_ret(int type, char *arg);
void	error_check(int argc, char **argv);
void	free_path(char *path);
char	**check_args(char **args);
bool	is_space_only(char *str);
void	print_exit(char *mes, char *cmd, int exit_code);
char	*check_paths(char **paths, char **args);
void	exit_child(t_pipex *pip, char *arg, int exit_code);
void	clean_strs(char **strs);
int		count_mid_args(char **argv);
void	alloc_pipes(t_pipex *pip);
void	close_fds(t_pipex *pip);
int		check_permission(t_pipex *pip, char **argv, bool first);
void	call_children(t_pipex *pip, char **argv, pid_t *p);
int		wait_children(int *status, pid_t *p, t_pipex *pip);

#endif

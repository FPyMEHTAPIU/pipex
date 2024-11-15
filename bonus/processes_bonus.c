/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:49:58 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/15 17:05:59 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

static void	read_first(t_pipex *pip, char **argv, int arg)
{
	if (arg == 0)
	{
		pip->fd_in = open(argv[1], O_RDONLY);
		if (pip->fd_in == -1)
			exit_child(pip, argv[1], 1, arg);
		dup2(pip->fd_in, STDIN_FILENO);
		close(pip->fd_in);
	}
	else
	{
		dup2(pip->pipfd[arg][0], STDIN_FILENO);
		close(pip->pipfd[arg][0]);
	}
}

void	first_child(t_pipex *pip, char **argv, pid_t p, int arg)
{
	if (p < 0)
		error_ret(5, NULL);
	else if (p == 0)
	{
		read_first(pip, argv, arg);
		dup2(pip->pipfd[arg][1], STDOUT_FILENO);
		close(pip->pipfd[arg][1]);
		close(pip->pipfd[arg][0]);
		pip->args = split_and_check(argv[2 + arg], ' ', pip);
		pip->path = check_paths_access(pip->paths, pip->args, argv[2 + arg], pip);
		if (execve(pip->path, pip->args, pip->paths) == -1)
			exit_child(pip, argv[2], 127, arg);
	}
}

void	last_child(t_pipex *pip, char **argv, pid_t p, int arg)
{
	if (p < 0)
		error_ret(5, NULL);
	else if (p == 0)
	{
		pip->fd_out = open(argv[2 + pip->mid_args], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pip->fd_out == -1)
			exit_child(pip, argv[4], 1, arg);
		dup2(pip->pipfd[arg][0], STDIN_FILENO);
		close(pip->pipfd[arg][0]);
		dup2(pip->fd_out, STDOUT_FILENO);
		close(pip->fd_out);
		close(pip->pipfd[arg][1]);
		pip->args = split_and_check(argv[1 + pip->mid_args], ' ', pip);
		pip->path = check_paths_access(pip->paths, pip->args, argv[1 + pip->mid_args], pip);
		if (execve(pip->path, pip->args, pip->paths) == -1)
			exit_child(pip, argv[3], 127, arg);
	}
}

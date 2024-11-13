/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:49:58 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/13 16:32:29 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	first_child(t_pipex *pip, char **argv, pid_t p)
{
	if (p < 0)
		error_ret(5, NULL);
	else if (p == 0)
	{
		pip->thread++;
		pip->fd_in = open(argv[1], O_RDONLY);
		if (pip->fd_in == -1)
			exit_child(pip, argv[1], 1);
		dup2(pip->fd_in, STDIN_FILENO);
		dup2(pip->pipfd[1], STDOUT_FILENO);
		close(pip->pipfd[1]);
		close(pip->pipfd[0]);
		close(pip->fd_in);
		///ft_printf("child 1.1 error: %d\n", pip->exit_code);
		pip->path = check_paths_access(pip->paths, pip->in_args, argv[2], pip);
		//ft_printf("child 1.2 error: %d\n", pip->exit_code);
		//ft_printf("child 1 path: %s\n", pip->path);
		if (execve(pip->path, pip->in_args, pip->paths) == -1)
			exit_child(pip, argv[2], 127);
	}
}

void	last_child(t_pipex *pip, char **argv, pid_t p)
{
	if (p < 0)
		error_ret(5, NULL);
	else if (p == 0)
	{
		pip->fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pip->fd_out == -1)
			exit_child(pip, argv[4], 1);
		dup2(pip->pipfd[0], STDIN_FILENO);
		dup2(pip->fd_out, STDOUT_FILENO);
		close(pip->pipfd[0]);
		close(pip->pipfd[1]);
		close(pip->fd_out);
		//ft_printf("child 2.1 error: %d\n", pip->exit_code);
		pip->path = check_paths_access(pip->paths, pip->out_args, argv[3], pip);
		//ft_printf("child 2.2 error: %d\n", pip->exit_code);
		//ft_printf("child 2 path: %s\n", pip->path);
		if (execve(pip->path, pip->out_args, pip->paths) == -1)
			exit_child(pip, argv[3], 127);
	}
}

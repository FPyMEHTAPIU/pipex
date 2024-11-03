/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:49:58 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/01 12:49:33 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	first_child(t_pipex *pip, char **argv, pid_t p)
{
	if (p < 0)
		error_ret(5, NULL);
	else if (p == 0)
	{
		pip->fd_in = open(argv[1], O_RDONLY);
		dup2(pip->fd_in, STDIN_FILENO);
		dup2(pip->pipfd[1], STDOUT_FILENO);
		close(pip->pipfd[1]);
		close(pip->pipfd[0]);
		close(pip->fd_in);
		//(pip->thread)++;
		if (execve(pip->path, pip->in_args, pip->paths) == -1)
		{
			clean_pip(pip);
			perror(argv[0]);
			exit(1);
		}
		ft_printf("child 1\n");
		//free_path(path);
		exit(0);
	}
}

void	last_child(t_pipex *pip, char **argv, pid_t p)
{
	if (p < 0)
		error_ret(5, NULL);
	else if (p == 0 /*&& pip->thread > 0*/)
	{
		pip->fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(pip->pipfd[0], STDIN_FILENO);
		dup2(pip->fd_out, STDOUT_FILENO);
		close(pip->pipfd[0]);
		close(pip->pipfd[1]);
		close(pip->fd_out);
		//free_path(pip->path);
		pip->path = check_paths_access(pip->paths, pip->out_args, pip);
		if (execve(pip->path, pip->out_args, pip->paths) == -1)
		{
			ft_printf("child 2\n");
			clean_pip(pip);
			perror(argv[0]);
			exit(1);
		}
		ft_printf("child 2\n");
		//free_path(path);
		exit(0);
	}
}

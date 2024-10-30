/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:49:58 by msavelie          #+#    #+#             */
/*   Updated: 2024/10/30 15:41:27 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	first_child(t_pipex *pip, char **argv, char *path, pid_t p)
{
	if (p < 0)
		error_ret(5);
	else if (p == 0)
	{
		pip->fd_in = open(argv[1], O_RDONLY);
		dup2(pip->fd_in, STDIN_FILENO);
		dup2(pip->pipfd[1], STDOUT_FILENO);
		close(pip->pipfd[1]);
		close(pip->pipfd[0]);
		close(pip->fd_in);
		//(pip->thread)++;
		if (execve(path, pip->in_args, pip->paths) == -1)
			free(path);
		free(path);
		perror("execve");
		exit(1);
	}
}

void	last_child(t_pipex *pip, char **argv, char *path, pid_t p)
{
	(void) argv;
	if (p < 0)
		error_ret(5);
	else if (p == 0 /*&& pip->thread > 0*/)
	{
		ft_printf("%s\n", argv[4]);
		dup2(pip->pipfd[0], STDIN_FILENO);
		dup2(pip->fd_out, STDOUT_FILENO);
		close(pip->pipfd[0]);
		close(pip->pipfd[1]);
		close(pip->fd_out);
		path = check_paths_access(pip->paths, pip->out_args, pip);
		if (execve(path, pip->out_args, pip->paths) == -1)
			free(path);
		perror("execve");
		free(path);
		exit(1);
	}
}

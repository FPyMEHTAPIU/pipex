/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:49:58 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/28 11:20:38 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

static void	read_first(t_pipex *pip, char **argv, int arg)
{
	if (arg == 0)
	{
		if (pip->is_heredoc == 0)
			check_permission(pip, argv, arg, true);
		else
		{
			pip->fd_in = open(".heredoc_temp", O_RDONLY);
			if (pip->fd_in == -1)
				exit_child(pip, ".heredoc_temp", 1, arg);
		}
		dup2(pip->fd_in, STDIN_FILENO);
		close(pip->fd_in);
	}
	else
	{
		dup2(pip->pipfd[pip->pipe_index - 1][0], STDIN_FILENO);
		close(pip->pipfd[pip->pipe_index - 1][0]);
	}
}

static void	first_child(t_pipex *pip, char **argv, pid_t *p, int arg)
{
	if (p[arg] < 0)
	{
		clean_pip(pip);
		free(p);
		error_ret(5, NULL);
	}
	else if (p[arg] == 0)
	{
		read_first(pip, argv, arg);
		dup2(pip->pipfd[pip->pipe_index][1], STDOUT_FILENO);
		close(pip->pipfd[pip->pipe_index][1]);
		close_fds(pip);
		pip->args = split_and_check(argv[2 + arg + pip->is_heredoc], ' ', pip);
		pip->path = check_paths_access(pip->paths, pip->args, argv[2 + arg + pip->is_heredoc], pip);
		if (execve(pip->path, pip->args, pip->paths) == -1)
			exit_child(pip, argv[2 + arg + pip->is_heredoc], 127, arg);
	}
}

static void	last_child(t_pipex *pip, char **argv, pid_t *p, int arg)
{
	if (p[arg + 1] < 0)
	{
		clean_pip(pip);
		free(p);
		error_ret(5, NULL);
	}
	else if (p[arg + 1] == 0)
	{
		check_permission(pip, argv, arg, false);
		dup2(pip->pipfd[pip->pipe_index][0], STDIN_FILENO);	
		close(pip->pipfd[pip->pipe_index][0]);
		dup2(pip->fd_out, STDOUT_FILENO);
		close(pip->fd_out);
		close_fds(pip);
		pip->args = split_and_check(argv[1 + pip->mid_args], ' ', pip);
		pip->path = check_paths_access(pip->paths, pip->args, argv[1 + pip->mid_args], pip);
		if (execve(pip->path, pip->args, pip->paths) == -1)
			exit_child(pip, argv[1 + pip->mid_args], 127, arg);
	}
}

void	pipex(t_pipex *pip, char **argv)
{
	int		i;
	pid_t	*p;
	int		status;

	p = malloc (sizeof(pid_t) * pip->mid_args);
	if (!p)
	{
		clean_pip(pip);
		error_ret(6, NULL);
	}
	i = 0;
	while (i < pip->mid_args)
	{
		p[i] = fork();
		if (i == pip->mid_args - 1)
		{
			pip->pipe_index--;
			last_child(pip, argv, p, i - 1);
		}
		else 
		{
			first_child(pip, argv, p, i);
			pip->pipe_index++;
		}
		free_path(pip->path);
		clean_strs(pip->args);
		pip->path = NULL;
		i++;
	}
	close_fds(pip);
	i = 0;
	while (i < pip->mid_args)
	{
		if (wait(&status) == p[pip->mid_args - 1] && WIFEXITED(status))
			pip->exit_code = WEXITSTATUS(status);
		i++;
	}
	free(p);
}

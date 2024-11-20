/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:49:58 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/20 15:09:45 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

static void	handle_here_doc(t_pipex *pip, char **argv)
{
	char	*str;

	pip->fd_in = open(".heredoc_temp", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (!pip->fd_in)
	{
		clean_pip(pip);
		error_ret(7, NULL);
	}
	ft_putstr_fd("pipe heredoc> ", STDOUT_FILENO);
	str = get_next_line(STDIN_FILENO);
	while (str && !ft_strnstr(str, argv[1], ft_strlen(argv[1])))
	{
		ft_putstr_fd(str, pip->fd_in);
		free(str);
		ft_putstr_fd("pipe heredoc> ", STDOUT_FILENO);
		str = get_next_line(STDIN_FILENO);
	}
	if (str)
		free(str);
	dup2(pip->fd_in, STDIN_FILENO);
	close(pip->fd_in);
}

static void	read_first(t_pipex *pip, char **argv, int arg)
{
	if (arg == 0)
	{
		if (pip->is_heredoc == 1)
			handle_here_doc(pip, argv);
		else
		{
			pip->fd_in = open(argv[1], O_RDONLY);
			if (pip->fd_in == -1)
				exit_child(pip, argv[1], 1, arg);
			dup2(pip->fd_in, STDIN_FILENO);
			close(pip->fd_in);
		}	
	}
	else
	{
		dup2(pip->pipfd[pip->pipe_index - 1][0], STDIN_FILENO);
		close(pip->pipfd[pip->pipe_index - 1][0]);
	}
}

void	first_child(t_pipex *pip, char **argv, pid_t p, int arg)
{
	if (p < 0)
		error_ret(5, NULL);
	else if (p == 0)
	{
		read_first(pip, argv, arg);
		dup2(pip->pipfd[pip->pipe_index][1], STDOUT_FILENO);
		close(pip->pipfd[pip->pipe_index][1]);
		close_fds(pip);
		pip->args = split_and_check(argv[2 + arg], ' ', pip);
		pip->path = check_paths_access(pip->paths, pip->args, argv[2 + arg], pip);
		if (execve(pip->path, pip->args, pip->paths) == -1)
			exit_child(pip, argv[2 + arg], 127, arg);
	}
}

void	last_child(t_pipex *pip, char **argv, pid_t p, int arg)
{
	if (p < 0)
		error_ret(5, NULL);
	else if (p == 0)
	{
		if (pip->is_heredoc == 1)
			pip->fd_out = open(argv[2 + pip->mid_args], O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			pip->fd_out = open(argv[2 + pip->mid_args], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pip->fd_out == -1)
			exit_child(pip, argv[2 + pip->mid_args], 1, arg);
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
	pid_t	p;

	i = 0;
	while (i < pip->mid_args)
	{
		p = fork();
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
}

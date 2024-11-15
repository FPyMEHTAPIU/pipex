/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:23:55 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/15 17:09:14 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

static t_pipex	init_pip(char **envp, char **argv)
{
	t_pipex	pip;

	pip.fd_in = 0;
	pip.fd_out = 0;
	pip.args = NULL;
	pip.path = NULL;
	pip.paths = fetch_paths(envp);
	pip.exit_code = 0;
	pip.mid_args = count_mid_args(argv + 2);
	pip.allocated_pipes = 0;
	pip.pipfd = (int **)malloc(sizeof(int) * pip.mid_args + 2);
	if (!pip.pipfd)
	{
		clean_pip(&pip);
		error_ret(6, NULL);
	}
	return (pip);
}

static void	alloc_pipe(t_pipex *pip, int arg)
{
	pip->pipfd[arg] = (int *)malloc(sizeof(int) * 2);
	if (!pip->pipfd[arg])
	{
		clean_pip(pip);
		error_ret(6, NULL);
	}
	if (pipe(pip->pipfd[arg]) == -1)
	{
		clean_pip(pip);
		error_ret(4, NULL);
	}
	pip->allocated_pipes++;
}

int	main(int argc, char *argv[], char **envp)
{
	t_pipex	pip;
	pid_t	p;
	int		status;
	int		i;

	error_check(argc);
	pip = init_pip(envp, argv);
	i = 0;
	while (i < pip.mid_args - 1)
	{
		alloc_pipe(&pip, i);
		p = fork();
		first_child(&pip, argv, p, i);
		free_path(pip.path);
		clean_strs(pip.args);
		pip.path = NULL;
		i++;
	}
	p = fork();
	last_child(&pip, argv, p, --i);
	close(pip.pipfd[i][0]);
	close(pip.pipfd[i][1]);
	while (wait(&status) > 0)
		if (WIFEXITED(status))
			pip.exit_code = WEXITSTATUS(status);
	clean_pip(&pip);
	return (pip.exit_code);
}

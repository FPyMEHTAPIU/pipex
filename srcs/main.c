/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:23:55 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/28 15:35:25 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static t_pipex	init_pip(char **envp)
{
	t_pipex	pip;

	pip.pipfd[0] = -1;
	pip.pipfd[1] = -1;
	pip.fd_in = -1;
	pip.fd_out = -1;
	pip.in_args = NULL;
	pip.out_args = NULL;
	pip.path = NULL;
	pip.paths = fetch_paths(envp);
	pip.exit_code = 0;
	return (pip);
}

static int	wait_children(int *status, pid_t *p, t_pipex *pip)
{
	int	i;
	int	signal;

	i = 0;
	while (i < 2)
	{
		if (wait(status) == p[1] && WIFEXITED(*status))
		{
			if (WIFSIGNALED(*status))
			{
				signal = WTERMSIG(*status);
				return (128 + signal);
			}
			pip->exit_code = WEXITSTATUS(*status);
		}
		i++;
	}
	return (pip->exit_code);
}

int	main(int argc, char *argv[], char **envp)
{
	t_pipex	pip;
	pid_t	p[2];
	int		status;

	error_check(argc);
	pip = init_pip(envp);
	parse_args(argv, &pip);
	if (pipe(pip.pipfd) == -1)
	{
		clean_pip(&pip);
		return (error_ret(4, NULL));
	}	
	p[0] = fork();
	first_child(&pip, argv, p[0]);
	free_path(pip.path);
	pip.path = NULL;
	p[1] = fork();
	last_child(&pip, argv, p[1]);
	close_fds(&pip);
	pip.exit_code = wait_children(&status, p, &pip);
	clean_pip(&pip);
	return (pip.exit_code);
}

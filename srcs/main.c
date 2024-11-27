/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:23:55 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/27 12:59:59 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static t_pipex	init_pip(char **envp)
{
	t_pipex	pip;

	pip.pipfd[0] = 0;
	pip.pipfd[1] = 0;
	pip.fd_in = 0;
	pip.fd_out = 0;
	pip.in_args = NULL;
	pip.out_args = NULL;
	pip.path = NULL;
	pip.paths = fetch_paths(envp);
	pip.exit_code = 0;
	return (pip);
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
	close(pip.pipfd[0]);
	close(pip.pipfd[1]);
	waitpid(p[0], &status, 0);
	if (waitpid(p[1], &status, 0) != -1 && WIFEXITED(status))
		pip.exit_code = WEXITSTATUS(status);
	clean_pip(&pip);
	return (pip.exit_code);
}

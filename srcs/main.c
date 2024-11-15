/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:23:55 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/15 16:10:35 by msavelie         ###   ########.fr       */
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
	pid_t	p;
	int		status;

	error_check(argc);
	pip = init_pip(envp);
	parse_args(argv, &pip);
	if (pipe(pip.pipfd) == -1)
	{
		clean_pip(&pip);
		return (error_ret(4, NULL));
	}	
	p = fork();
	first_child(&pip, argv, p);
	free_path(pip.path);
	pip.path = NULL;
	p = fork();
	last_child(&pip, argv, p);
	close(pip.pipfd[0]);
	close(pip.pipfd[1]);
	while (wait(&status) > 0)
		if (WIFEXITED(status))
			pip.exit_code = WEXITSTATUS(status);
	clean_pip(&pip);
	return (pip.exit_code);
}

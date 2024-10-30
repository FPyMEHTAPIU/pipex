/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:23:55 by msavelie          #+#    #+#             */
/*   Updated: 2024/10/30 11:53:38 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static int	error_ret(int type)
{
	if (type == 1)
		perror("Incorrect number of arguments! \
Provide 4 arguments!\n");
	else if (type == 2)
		perror("Don't have an access to an infile!\n");
	else if (type == 3)
		perror("Don't have an access to an outfile!\n");
	else if (type == 4)
		perror("Pipe failed\n");
	else if (type == 5)
		perror("Fork failed\n");
	exit(1);
}

static void	error_check(int argc, char **argv)
{
	if (argc != 5)
		error_ret(1);	
	if (access(argv[1], R_OK) != 0)
		error_ret(2);
	if (access(argv[4], W_OK) != 0 && \
		access(argv[4], W_OK) != ENOENT)
		error_ret(3);
}

static t_pipex	init_pip(char **envp)
{
	t_pipex	pip;
	
	pip.pipfd[0] = 0;
	pip.pipfd[1] = 0;
	pip.is_here_doc = false;
	pip.fd_in = 0;
	pip.fd_out = 0;
	pip.in_args = NULL;
	pip.out_args = NULL;
	pip.paths = fetch_paths(envp);

	return (pip);
}

int	main(int argc, char *argv[], char **envp)
{
	t_pipex pip;
	char *path;
	pid_t p;

	pip = init_pip(envp);
	error_check(argc, argv);
	path = parse_args(argv, &pip);
	ft_printf("exec path: %s\n", path);

	if (pipe(pip.pipfd) == -1)
		return (error_ret(4));

	p = fork();
	if (p < 0)
		return (error_ret(5));
	else if (p == 0)
	{
		dup2(pip.fd_in, STDIN_FILENO);
		dup2(pip.pipfd[1], STDOUT_FILENO);

		close(pip.pipfd[0]);
		close(pip.fd_in);
		execve(path, pip.in_args, pip.paths);
		free(path);
		perror("execve");
		exit(1);
	}

	p = fork();
	if (p < 0)
		return (error_ret(5));
	else if (p == 0)
	{
		dup2(pip.pipfd[0], STDIN_FILENO);
		dup2(pip.fd_out, STDOUT_FILENO);
		close(pip.pipfd[1]);
		close(pip.fd_out);

		path = check_paths_access(pip.paths, pip.out_args);
		execve(path, pip.out_args, pip.paths);
		perror("execve");
		free(path);
		exit(1);
	}

	close(pip.pipfd[0]);
	close(pip.pipfd[1]);

	wait(NULL);
	wait(NULL);

	free(path);
	clean_pip(&pip);
	return (0);
}

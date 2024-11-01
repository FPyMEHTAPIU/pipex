/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:23:55 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/01 11:26:40 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	error_ret(int type)
{
	if (type == 1)
		ft_putstr_fd("Incorrect number of arguments! \
Provide 4 arguments!\n", 2);
	else if (type == 2)
		ft_putstr_fd("Don't have an access to an infile!\n", 2);
	else if (type == 3)
		ft_putstr_fd("Don't have an access to an outfile!\n", 2);
	else if (type == 4)
		ft_putstr_fd("Pipe failed\n", 2);
	else if (type == 5)
		ft_putstr_fd("Fork failed\n", 2);
	else if (type == 6)
		ft_putstr_fd("Malloc failed\n", 2);
	exit(1);
}

static void	error_check(int argc, char **argv)
{
	if (argc != 5)
		error_ret(1);
	if (access(argv[1], R_OK) != 0)
		error_ret(2);
	if (access(argv[4], F_OK) == 0 && access(argv[4], W_OK) != 0)
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
	//pip.thread = 0;
	return (pip);
}

int	main(int argc, char *argv[], char **envp)
{
	t_pipex	pip;
	pid_t	p;

	error_check(argc, argv);
	pip = init_pip(envp);
	pip.path = parse_args(argv, &pip);
	if (pipe(pip.pipfd) == -1)
		return (error_ret(4));
	p = fork();
	first_child(&pip, argv, pip.path, p);
	//free_path(path);
	p = fork();
	last_child(&pip, argv, pip.path, p);
	close(pip.pipfd[0]);
	close(pip.pipfd[1]);
	wait(NULL);
	wait(NULL);
	clean_pip(&pip);
	return (0);
}

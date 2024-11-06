/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:23:55 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/06 15:41:48 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	error_ret(int type, char *arg)
{
	if (type == 1)
		ft_putstr_fd("Incorrect number of arguments! \
Provide 4 arguments!\n", 2);
	else if (type == 2)
		perror(arg);
	else if (type == 3)
		perror(arg);
	else if (type == 4)
		ft_putstr_fd("Pipe failed\n", 2);
	else if (type == 5)
		ft_putstr_fd("Fork failed\n", 2);
	else if (type == 6)
		ft_putstr_fd("Malloc failed\n", 2);
	exit(0);
}

static void	error_check(int argc, char **argv)
{
	if (argc != 5)
		error_ret(1, NULL);
	if (access(argv[1], R_OK) != 0)
		error_ret(2, argv[1]);
	if (access(argv[4], F_OK) == 0 && access(argv[4], W_OK) != 0)
		error_ret(3, argv[4]);
}

static t_pipex	init_pip(char **envp, int *exit_code)
{
	t_pipex	pip;

	pip.pipfd[0] = 0;
	pip.pipfd[1] = 0;
	pip.is_here_doc = false;
	pip.fd_in = 0;
	pip.fd_out = 0;
	pip.in_args = NULL;
	pip.out_args = NULL;
	pip.path = NULL;
	pip.paths = fetch_paths(envp);
	pip.thread = 0;
	pip.exit_code = exit_code;
	return (pip);
}

int	main(int argc, char *argv[], char **envp)
{
	t_pipex	pip;
	pid_t	p;
	int		exit_code;
	int		status;

	error_check(argc, argv);
	pip = init_pip(envp, &exit_code);
	parse_args(argv, &pip);
	if (pipe(pip.pipfd) == -1)
		return (error_ret(4, NULL));
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
			exit_code = WEXITSTATUS(status);
	clean_pip(&pip);
	return (*pip.exit_code);
}

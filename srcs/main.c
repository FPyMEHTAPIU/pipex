/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:23:55 by msavelie          #+#    #+#             */
/*   Updated: 2024/10/03 11:36:39 by msavelie         ###   ########.fr       */
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
	return (1);
}

static t_pipex	init_pip(void)
{
	t_pipex	pip;

	pip.fd_in[0] = 0;
	pip.fd_in[1] = 0;
	pip.fd_out[0] = 0;
	pip.fd_out[1] = 0;
	pip.is_here_doc = false;
	return (pip);
}

int	main(int argc, char *argv[])
{
	if (argc != 5)
		return (error_ret(1));
	ft_printf("%s\n", argv[0]);
	t_pipex	pip;
	
	pip = init_pip();
	if (access(argv[1], R_OK) != 0)
		return (error_ret(2));
	if (pipe(pip.fd_in) < 0 || pipe(pip.fd_out) < 0)
		return (error_ret(4));
	pid_t p = fork();
	if (p < 0)
		return (error_ret(5));
	else if (p > 0)
	{
		// close read and do writing
		close(pip.fd_in[0]);
	}
	else
	{
		pip.fd_out[0] = open(argv[2], O_RDWR);
		
	}
	return (0);
}

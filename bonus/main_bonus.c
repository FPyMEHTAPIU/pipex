/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:23:55 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/19 13:20:11 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

static void	alloc_pipes(t_pipex *pip)
{
	int	i;

	pip->pipfd = (int **)malloc(sizeof(int) * pip->mid_args - 1);
	if (!pip->pipfd)
	{
		clean_pip(pip);
		error_ret(6, NULL);
	}
	i = 0;
	while (i < pip->mid_args - 1)
	{
		pip->pipfd[i] = (int *)malloc(sizeof(int) * 2);
		if (!pip->pipfd[i])
		{
			clean_pip(pip);
			error_ret(6, NULL);
		}
		if (pipe(pip->pipfd[i]) == -1)
		{
			clean_pip(pip);
			error_ret(4, NULL);
		}
		pip->allocated_pipes++;
		i++;
	}
}

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
	pip.pipe_index = 0;
	alloc_pipes(&pip);
	return (pip);
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
	while (i < pip.mid_args)
	{
		p = fork();
		if (i == pip.mid_args - 1)
		{
			pip.pipe_index--;
			last_child(&pip, argv, p, i - 1);
		}
		else 
		{
			first_child(&pip, argv, p, i);
			pip.pipe_index++;
		}
		free_path(pip.path);
		clean_strs(pip.args);
		pip.path = NULL;
		i++;
	}
	ft_printf("allocated pipes: %d\n", pip.allocated_pipes);
	i = 0;
	while (i < pip.allocated_pipes)
	{
		close(pip.pipfd[i][0]);
		close(pip.pipfd[i][1]);
		i++;
	}
	while (wait(&status) != -1) 
	{
		if (WIFEXITED(status))
			pip.exit_code = WEXITSTATUS(status);
	}
		
	clean_pip(&pip);
	return (pip.exit_code);
}

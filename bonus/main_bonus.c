/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:23:55 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/20 13:27:40 by msavelie         ###   ########.fr       */
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
	if (ft_strnstr(argv[1], "here_doc", 8))
		pip.is_heredoc = 1;
	else
		pip.is_heredoc = 0;
	pip.mid_args = count_mid_args(argv + 2 + pip.is_heredoc);
	pip.allocated_pipes = 0;
	pip.pipe_index = 0;
	alloc_pipes(&pip);
	return (pip);
}

int	main(int argc, char *argv[], char **envp)
{
	t_pipex	pip;
	int		status;

	error_check(argc);
	pip = init_pip(envp, argv);
	pipex(&pip, argv + pip.is_heredoc);
	while (wait(&status) != -1) 
	{
		if (WIFEXITED(status))
			pip.exit_code = WEXITSTATUS(status);
	}
	clean_pip(&pip);
	return (pip.exit_code);
}

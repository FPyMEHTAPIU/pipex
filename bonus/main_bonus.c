/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:23:55 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/29 12:36:32 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

static void	handle_here_doc(t_pipex *pip, char **argv)
{
	char	*str;

	if (pip->is_heredoc == 0)
		return ;
	pip->fd_in = open(".heredoc_temp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pip->fd_in < 0)
	{
		clean_pip(pip);
		error_ret(7, NULL);
	}
	str = get_next_line(STDIN_FILENO);
	while (str && ft_strncmp(str, argv[1], ft_strlen(str) - 1) != 0)
	{
		ft_putstr_fd(str, pip->fd_in);
		free(str);
		str = get_next_line(STDIN_FILENO);
	}
	if (str)
		free(str);
	close(pip->fd_in);
}

static t_pipex	init_pip(char **envp, char **argv)
{
	t_pipex	pip;

	pip.fd_in = -1;
	pip.fd_out = -1;
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

static void	pipex(t_pipex *pip, char **argv)
{
	pid_t	*p;
	int		status;

	p = malloc(sizeof(pid_t) * pip->mid_args);
	if (!p)
	{
		clean_pip(pip);
		error_ret(6, NULL);
	}
	call_children(pip, argv, p);
	close_fds(pip);
	pip->exit_code = wait_children(&status, p, pip);
	free(p);
}

int	main(int argc, char *argv[], char **envp)
{
	t_pipex	pip;

	error_check(argc, argv);
	pip = init_pip(envp, argv);
	handle_here_doc(&pip, argv + pip.is_heredoc);
	pipex(&pip, argv + pip.is_heredoc);
	if (pip.is_heredoc == 1)
	{
		close(pip.fd_in);
		unlink(".heredoc_temp");
	}
	clean_pip(&pip);
	return (pip.exit_code);
}

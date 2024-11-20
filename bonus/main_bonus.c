/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:23:55 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/20 16:05:29 by msavelie         ###   ########.fr       */
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
	ft_putstr_fd("pipe heredoc> ", STDOUT_FILENO);
	str = get_next_line(STDIN_FILENO);
	while (str && !ft_strnstr(str, argv[1], ft_strlen(argv[1])))
	{
		ft_putstr_fd(str, pip->fd_in);
		free(str);
		ft_putstr_fd("pipe heredoc> ", STDOUT_FILENO);
		str = get_next_line(STDIN_FILENO);
	}
	if (str)
		free(str);
	close(pip->fd_in);
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
	handle_here_doc(&pip, argv + pip.is_heredoc);
	pipex(&pip, argv + pip.is_heredoc);
	while (wait(&status) != -1) 
	{
		if (WIFEXITED(status))
			pip.exit_code = WEXITSTATUS(status);
	}
	if (pip.is_heredoc == 1)
	{
		close(pip.fd_in);
		unlink(".heredoc_temp");
	}
	clean_pip(&pip);
	return (pip.exit_code);
}

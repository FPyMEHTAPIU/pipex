/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:24:43 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/27 17:05:27 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	alloc_pipes(t_pipex *pip)
{
	int	i;

	pip->pipfd = (int **)malloc(sizeof(int *) * pip->mid_args - 1);
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

void	close_fds(t_pipex *pip)
{
	int	i;

	i = 0;
	while (i < pip->allocated_pipes)
	{
		if (pip->pipfd[i][0] != -1)
			close(pip->pipfd[i][0]);
		if (pip->pipfd[i][1] != -1)
			close(pip->pipfd[i][1]);
		i++;
	}
	if (pip->fd_in != -1)
		close(pip->fd_in);
	if (pip->fd_out != -1)
		close(pip->fd_out);
}

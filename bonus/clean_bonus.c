/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 10:45:18 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/15 16:51:10 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	clean_strs(char **strs)
{
	int	i;
	
	if (!strs || !*strs)
		return ;
	i = 0;
	while (strs[i])
		free(strs[i++]);
	free(strs);
	strs = NULL;
}

void	free_path(char *path)
{
	if (!path || !*path)
		return ;
	free(path);
	path = NULL;
}

static void clean_pipes(int **pipfd, t_pipex *pip)
{
	int	i;

	if (!pipfd)
		return ;
	if (pipfd && !*pipfd)
	{
		free(pipfd);
		return ;
	}
	i = 0;
	while (i < pip->allocated_pipes)
	{
		free(pipfd[i]);
		i++;
	}
	pip->allocated_pipes = 0;
	free(pipfd);
}

void	clean_pip(t_pipex *pip)
{
	clean_pipes(pip->pipfd, pip);
	clean_strs(pip->args);
	clean_strs(pip->paths);
	free_path(pip->path);
}

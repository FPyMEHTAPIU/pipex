/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 10:45:18 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/19 13:23:08 by msavelie         ###   ########.fr       */
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

static void clean_pipes(t_pipex *pip)
{
	int	i;

	if (!pip->pipfd)
		return ;
	if (pip->pipfd && !*pip->pipfd)
	{
		free(pip->pipfd);
		return ;
	}
	i = 0;
	while (i < pip->allocated_pipes)
	{
		free(pip->pipfd[i]);
		i++;
	}
	pip->allocated_pipes = 0;
	free(pip->pipfd);
}

void	clean_pip(t_pipex *pip)
{
	clean_pipes(pip);
	clean_strs(pip->args);
	clean_strs(pip->paths);
	free_path(pip->path);
}

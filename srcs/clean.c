/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 10:45:18 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/01 13:31:29 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static void	clean_strs(char **strs)
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

void	clean_pip(t_pipex *pip)
{
	clean_strs(pip->in_args);
	clean_strs(pip->out_args);
	clean_strs(pip->paths);
	free_path(pip->path);
}

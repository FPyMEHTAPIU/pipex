/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:35:51 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/15 15:26:08 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

char	**fetch_paths(char **envp)
{
	int		i;
	char	*check_path;
	char	**paths;

	i = 0;
	paths = NULL;
	check_path = NULL;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
		{
			check_path = ft_strdup(envp[i]);
			if (!check_path)
				error_ret(6, NULL);
		}
		i++;
	}
	if (check_path)
	{
		paths = ft_split(check_path + 5, ':');
		free(check_path);
		if (!paths)
			error_ret(6, NULL);
	}
	return (paths);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:35:51 by msavelie          #+#    #+#             */
/*   Updated: 2024/10/30 15:42:40 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

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
				error_ret(6);
		}
		i++;
	}
	if (check_path)
	{
		paths = ft_split(check_path + 5, ':');
		free(check_path);
		if (!paths)
			error_ret(6);
	}
	return (paths);
}

char	*check_paths_access(char **paths, char **args, t_pipex *pip)
{
	int		i;
	char	*path;
	size_t	path_len;

	if (!paths || !*paths)
		exit (1);
	i = 0;
	while (paths[i])
	{
		path_len = ft_strlen(paths[i]) + ft_strlen(args[0]) + 2;
		path = ft_calloc(path_len, sizeof(char));
		ft_strlcpy(path, paths[i], path_len);
		ft_strlcat(path, "/", path_len);
		ft_strlcat(path, args[0], path_len);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	perror(args[0]);
	clean_pip(pip);
	exit (1);
}

char	**split_and_check(char *str, char del, t_pipex *pip)
{
	char	**strs;

	strs = ft_split(str, del);
	if (!strs || !*strs)
	{
		perror("split");
		clean_pip(pip);
		exit (1);
	}
	return (strs);
}

char	*parse_args(char **argv, t_pipex *pip)
{
	char	*path;

	pip->in_args = split_and_check(argv[2], ' ', pip);
	pip->out_args = split_and_check(argv[3], ' ', pip);
	path = check_paths_access(pip->paths, pip->in_args, pip);
	return (path);
}

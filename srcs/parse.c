/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:35:51 by msavelie          #+#    #+#             */
/*   Updated: 2024/10/30 11:17:09 by msavelie         ###   ########.fr       */
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
				perror("Malloc error!"), exit(1);
		}
		i++;
	}
	if (check_path)
	{
		paths = ft_split(check_path + 5, ':');
		free(check_path);
		if (!paths)
			perror("Malloc error!"), exit(1);
	}
	return (paths);
}

static char	*check_paths_access(t_pipex *pip, char **paths)
{
	int		i;
	char	*path;
	size_t	path_len;

	if (!paths || !*paths)
		exit (1);
	i = 0;
	while (paths[i])
	{
		path_len = ft_strlen(paths[i]) + ft_strlen(pip->in_args[0]) + 2;
		path = ft_calloc(path_len, sizeof(char));
		ft_strlcpy(path, paths[i], path_len);
		ft_strlcat(path, "/", path_len);
		ft_strlcat(path, pip->in_args[0], path_len);
		if (access(paths[i], X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	perror("You don't have access!");
	ft_free_strs(paths, count_strs(paths));
	exit (1);
}

char	**split_and_check(char *str, char del, t_pipex *pip)
{
	char	**strs;

	strs = ft_split(str, del);
	if (!strs || !*strs)
	{
		perror("Split error!\n");
		// free the rest memory inside the struct
		clean_pip(pip);
		exit (1);
	}
	ft_printf("%d\n", pip->fd_in);
	return (strs);
}

char	*parse_args(char **argv, t_pipex *pip)
{
	char	*path;

	// for multiple num of args - run while until argv[i + 1] != NULL
	pip->fd_in = open(argv[1], O_RDONLY);
	pip->fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644 );
	pip->in_args = split_and_check(argv[2], ' ', pip);
	pip->out_args = split_and_check(argv[3], ' ', pip);
	path = check_paths_access(pip, pip->paths);
	
	return (path);
}

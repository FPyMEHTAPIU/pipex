/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:13:03 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/28 15:06:20 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

static void	check_is_dir(char *arg, t_pipex* pip)
{
	int	fd;

	fd = open(arg, O_DIRECTORY);
	if (fd >= 0)
	{
		close(fd);
		clean_pip(pip);
		pip->exit_code = 126;
		print_exit("Is a directory\n", arg, pip->exit_code);
	}
}

char	*check_paths(char **paths, char **args)
{
	int		i;
	char	*path;
	size_t	path_len;

	if (!paths || !*paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path_len = ft_strlen(paths[i]) + ft_strlen(args[0]) + 2;
		path = ft_calloc(path_len, sizeof(char));
		if (!path)
		{
			ft_putstr_fd("Malloc failed\n", 2);
			return (NULL);
		}
		ft_strlcpy(path, paths[i], path_len);
		ft_strlcat(path, "/", path_len);
		ft_strlcat(path, args[0], path_len);
		if (access(path, F_OK) == 0)
			return (path);
		free_path(path);
		i++;
	}
	return (NULL);
}

char	*check_paths_access(char **paths, char **args, char *arg, t_pipex *pip)
{
	char	*path;

	args = check_args(args);
	check_is_dir(arg, pip);
	if (args[0][0] == '/' || args[0][0] == '.')
		return (ft_strdup(args[0]));
	else
	{
		if (ft_strchr(arg, '/'))
			return (ft_strdup(arg));
		path = check_paths(paths, args);
		if (!path || args[0][0] == '\0')
		{
			clean_pip(pip);
			if (path)
				free(path);
			pip->exit_code = 127;
			print_exit("command not found\n", arg, pip->exit_code);
		}
		check_is_dir(arg, pip);
		return (path);
	}
}

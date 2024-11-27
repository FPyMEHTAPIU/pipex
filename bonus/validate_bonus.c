/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:13:03 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/27 12:39:51 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

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
		return (path);
	}
}

int	check_permission(t_pipex *pip, char **argv, int arg, bool first)
{
	if (first)
	{
		if (access(argv[1], F_OK) != 0)
			exit_child(pip, argv[1], 1, arg);
		pip->fd_in = open(argv[1], O_RDONLY);
		if (pip->fd_in == -1)
			exit_child(pip, argv[1], 126, arg);
	}
	else
	{
		if (pip->is_heredoc == 1)
			pip->fd_out = open(argv[2 + pip->mid_args], O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			pip->fd_out = open(argv[2 + pip->mid_args], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pip->fd_out == -1)
			exit_child(pip, argv[2 + pip->mid_args], 126, arg);
	}
	return (0);
}

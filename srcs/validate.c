/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:13:03 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/13 16:32:19 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

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
		//ft_printf("path in check: %s\n", path);
		if (access(path, X_OK) == 0)
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
		path = check_paths(paths, args);
		//ft_printf("path inout check: %s\n", path);
		if (!path || args[0][0] == '\0')
		{
			clean_pip(pip);
			if (path)
				free(path);
			pip->exit_code = 127;
			//ft_printf("thread: %d\n", pip->thread);
			print_exit("command not found\n", arg, pip->exit_code);
		}
		return (path);
	}
}

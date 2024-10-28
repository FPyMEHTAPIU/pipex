/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:35:51 by msavelie          #+#    #+#             */
/*   Updated: 2024/10/28 17:32:36 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	**fetch_paths(char **envp)
{
	int		i;
	char	*check_path;
	char	**paths;

	i = 0;
	while (envp[i]) {
		if (ft_strnstr(envp[i], "PATH", 4))
		{
			check_path = ft_strdup(envp[i]);
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
	ft_printf("%d\n", pip->fd_in[0]);
	return (strs);
}

char	*parse_args(char **argv, t_pipex *pip)
{
	char	*path;
	size_t	path_len;

	// for multiple num of args - run while until argv[i + 1] != NULL
	pip->fd_in = open(argv[1], O_RDONLY);
	pip->fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644 );
	pip->in_args = split_and_check(argv[2], ' ', pip);
	pip->out_args = split_and_check(argv[3], ' ', pip);
	path_len = ft_strlen("/bin/") + ft_strlen(pip->in_args[0]) + 1;
	path = ft_calloc(path_len, sizeof(char));
	ft_strlcpy(path, "/bin/", path_len);
	ft_strlcat(path, pip->in_args[0], path_len);
	return (path);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:35:51 by msavelie          #+#    #+#             */
/*   Updated: 2024/10/28 12:16:41 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

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
	pip->in_args = split_and_check(argv[2], ' ', pip);
	pip->out_args = split_and_check(argv[3], ' ', pip);
	path_len = ft_strlen("/bin/") + ft_strlen(pip->in_args[0]) + 1;
	path = ft_calloc(path_len, sizeof(char));
	ft_strlcpy(path, "/bin/", path_len);
	ft_strlcat(path, pip->in_args[0], path_len);
	return (path);
}

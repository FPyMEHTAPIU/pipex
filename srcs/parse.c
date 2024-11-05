/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:35:51 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/05 13:48:23 by msavelie         ###   ########.fr       */
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

char	*check_paths_access(char **paths, char **args, char **argv, t_pipex *pip)
{
	char	*path;
	int		excode;

	excode = 0;
	args = check_args(args);
	if (args[0][0] == '/' || args[0][0] == '.')
		return (ft_strdup(args[0]));
	else
	{
		if (pip->thread != 0)
			excode = 127;
		path = check_paths(paths, args);
		if (!path || args[0][0] == '\0')
		{
			clean_pip(pip);
			if (path)
				free(path);
			print_exit("command not found\n", argv[2 + pip->thread], excode);
		}
		return (path);
	}
	return (argv[0]);
	//clean_pip(pip);
	//exit (1);
}

char	**split_and_check(char *str, char del, t_pipex *pip)
{
	char	**strs;
	bool	spaces_only;

	if (!str || !*str)
		return (NULL);
	spaces_only = is_space_only(str);
	if (spaces_only)
		return (NULL);
	strs = ft_split(str, del);
	if (!strs)
	{
		perror("split");
		clean_pip(pip);
		exit (1);
	}
	return (strs);
}

void	parse_args(char **argv, t_pipex *pip)
{
	pip->in_args = split_and_check(argv[2], ' ', pip);
	pip->out_args = split_and_check(argv[3], ' ', pip);
}

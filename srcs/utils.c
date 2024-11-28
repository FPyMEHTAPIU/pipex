/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 10:48:42 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/28 15:48:08 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	count_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i++])
		;
	return (i);
}

char	**check_args(char **args)
{
	static char	*new_args[] = {"", NULL};

	if (!args)
		return (new_args);
	return (args);
}

bool	is_space_only(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			i++;
		else
			return (false);
	}
	return (true);
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

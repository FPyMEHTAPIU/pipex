/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 10:48:42 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/01 12:48:01 by msavelie         ###   ########.fr       */
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
	static char *new_args[] = { "", NULL };

	if (!args)
		return (new_args);
	return (args);
}

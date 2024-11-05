/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:51:46 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/05 14:18:37 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	print_exit(char *mes, char *cmd, int exit_code)
{
	char	*full_msg;
	int		mes_len;

	mes_len = ft_strlen(mes) + ft_strlen(cmd) + 3;
	full_msg = ft_calloc(mes_len, sizeof(char));
	if (cmd)
		ft_strlcpy(full_msg, cmd, mes_len);
	ft_strlcat(full_msg, ": ", mes_len);
	ft_strlcat(full_msg, mes, mes_len);
	ft_putstr_fd(full_msg, 2);
	free (full_msg);
	exit (exit_code);
}

void	exit_child(t_pipex *pip, char *arg, int exit_code)
{
	clean_pip(pip);
	perror(arg);
	exit(exit_code);
}

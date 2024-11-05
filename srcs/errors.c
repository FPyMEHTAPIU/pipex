/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:51:46 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/05 13:19:41 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	print_exit(char *mes, char *cmd, int exit_code)
{
	char	*full_msg;
	int		mes_len;

	mes_len = 0;
	if (!cmd || !*cmd)
		mes_len = -1;
	if (mes_len == 0)
	{
		mes_len = ft_strlen(mes) + ft_strlen(cmd) + 3;
		full_msg = ft_calloc(mes_len, sizeof(char));
		ft_strlcpy(full_msg, cmd, mes_len);
		ft_strlcat(full_msg, ": ", mes_len);
		ft_strlcat(full_msg, mes, mes_len);
		ft_putstr_fd(full_msg, 2);
		free (full_msg);
	}
	else
		ft_putstr_fd(mes, 2);
	exit (exit_code);
}

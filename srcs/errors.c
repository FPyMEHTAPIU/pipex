/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msavelie <msavelie@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:51:46 by msavelie          #+#    #+#             */
/*   Updated: 2024/11/28 11:51:00 by msavelie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	print_exit(char *mes, char *cmd, int exit_code)
{
	char	*full_msg;
	int		mes_len;

	mes_len = ft_strlen(mes) + ft_strlen(cmd) + 3;
	full_msg = ft_calloc(mes_len, sizeof(char));
	if (!full_msg)
	{
		ft_putstr_fd("Malloc failed\n", 2);
		exit (exit_code);
	}
	if (cmd)
		ft_strlcpy(full_msg, cmd, mes_len);
	ft_strlcat(full_msg, ": ", mes_len);
	ft_strlcat(full_msg, mes, mes_len);
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(full_msg, 2);
	free (full_msg);
	exit (exit_code);
}

void	exit_child(t_pipex *pip, char *arg, int exit_code)
{
	pip->exit_code = exit_code;
	clean_pip(pip);
	ft_putstr_fd("pipex: ", 2);
	if (!*arg)
		ft_putstr_fd(": ", 2);
	perror(arg);
	close(pip->pipfd[1]);
	close(pip->pipfd[0]);
	exit(exit_code);
}

int	error_ret(int type, char *arg)
{
	ft_putstr_fd("pipex: ", 2);
	if (type == 1)
		ft_putstr_fd("Incorrect number of arguments! \
Provide 4 arguments!\n", 2);
	else if (type == 2)
		perror(arg);
	else if (type == 3)
		perror(arg);
	else if (type == 4)
		ft_putstr_fd("Pipe failed\n", 2);
	else if (type == 5)
		ft_putstr_fd("Fork failed\n", 2);
	else if (type == 6)
		ft_putstr_fd("Malloc failed\n", 2);
	exit(0);
}

void	error_check(int argc)
{
	if (argc != 5)
		error_ret(1, NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbouizem <djihane.bouizem@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 03:35:00 by dbouizem          #+#    #+#             */
/*   Updated: 2026/02/20 03:35:00 by dbouizem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void	read_fd(int fd)
{
	char	*line;
	int		i;

	i = 1;
	line = get_next_line(fd);
	while (line)
	{
		printf("M[%d]: %s", i, line);
		free(line);
		i++;
		line = get_next_line(fd);
	}
}

int	main(int argc, char **argv)
{
	int	fd;

	if (argc != 2)
		return (printf("Usage: %s <file>\n", argv[0]), 1);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (perror("open"), 1);
	read_fd(fd);
	close(fd);
	return (0);
}

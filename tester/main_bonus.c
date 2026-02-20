/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbouizem <djihane.bouizem@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 03:35:00 by dbouizem          #+#    #+#             */
/*   Updated: 2026/02/20 03:35:00 by dbouizem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line_bonus.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void	print_line(char *line, int index)
{
	if (line)
		printf("B%d: %s", index, line);
	else
		printf("B%d: NULL\n", index);
}

static int	read_three(int *fd)
{
	char	*line[3];

	line[0] = get_next_line(fd[0]);
	line[1] = get_next_line(fd[1]);
	line[2] = get_next_line(fd[2]);
	if (!line[0] && !line[1] && !line[2])
		return (0);
	print_line(line[0], 1);
	print_line(line[1], 2);
	print_line(line[2], 3);
	free(line[0]);
	free(line[1]);
	free(line[2]);
	return (1);
}

int	main(int argc, char **argv)
{
	int	fd[3];

	if (argc != 4)
		return (printf("Usage: %s <file1> <file2> <file3>\n", argv[0]), 1);
	fd[0] = open(argv[1], O_RDONLY);
	fd[1] = open(argv[2], O_RDONLY);
	fd[2] = open(argv[3], O_RDONLY);
	if (fd[0] < 0 || fd[1] < 0 || fd[2] < 0)
		return (perror("open"), 1);
	while (read_three(fd))
		;
	close(fd[0]);
	close(fd[1]);
	close(fd[2]);
	return (0);
}

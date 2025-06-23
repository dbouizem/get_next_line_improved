/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbouizem <djihane.bouizem@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 20:16:27 by dbouizem          #+#    #+#             */
/*   Updated: 2025/06/23 22:01:47 by dbouizem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <stdio.h>
#include <fcntl.h>

int	main(void)
{
	int		fd1, fd2, fd3;
	char	*line1, *line2, *line3;
	int		i = 0;

	fd1 = open("1.txt", O_RDONLY);
	fd2 = open("2.txt", O_RDONLY);
	fd3 = open("3.txt", O_RDONLY);
	if (fd1 < 0 || fd2 < 0 || fd3 < 0)
		return (perror("Error open !!"), 1);

	while (i < 5)
	{
		line1 = get_next_line(fd1);
		line2 = get_next_line(fd2);
		line3 = get_next_line(fd3);
		printf("\n-----------Call %d-----\n", i);
		printf("fd1: %s", line1 ? line1 : "NULL\n");
		printf("fd2: %s", line2 ? line2 : "NULL\n");
		printf("fd3: %s", line3 ? line3 : "NULL\n");
		free(line1);
		free(line2);
		free(line3);
		i++;
	}
	close(fd1);
	close(fd2);
	close(fd3);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbouizem <djihane.bouizem@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 03:11:12 by dbouizem          #+#    #+#             */
/*   Updated: 2025/06/18 04:32:56 by dbouizem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static int	extend_buf(t_dynbuf *buf, ssize_t br)
{
	char	*new_data;
	size_t	new_cap;

	if (buf->capacity)
		new_cap = buf->capacity * 2;
	else
		new_cap = (size_t)br + 1;
	while (new_cap < buf->len + (size_t)br + 1)
		new_cap *= 2;
	new_data = malloc(new_cap);
	if (!new_data)
		return (0);
	if (buf->data)
	{
		ft_memcpy(new_data, buf->data, buf->len);
		free(buf->data);
	}
	buf->data = new_data;
	buf->capacity = new_cap;
	return (1);
}

static char	*read_loop(int fd, char *buff, ssize_t br, t_dynbuf buf)
{
	ssize_t	i;

	while (br > 0)
	{
		if (buf.capacity < buf.len + (size_t)br + 1)
		{
			if (!extend_buf(&buf, br))
				return (free(buff), free(buf.data), NULL);
		}
		ft_memcpy(buf.data + buf.len, buff, br);
		buf.len += br;
		buf.data[buf.len] = '\0';
		i = 0;
		while (i < br && buff[i] != '\n')
			i++;
		if (i < br)
			break ;
		br = read(fd, buff, BUFFER_SIZE);
	}
	free(buff);
	if (br < 0)
		return (free(buf.data), NULL);
	return (buf.data);
}

static char	*read_file(int fd, char *stock)
{
	t_dynbuf	buf;
	char		*buff;
	ssize_t		br;
	size_t		len;

	buff = malloc(BUFFER_SIZE);
	if (!buff)
		return (free(stock), NULL);
	buf = (t_dynbuf){0};
	if (stock)
	{
		len = 0;
		while (stock[len])
			len++;
		buf.data = malloc(len + 1);
		if (!buf.data)
			return (free(buff), free(stock), NULL);
		buf.capacity = len + 1;
		ft_memcpy(buf.data, stock, len);
		buf.data[len] = '\0';
		buf.len = len;
		free(stock);
	}
	br = read(fd, buff, BUFFER_SIZE);
	return (read_loop(fd, buff, br, buf));
}

static char	*next_line(char *stock)
{
	size_t	i;
	char	*rest;

	if (!stock)
		return (NULL);
	i = 0;
	while (stock[i] && stock[i] != '\n')
		i++;
	if (stock[i] != '\n')
	{
		free(stock);
		return (NULL);
	}
	rest = ft_strdup(stock + i + 1);
	free(stock);
	if (!rest)
		return (NULL);
	if (!*rest)
		return (free(rest), NULL);
	return (rest);
}

char	*get_next_line(int fd)
{
	static t_fd_list	*fd_list = NULL;
	t_fd_list			*current;
	char				*line;
	size_t				len;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (remove_fd_node(&fd_list, fd), NULL);
	current = get_fd_node(&fd_list, fd);
	if (!current)
		return (NULL);
	current->stock = read_file(fd, current->stock);
	if (current->stock == NULL)
		return (remove_fd_node(&fd_list, fd), NULL);
	len = 0;
	while (current->stock[len] && current->stock[len] != '\n')
		len++;
	if (current->stock[len] == '\n')
		len++;
	line = malloc(len + 1);
	if (!line || !ft_strlcpy(line, current->stock, len + 1))
		return (remove_fd_node(&fd_list, fd), free(line), NULL);
	current->stock = next_line(current->stock);
	if (current->stock == NULL)
		remove_fd_node(&fd_list, fd);
	return (line);
}

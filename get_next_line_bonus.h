/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbouizem <djihane.bouizem@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 03:10:59 by dbouizem          #+#    #+#             */
/*   Updated: 2025/06/18 03:54:48 by dbouizem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct s_dynbuf
{
	char	*data;
	size_t	len;
	size_t	capacity;
}	t_dynbuf;

typedef struct s_fd_list
{
	int					fd;
	char				*stock;
	struct s_fd_list	*next;
}	t_fd_list;

char		*get_next_line(int fd);

t_fd_list	*get_fd_node(t_fd_list **list, int fd);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
void		remove_fd_node(t_fd_list **list, int fd);
void		*ft_memcpy(void *dest, const void *src, size_t n);
char		*ft_strdup(const char *s);

#endif

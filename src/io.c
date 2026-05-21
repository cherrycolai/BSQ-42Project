/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idilsincer <idilsincer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:24:16 by idilsincer        #+#    #+#             */
/*   Updated: 2026/05/21 23:03:00 by idilsincer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "bsq.h"

// Equavilent to ft_putchar
void	out_char(char c)
{
	write(1, &c, 1);
}

// Equavilent to ft_putstr
void	out_str(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(1, &s[i], 1);
		i++;
	}
}

/*Concatenate a new chunk of 'n' bytes onto an existing buffer of 'total' 
bytes and return the freshly allocated result (NUL-terminated). 
The caller is responsible for freeing both 'buf' and the returned pointer. 
Returns NULL on allocation failure. 
*/
char	*append_chunk(char *buf, int total, char *chunk, int n)
{
	char	*out;
	int		i;

	out = (char *)malloc(total + n + 1);
	if (!out)
		return (0);
	i = 0;
	while (i < total)
	{
		out[i] = buf[i];
		i++;
	}
	i = 0;
	while (i < n)
	{
		out[total + i] = chunk[i];
		i++;
	}
	out[total + n] = '\0';
	return (out);
}

/*Core read loop: repeatedly fills a BSQ_CHUNK-sized stack buffer 
from 'fd' and grows the heap buffer via append_chunk() until EOF or error. 
The previous heap buffer is freed after every successful append 
so there is never more than one stale copy alive at a time. 
Returns NULL and frees 'buf' on a read error. 
*/
static char	*read_loop(int fd, char *buf)
{
	char	chunk[BSQ_CHUNK];
	char	*tmp;
	int		total;
	int		n;

	total = 0;
	n = read(fd, chunk, BSQ_CHUNK);
	while (n > 0)
	{
		tmp = append_chunk(buf, total, chunk, n);
		free(buf);
		if (!tmp)
			return (0);
		buf = tmp;
		total += n;
		n = read(fd, chunk, BSQ_CHUNK);
	}
	if (n < 0)
	{
		free(buf);
		return (0);
	}
	return (buf);
}

/*Read the entire content of file descriptor 'fd' into a heap-allocated, 
NUL-terminated string. 
Returns NULL on any allocation or read failure. 
The caller must free the returned pointer. 
*/
char	*slurp(int fd)
{
	char	*buf;

	buf = (char *)malloc(1);
	if (!buf)
		return (0);
	buf[0] = '\0';
	return (read_loop(fd, buf));
}

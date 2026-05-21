/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idilsincer <idilsincer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:24:20 by idilsincer        #+#    #+#             */
/*   Updated: 2026/05/21 18:12:31 by idilsincer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsq.h"

int	str_to_int(char *s)
{
	int	n;
	int	i;

	n = 0;
	i = 0;
	while (s[i] >= '0' && s[i] <= '9')
	{
		if (n > 214748364 || (n == 214748364 && s[i] > '7'))
			return (-1);
		n = n * 10 + (s[i] - '0');
		i++;
	}
	return (n);
}

int	printable3_distinct(char a, char b, char d)
{
	if (a < 32 || a > 126)
		return (0);
	if (b < 32 || b > 126)
		return (0);
	if (d < 32 || d > 126)
		return (0);
	if (a == b || a == d || b == d)
		return (0);
	return (1);
}

int	parse_header(char *buf, t_canvas *c, int *offset)
{
	int	nl;
	int	i;

	nl = 0;
	while (buf[nl] && buf[nl] != '\n')
		nl++;
	if (buf[nl] != '\n' || nl < 4)
		return (0);
	i = 0;
	while (i < nl - 3)
	{
		if (buf[i] < '0' || buf[i] > '9')
			return (0);
		i++;
	}
	c->rows = str_to_int(buf);
	if (c->rows <= 0)
		return (0);
	if (!printable3_distinct(buf[nl - 3], buf[nl - 2], buf[nl - 1]))
		return (0);
	c->vacant = buf[nl - 3];
	c->blocked = buf[nl - 2];
	c->marked = buf[nl - 1];
	*offset = nl + 1;
	return (1);
}

int	detect_cols(char *buf, int offset)
{
	int	n;

	n = 0;
	while (buf[offset + n] && buf[offset + n] != '\n')
		n++;
	return (n);
}

int	check_row(char *buf, int offset, t_canvas *c, int row_idx)
{
	int	j;

	j = 0;
	while (j < c->cols)
	{
		if (buf[offset + j] != c->vacant && buf[offset + j] != c->blocked)
			return (-1);
		c->grid[row_idx][j] = buf[offset + j];
		j++;
	}
	if (buf[offset + j] == '\n')
		return (offset + j + 1);
	if (buf[offset + j] == '\0' && row_idx == c->rows - 1)
		return (offset + j);
	return (-1);
}

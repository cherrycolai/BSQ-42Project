/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idilsincer <idilsincer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:24:20 by idilsincer        #+#    #+#             */
/*   Updated: 2026/05/21 23:02:45 by idilsincer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsq.h"

/*Convert a decimal ASCII string to a non-negative int. 
Stops at the first non-digit character (no sign support). 
Returns -1 on overflow (guards against values > INT_MAX). 
*/
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

/*Return 1 if all three characters are printable 
ASCII (0x20–0x7E) and mutually distinct. 
Used to validate the three map symbols 
(vacant / blocked / marked) in the header line. 
*/
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

/*Parse the first line of 'buf' as the map header. 
Expected format: "<rows><vacant><blocked><marked>\n". 
Everything before the last three characters must be decimal digits. 
The three trailing characters must be printable and mutually distinct. 
The line must be at least 4 characters long (1 digit + 3 symbols). 
Populates c->rows, c->vacant, c->blocked, c->marked. 
Sets *offset to the byte index of the first data row on success. 
Returns 1 on success, 0 on any format error.
*/
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

/*Count the number of characters on the first data row
starting at 'buf[offset]', 
stopping at '\n' or '\0'. This value becomes c->cols */
int	detect_cols(char *buf, int offset)
{
	int	n;

	n = 0;
	while (buf[offset + n] && buf[offset + n] != '\n')
		n++;
	return (n);
}

/*Validate and copy one grid row into c->grid[row_idx].
Every character must be either c->vacant or c->blocked. 
The row must be exactly c->cols wide,
terminated by '\n' (or '\0' for the very last row). 
Returns the buffer offset that immediately follows this row,
or -1 on any validation failure. 
*/
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grid.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idilsincer <idilsincer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:24:11 by idilsincer        #+#    #+#             */
/*   Updated: 2026/05/21 22:56:21 by idilsincer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "bsq.h"

/*Free the first 'n' row pointers of a partially-allocated grid, 
then free the array itself. Used for cleanup when build_grid() fails mid-way 
*/
void	free_partial(char **g, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(g[i]);
		i++;
	}
	free(g);
}

/*Allocate a rows × cols grid of chars. 
Each row is an independent heap allocation (no NUL terminator added; 
row width is always exactly 'cols' bytes). 
Returns NULL and frees all partial allocations on failure. 
*/
char	**build_grid(int rows, int cols)
{
	char	**g;
	int		i;

	g = (char **)malloc(sizeof(char *) * rows);
	if (!g)
		return (0);
	i = 0;
	while (i < rows)
	{
		g[i] = (char *)malloc(sizeof(char) * cols);
		if (!g[i])
		{
			free_partial(g, i);
			return (0);
		}
		i++;
	}
	return (g);
}

/*Allocate c->grid and fill it from 'buf' starting at 'offset'. 
Delegates row-by-row validation to check_row(). 
After all rows are consumed, 
verifies that 'buf' is exhausted (no trailing garbage allowed). 
Returns 1 on success, 0 on any error (grid is freed by the caller). 
*/
int	populate_grid(char *buf, int offset, t_canvas *c)
{
	int	i;

	c->grid = build_grid(c->rows, c->cols);
	if (!c->grid)
		return (0);
	i = 0;
	while (i < c->rows)
	{
		offset = check_row(buf, offset, c, i);
		if (offset < 0)
			return (0);
		i++;
	}
	if (buf[offset] != '\0')
		return (0);
	return (1);
}

/*Release all memory owned by a t_canvas. 
Safe to call even if c->grid is NULL (e.g. after a failed build). */
void	free_canvas(t_canvas *c)
{
	int	i;

	if (!c->grid)
		return ;
	i = 0;
	while (i < c->rows)
	{
		free(c->grid[i]);
		i++;
	}
	free(c->grid);
	c->grid = 0;
}

/*Print the entire grid to stdout, one row per line. 
Each cell is written with out_char() to stay within 
the no-printf constraint of the project. 
*/
void	print_canvas(t_canvas *c)
{
	int	i;
	int	j;

	i = 0;
	while (i < c->rows)
	{
		j = 0;
		while (j < c->cols)
		{
			out_char(c->grid[i][j]);
			j++;
		}
		out_char('\n');
		i++;
	}
}

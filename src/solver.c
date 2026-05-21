/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idilsincer <idilsincer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:25:26 by idilsincer        #+#    #+#             */
/*   Updated: 2026/05/21 23:00:49 by idilsincer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "bsq.h"

// Return the smallest of three unsigned integers
unsigned int	min3(unsigned int a, unsigned int b, unsigned int d)
{
	unsigned int	m;

	m = a;
	if (b < m)
		m = b;
	if (d < m)
		m = d;
	return (m);
}

/*Fill a single cell (i, j) of the DP table
using the classic "maximal square" recurrence. */
void	fill_cell(t_canvas *c, unsigned int *dp, int i, int j)
{
	unsigned int	a;
	unsigned int	b;
	unsigned int	d;

	if (c->grid[i][j] == c->blocked)
	{
		dp[i * c->cols + j] = 0;
		return ;
	}
	if (i == 0 || j == 0)
	{
		dp[i * c->cols + j] = 1;
		return ;
	}
	a = dp[(i - 1) * c->cols + j];
	b = dp[i * c->cols + (j - 1)];
	d = dp[(i - 1) * c->cols + (j - 1)];
	dp[i * c->cols + j] = min3(a, b, d) + 1;
}

/*Allocate and fill the entire DP table for the canvas. 
The table is stored as a flat rows×cols array (row-major order). 
Returns NULL on allocation failure; the caller must free the result. */
unsigned int	*build_dp(t_canvas *c)
{
	unsigned int	*dp;
	int				i;
	int				j;

	dp = (unsigned int *)malloc(sizeof(unsigned int) * c->rows * c->cols);
	if (!dp)
		return (0);
	i = 0;
	while (i < c->rows)
	{
		j = 0;
		while (j < c->cols)
		{
			fill_cell(c, dp, i, j);
			j++;
		}
		i++;
	}
	return (dp);
}

/*Scan the completed DP table for the cell with the highest value.
That cell is the bottom-right corner of the optimal square.
The result is returned as a t_square (side, row, col).
If every cell is 0 (all blocked), side == 0 is returned.
Tie-breaking: the first maximum found in row-major order is kept,
which corresponds to the topmost, then leftmost, winning corner.
*/
t_square	find_origin(unsigned int *dp, int rows, int cols)
{
	t_square	sq;
	int			i;
	int			j;

	sq.side = 0;
	sq.row = 0;
	sq.col = 0;
	i = 0;
	while (i < rows)
	{
		j = 0;
		while (j < cols)
		{
			if ((int)dp[i * cols + j] > sq.side)
			{
				sq.side = (int)dp[i * cols + j];
				sq.row = i;
				sq.col = j;
			}
			j++;
		}
		i++;
	}
	return (sq);
}

/*Entry point for the BSQ solver.
Builds the DP table, locates the optimal square, frees the table,
and returns the result as a t_square.
Returns a zero-side square if memory allocation fails.
*/
t_square	solve(t_canvas *c)
{
	unsigned int	*dp;
	t_square		sq;
}

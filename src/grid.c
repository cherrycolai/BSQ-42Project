/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grid.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idilsincer <idilsincer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:24:11 by idilsincer        #+#    #+#             */
/*   Updated: 2026/05/21 18:12:50 by idilsincer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "bsq.h"

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

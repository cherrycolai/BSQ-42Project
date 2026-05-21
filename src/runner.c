/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idilsincer <idilsincer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:25:05 by idilsincer        #+#    #+#             */
/*   Updated: 2026/05/21 18:12:25 by idilsincer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "bsq.h"

void	stamp_square(t_canvas *c, t_square sq)
{
	int	i;
	int	j;
	int	r0;
	int	c0;

	if (sq.side == 0)
		return ;
	r0 = sq.row - sq.side + 1;
	c0 = sq.col - sq.side + 1;
	i = r0;
	while (i <= sq.row)
	{
		j = c0;
		while (j <= sq.col)
		{
			c->grid[i][j] = c->marked;
			j++;
		}
		i++;
	}
}

int	build_canvas(char *buf, t_canvas *c, int *offset)
{
	if (!parse_header(buf, c, offset))
		return (0);
	c->cols = detect_cols(buf, *offset);
	if (c->cols <= 0 || c->rows <= 0)
		return (0);
	if (!populate_grid(buf, *offset, c))
		return (0);
	return (1);
}

void	handle_buffer(char *buf)
{
	t_canvas	canvas;
	t_square	sq;
	int			offset;

	canvas.grid = 0;
	canvas.rows = 0;
	canvas.cols = 0;
	if (!buf || !build_canvas(buf, &canvas, &offset))
	{
		out_str("map error\n");
		free(buf);
		free_canvas(&canvas);
		return ;
	}
	sq = solve(&canvas);
	stamp_square(&canvas, sq);
	print_canvas(&canvas);
	free(buf);
	free_canvas(&canvas);
}

void	process_file(char *path)
{
	char	*buf;
	int		fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		out_str("map error\n");
		return ;
	}
	buf = slurp(fd);
	close(fd);
}

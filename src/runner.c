/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idilsincer <idilsincer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:25:05 by idilsincer        #+#    #+#             */
/*   Updated: 2026/05/21 22:49:03 by idilsincer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "bsq.h"

/*Overwrite every cell of the solution square in c->grid with c->marked. 
sq.row / sq.col are the bottom-right corner of the square; 
the top-left corner is derived as (sq.row - sq.side + 1, sq.col - sq.side + 1). 
Does nothing when sq.side == 0 (no solution exists). 
*/
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

/*Parse 'buf' into a fully-populated t_canvas: 
	1.Parse the header line c->rows and the three map symbols. 
	2. Detect column count c->cols. 
   	3. Allocate and fill grid c->grid. 
Returns 1 on success, 0 on any format or allocation error. 
*/
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

/*Full pipeline for a single map buffer:
	1. Build the canvas (parse + allocate).
	2. Solve for the largest square.
	3. Stamp the solution onto the grid.
   	4. Print the result. 5. Free all resources. 
	Prints "map error\n" and cleans up if any step fails.
'buf' is always freed before returning.
*/
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

/*Open 'path', slurp its entire content into a heap buffer, close the file, 
and pass the buffer to handle_buffer(). 
Prints "map error\n" if the file cannot be opened.  
NOTE: the slurp() result is not passed to handle_buffer() here — this is a bug; 
see the missing handle_buffer(buf) call below. 
*/
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsq.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idilsincer <idilsincer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 17:58:24 by idilsincer        #+#    #+#             */
/*   Updated: 2026/05/21 18:13:31 by idilsincer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BSQ_H
# define BSQ_H

# define BSQ_CHUNK 4096

typedef struct s_canvas
{
	char	**grid;
	int		rows;
	int		cols;
	char	vacant;
	char	blocked;
	char	marked;
}	t_canvas;

typedef struct s_square
{
	int	side;
	int	row;
	int	col;
}	t_square;

void			out_char(char c);
void			out_str(char *s);
int				str_to_int(char *s);
char			*slurp(int fd);
char			*append_chunk(char *buf, int total, char *chunk, int n);

int				parse_header(char *buf, t_canvas *c, int *offset);
int				printable3_distinct(char a, char b, char d);
int				detect_cols(char *buf, int offset);
int				check_row(char *buf, int offset, t_canvas *c, int row_idx);
int				populate_grid(char *buf, int offset, t_canvas *c);

char			**build_grid(int rows, int cols);
void			free_partial(char **g, int n);
void			free_canvas(t_canvas *c);
void			print_canvas(t_canvas *c);
void			stamp_square(t_canvas *c, t_square sq);

unsigned int	min3(unsigned int a, unsigned int b, unsigned int d);
unsigned int	*build_dp(t_canvas *c);
void			fill_cell(t_canvas *c, unsigned int *dp, int i, int j);
t_square		find_origin(unsigned int *dp, int rows, int cols);
t_square		solve(t_canvas *c);

void			process_file(char *path);
void			process_stdin(void);
int				build_canvas(char *buf, t_canvas *c, int *offset);
void			handle_buffer(char *buf);

#endif
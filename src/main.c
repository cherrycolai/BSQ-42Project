/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idilsincer <idilsincer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:24:18 by idilsincer        #+#    #+#             */
/*   Updated: 2026/05/21 23:03:31 by idilsincer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsq.h"

/*No arguments: read the map from stdin via process_stdin()
One or more arguments: treat each as a file path and process 
it with process_file(). 
A blank line is printed between consecutive maps to 
separate their outputs. */
int	main(int argc, char **argv)
{
	int	i;

	if (argc < 2)
	{
		process_stdin();
		return (0);
	}
	i = 1;
	while (i < argc)
	{
		process_file(argv[i]);
		if (i < argc - 1)
			out_char('\n');
		i++;
	}
	return (0);
}

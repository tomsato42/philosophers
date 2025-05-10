/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:00:24 by tomsato           #+#    #+#             */
/*   Updated: 2025/05/06 17:01:42 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_data	*data;

	if (!(5 <= ac && ac <= 6))
	{
		write(2, "Error!\n", 7);
		return (1);
	}
	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	if (init_data(ac, av, data) || init_philos(data))
	{
		clear_data(data);
		free(data);
		return (1);
	}
	start_simulation(data);
	clear_data(data);
	free(data);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:08:27 by tomsato           #+#    #+#             */
/*   Updated: 2025/05/06 18:46:52 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_valid_input(t_data *data)
{
	if (data->num_philos <= 0 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (0);
	if (data->must_eat != -1 && data->must_eat <= 0)
		return (0);
	return (1);
}

int	init_forks(t_data *data)
{
	int	i;

	data->fork_mutexes = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->fork_mutexes)
		return (1);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->fork_mutexes[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->fork_mutexes[i]);
			free(data->fork_mutexes);
			data->fork_mutexes = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_data(int ac, char **av, t_data *data)
{
	if (ac == 6)
		data->must_eat = ft_atol(av[5]);
	else
		data->must_eat = -1;
	data->num_philos = ft_atol(av[1]);
	data->time_to_die = ft_atol(av[2]);
	data->time_to_eat = ft_atol(av[3]);
	data->time_to_sleep = ft_atol(av[4]);
	data->start_time = get_time() + (data->num_philos * 10) + 1000;
	data->stop = 0;
	if (pthread_mutex_init(&data->stop_flag, NULL) != 0)
		return (1);
	if (init_forks(data))
		return (1);
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	if (!is_valid_input(data))
		return (1);
	return (0);
}

int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (1);
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i;
		data->philos[i].last_meal_time = -1;
		data->philos[i].eat_count = 0;
		data->philos[i].left_fork_id = i;
		data->philos[i].right_fork_id = (i + 1) % data->num_philos;
		data->philos[i].data = data;
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	clear_data(t_data *data)
{
	int	i;

	if (data->fork_mutexes)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_destroy(&data->fork_mutexes[i]);
			i++;
		}
		free(data->fork_mutexes);
		data->fork_mutexes = NULL;
	}
	if (data->philos)
	{
		i = -1;
		while (++i < data->num_philos)
		{
			pthread_mutex_destroy(&data->philos[i].meal_mutex);
		}
		free(data->philos);
		data->philos = NULL;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->stop_flag);
}

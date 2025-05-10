/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 21:00:14 by tomsato           #+#    #+#             */
/*   Updated: 2025/05/06 18:18:02 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	change_stop_flag(int n, t_data *data)
{
	pthread_mutex_lock(&data->stop_flag);
	data->stop = n;
	pthread_mutex_unlock(&data->stop_flag);
}

static int	check_philo_death(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < data->num_philos)
	{
		philo = &data->philos[i];
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->last_meal_time != -1)
		{
			if (data->time_to_die < get_time() - philo->last_meal_time)
			{
				change_stop_flag(1, data);
				print_lock(philo, get_status_msg(S_DEAD));
				pthread_mutex_unlock(&philo->meal_mutex);
				return (1);
			}
		}
		pthread_mutex_unlock(&philo->meal_mutex);
		i++;
	}
	return (0);
}

static int	check_all_philos_done(t_data *data)
{
	int	i;
	int	done;

	done = 1;
	if (data->must_eat == -1)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		if (data->philos[i].eat_count < (size_t)data->must_eat)
			done = 0;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		if (!done)
			break ;
		i++;
	}
	if (done)
	{
		change_stop_flag(2, data);
		return (1);
	}
	return (0);
}

void	*monitor(t_data *data)
{
	while (!get_stop_flag(data))
	{
		if (check_philo_death(data))
			return (NULL);
		if (check_all_philos_done(data))
			return (NULL);
	}
	return (NULL);
}

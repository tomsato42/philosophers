/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 21:00:14 by tomsato           #+#    #+#             */
/*   Updated: 2025/05/06 18:12:08 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	change_stop_flag(int n, t_data *data)
{
	pthread_mutex_lock(&data->stop_flag);
	data->stop = n;
	pthread_mutex_unlock(&data->stop_flag);
}

void	*monitor(t_data *data)
{
	int		i;
	t_philo	*philo;
	int		done;

	while (!get_stop_flag(data))
	{
		i = 0;
		while (i < data->num_philos)
		{
			philo = &data->philos[i];
			pthread_mutex_lock(&philo->meal_mutex);
			if (philo->last_meal_time != -1 && data->time_to_die < get_time()
				- philo->last_meal_time)
			{
				change_stop_flag(1, data);
				print_lock(philo, get_status_msg(S_DEAD));
				pthread_mutex_unlock(&philo->meal_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&philo->meal_mutex);
			i++;
		}
		if (data->must_eat != -1)
		{
			done = 1;
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
				return (NULL);
			}
		}
	}
	return (NULL);
}

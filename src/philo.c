/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:58:39 by tomsato           #+#    #+#             */
/*   Updated: 2025/05/10 14:35:34 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (philo->id % 2)
	{
		pthread_mutex_lock(&data->fork_mutexes[philo->right_fork_id]);
		print_lock(philo, "has taken a fork");
		pthread_mutex_lock(&data->fork_mutexes[philo->left_fork_id]);
		print_lock(philo, "has taken a fork");
	}
	else
	{
		usleep(500);
		pthread_mutex_lock(&data->fork_mutexes[philo->left_fork_id]);
		print_lock(philo, "has taken a fork");
		pthread_mutex_lock(&data->fork_mutexes[philo->right_fork_id]);
		print_lock(philo, "has taken a fork");
	}
}

static void	philo_eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (get_stop_flag(data))
		return ;
	take_forks(philo);
	print_lock(philo, get_status_msg(S_EAT));
	pthread_mutex_lock(&philo->meal_mutex);
	philo->eat_count++;
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->meal_mutex);
	usleep(data->time_to_eat * 1000);
	pthread_mutex_unlock(&data->fork_mutexes[philo->right_fork_id]);
	pthread_mutex_unlock(&data->fork_mutexes[philo->left_fork_id]);
}

static void	*handle_single_philo(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->fork_mutexes[0]);
	print_lock(philo, "has taken a fork");
	usleep(data->time_to_die * 1000);
	pthread_mutex_unlock(&data->fork_mutexes[0]);
	return (NULL);
}

void	*philo_routine(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	wait_for_start(philo);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = data->start_time;
	pthread_mutex_unlock(&philo->meal_mutex);
	if (data->num_philos == 1)
		return (handle_single_philo(philo));
	if (!get_stop_flag(data))
		print_lock(philo, get_status_msg(S_THINK));
	while (!get_stop_flag(data))
	{
		philo_eat(philo);
		if (!get_stop_flag(data))
		{
			print_lock(philo, get_status_msg(S_SLEEP));
			usleep(data->time_to_sleep * 1000);
		}
		if (!get_stop_flag(data))
			print_lock(philo, get_status_msg(S_THINK));
		usleep(500);
	}
	return (NULL);
}

void	start_simulation(t_data *data)
{
	int			i;
	int			ret;
	pthread_t	monitor_thread;

	if (data->num_philos < 1)
		return ;
	i = 0;
	while (i < data->num_philos)
	{
		ret = pthread_create(&(data->philos[i].thread), NULL,
				(void *(*)(void *))philo_routine, &(data->philos[i]));
		i++;
	}
	pthread_create(&monitor_thread, NULL, (void *(*)(void *))monitor, data);
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

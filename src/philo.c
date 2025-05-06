/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:58:39 by tomsato           #+#    #+#             */
/*   Updated: 2025/05/06 18:11:13 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_eat(t_philo *philo)
{
	if (!get_stop_flag(philo->data))
	{
		philo->stat = S_EAT;
		print_lock(philo, get_status_msg(philo->stat));
		pthread_mutex_lock(&philo->meal_mutex);
		philo->eat_count++;
		philo->last_meal_time = get_time();
		pthread_mutex_unlock(&philo->meal_mutex);
		usleep(philo->data->time_to_eat * 1000);
	}
}

static void	philo_sleep(t_philo *philo)
{
	if (!get_stop_flag(philo->data))
	{
		philo->stat = S_SLEEP;
		print_lock(philo, get_status_msg(philo->stat));
		usleep(philo->data->time_to_sleep * 1000);
	}
}

static void	philo_think(t_philo *philo)
{
	if (!get_stop_flag(philo->data))
	{
		philo->stat = S_THINK;
		print_lock(philo, get_status_msg(philo->stat));
	}
}

void	*philo_routine(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	wait_for_start(philo);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->meal_mutex);
	while (!get_stop_flag(data))
	{
		if (philo->id % 2)
		{
			pthread_mutex_lock(&data->fork_mutexes[philo->right_fork_id]);
			pthread_mutex_lock(&data->fork_mutexes[philo->left_fork_id]);
		}
		else
		{
			pthread_mutex_lock(&data->fork_mutexes[philo->left_fork_id]);
			pthread_mutex_lock(&data->fork_mutexes[philo->right_fork_id]);
		}
		philo_eat(philo);
		pthread_mutex_unlock(&data->fork_mutexes[philo->right_fork_id]);
		pthread_mutex_unlock(&data->fork_mutexes[philo->left_fork_id]);
		philo_sleep(philo);
		philo_think(philo);
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
	if (data->num_philos == 1)
	{
		data->start_time = get_time();
		printf("0 1 has taken a fork\n");
		usleep(data->time_to_die * 1000);
		printf("%ld 1 died\n", get_time() - data->start_time);
		return ;
	}
	i = 0;
	while (i < data->num_philos)
	{
		ret = pthread_create(&(data->philos[i].thread), NULL,
				(void *(*)(void *))philo_routine, &(data->philos[i]));
		i++;
	}
	pthread_create(&monitor_thread, NULL, (void *(*)(void *))monitor, data);
	pthread_join(monitor_thread, NULL);
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

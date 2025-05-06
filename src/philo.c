/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:58:39 by tomsato           #+#    #+#             */
/*   Updated: 2025/05/04 12:15:29 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_philo *philo)
{
	if (philo->stat != S_DEAD)
	{
		print_lock(philo, get_status_msg(philo->stat));
		usleep(philo->data->time_to_eat * 1000);
	}
}

void	philo_sleep(t_philo *philo)
{
	if (philo->stat != S_DEAD)
	{
		print_lock(philo, get_status_msg(philo->stat));
		usleep(philo->data->time_to_sleep * 1000);
	}
}

void	*philo_routine(t_philo *philo)
{
	while (philo->stat != S_DEAD)
	{
		if (philo->id %2)
		{
			pthread_mutex_lock(&philo->data->fork_mutexes[philo->right_fork_id]);
			pthread_mutex_lock(&philo->data->fork_mutexes[philo->left_fork_id]);
		}
		else
		{
			pthread_mutex_lock(&philo->data->fork_mutexes[philo->left_fork_id]);
			pthread_mutex_lock(&philo->data->fork_mutexes[philo->right_fork_id]);
		}
		philo_eat(philo);
		pthread_mutex_unlock(&philo->data->fork_mutexes[philo->right_fork_id]);
		pthread_mutex_unlock(&philo->data->fork_mutexes[philo->left_fork_id]);
		philo_sleep(philo);
	}
}

void	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_create(&(data->philos[i].thread), NULL, philo_routine, &(data->philos[i]));
		i++;
	}
	return ;
}

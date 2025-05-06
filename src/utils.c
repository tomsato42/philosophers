/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:27:21 by tomsato           #+#    #+#             */
/*   Updated: 2025/05/04 12:16:42 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + tv.tv_usec / 1000);
}

void	wait_for_start(t_philo *philo)
{
	while (get_time() < philo->data->start_time)
		usleep(100);
}

void	print_lock(t_philo *philo, char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->data->someone_died)
	{
		timestamp = get_time() - philo->data->start_time;
		printf("%ld %zu %s\n", timestamp, philo->id + 1, msg);
	}
	if (philo->stat == S_EAT)
	{
		philo->data->must_eat--;
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}
const char	*get_status_msg(t_status status)
{
	if (status == S_EAT)
		return ("is eating");
	else if (status == S_THINK)
		return ("is thinking");
	else if (status == S_SLEEP)
		return ("is sleeping");
	else if (status == S_DEAD)
		return ("died");
	return (NULL);
}

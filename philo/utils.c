/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:27:21 by tomsato           #+#    #+#             */
/*   Updated: 2025/05/06 19:14:03 by tomsato          ###   ########.fr       */
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
		usleep(10);
}

void	print_lock(t_philo *philo, const char *msg)
{
	long		timestamp;
	static int	flag = 0;

	pthread_mutex_lock(&philo->data->print_mutex);
	timestamp = get_time() - philo->data->start_time;
	if (!get_stop_flag(philo->data))
		printf("%ld %zu %s\n", timestamp, philo->id + 1, msg);
	else if (flag == 0)
	{
		printf("%ld %zu %s\n", timestamp, philo->id + 1, msg);
		flag = 1;
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	get_stop_flag(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->stop_flag);
	i = data->stop;
	pthread_mutex_unlock(&data->stop_flag);
	return (i);
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

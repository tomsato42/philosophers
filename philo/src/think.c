/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   think.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 15:05:30 by tomsato           #+#    #+#             */
/*   Updated: 2025/05/10 15:27:58 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_think(t_philo *philo)
{
	t_data	*data;
	long	remain_time;

	data = philo->data;
	if (!get_stop_flag(data))
		print_lock(philo, get_status_msg(S_THINK));
	pthread_mutex_lock(&philo->meal_mutex);
	remain_time = data->time_to_die - (get_time() - philo->last_meal_time);
	pthread_mutex_unlock(&philo->meal_mutex);
	remain_time -= 10;
	if (remain_time < 0)
		remain_time = 0;
	if (philo->last_meal_time != -1)
		usleep(remain_time * 1000);
}

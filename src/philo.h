/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomsato <tomsato@student.42.jp>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 19:35:50 by tomsato           #+#    #+#             */
/*   Updated: 2025/05/06 18:47:05 by tomsato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

typedef enum e_status
{
	S_EAT,
	S_THINK,
	S_SLEEP,
	S_DEAD
}					t_status;

struct	s_philo;

typedef struct s_data
{
	int				num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat;
	long			start_time;
	int				stop;
	pthread_mutex_t	stop_flag;
	pthread_mutex_t	*fork_mutexes;
	pthread_mutex_t	print_mutex;
	struct s_philo	*philos;
}					t_data;

typedef struct s_philo
{
	size_t			id;
	pthread_t		thread;
	pthread_mutex_t	meal_mutex;
	long			last_meal_time;
	size_t			eat_count;
	int				left_fork_id;
	int				right_fork_id;
	t_data			*data;
}					t_philo;

/*init*/
int					init_data(int ac, char **av, t_data *data);
int					init_philos(t_data *data);

/*finalize*/
void				clear_data(t_data *data);

/*simulation*/
void				start_simulation(t_data *data);

/*monitor*/
void				*monitor(t_data *data);

/*utils*/
long				get_time(void);
void				wait_for_start(t_philo *philo);
void				print_lock(t_philo *philo, const char *msg);
int					get_stop_flag(t_data *data);
const char			*get_status_msg(t_status status);

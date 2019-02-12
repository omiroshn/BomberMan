/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 19:11:48 by omiroshn          #+#    #+#             */
/*   Updated: 2019/02/12 19:17:42 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/timer.h"
#include <stdio.h>

int main() {
	t_timer timer;
	init_timer(&timer);
	while (1)
	{
		get_updated_time(&timer);
		printf("Current time: %f\n", get_delta_time(&timer));
	}
}

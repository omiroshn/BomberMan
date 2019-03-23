/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 17:43:45 by omiroshn          #+#    #+#             */
/*   Updated: 2019/02/14 17:43:46 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>

class Timer
{
private:
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::duration<float, std::ratio<1>> duration;
	std::chrono::duration<float, std::ratio<1>> endTimeDuration;

	bool toDelete;
	bool usedEvent;
	int number;

	void (*event)(std::string time);
public:
	void Tick();
	bool readyForDelete();
	bool usedTheEvent();

	Timer(void (*f)(std::string time), int seconds, bool everlasting);
	virtual ~Timer();
};

#endif

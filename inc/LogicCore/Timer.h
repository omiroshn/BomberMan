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

template <typename... Ts>
class Timer
{
private:
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::duration<float, std::ratio<1>> duration;
	std::chrono::duration<float, std::ratio<1>> endTimeDuration;

	bool toDelete;
	bool usedEvent;
	float number;

	funcType event;
public:

	template <typename funcType>
	Timer(float seconds, bool everlasting, funcType lambda) {
		std::chrono::duration<int> chronoSeconds(seconds);
		startTime = std::chrono::high_resolution_clock::now();
		std::chrono::high_resolution_clock::time_point endTime = startTime + chronoSeconds;
		duration = std::chrono::duration<float, std::ratio<1>>(0.0f);
		endTimeDuration = endTime - startTime;
		toDelete = everlasting == true ? false : true;
		usedEvent = false;
		event = lambda;
		number = seconds;
	}
	
	void Tick() {
		duration = std::chrono::high_resolution_clock::now() - startTime;
		if (duration.count() >= endTimeDuration.count()) {
			// std::cout << "lul" << std::endl;
			event(std::to_string(number));
			usedEvent = true;
		}
	}

	bool readyForDelete() {
		return toDelete;
	}

	bool usedTheEvent() {
		return usedEvent;
	}

	virtual ~Timer() {}
}

#endif

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

#include "TimerBase.h"
#include <chrono>

template <typename funcType>
class Timer : public TimerBase
{

 private:

	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::duration<float, std::milli> duration;
	std::chrono::duration<float, std::milli> endTimeDuration;

	bool toDelete;
	bool usedEvent;

	funcType event;

public:

	Timer(int miliseconds, bool everlasting, funcType lambda) : event(lambda) {
			startTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<int, std::milli>
			milli(miliseconds);
		std::chrono::high_resolution_clock::time_point
			endTime = startTime + milli;
			duration = std::chrono::duration<float, std::milli>(0.0f);
			endTimeDuration = endTime - startTime;
			toDelete = everlasting == true ? false : true;
			usedEvent = false;
	}

	void Tick() {
		duration = std::chrono::high_resolution_clock::now() - startTime;
		if (duration.count() >= endTimeDuration.count()) {
			event();
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
};

#endif

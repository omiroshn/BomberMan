/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 17:47:33 by omiroshn          #+#    #+#             */
/*   Updated: 2019/02/14 17:47:33 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LogicCore/Timer.h"

Timer::Timer(void (*f)(std::string time), int seconds, bool everlasting) {
	std::chrono::duration<int> chronoSeconds(seconds);
	startTime = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point endTime = startTime + chronoSeconds;
	duration = std::chrono::duration<float, std::ratio<1>>(0.0f);
	endTimeDuration = endTime - startTime;
	toDelete = everlasting == true ? false : true;
	usedEvent = false;
	event = f;
	number = seconds;
}

Timer::~Timer() {

}

void Timer::Tick() {
	duration = std::chrono::high_resolution_clock::now() - startTime;
	if (duration.count() >= endTimeDuration.count()) {
		// std::cout << "lul" << std::endl;
		event(std::to_string(number));
		usedEvent = true;
	}
}

bool Timer::readyForDelete() {
	return toDelete;
}

bool Timer::usedTheEvent() {
	return usedEvent;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerManager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 19:32:22 by omiroshn          #+#    #+#             */
/*   Updated: 2019/02/14 19:32:28 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LogicCore/TimerManager.h"

TimerManager *TimerManager::sInstance = nullptr;

TimerManager *TimerManager::Instance() {

	if (sInstance == nullptr)
		sInstance = new TimerManager();

	return sInstance;
}

TimerManager::TimerManager() {
	
}

TimerManager::~TimerManager() {

}

void TimerManager::Release() {

	delete sInstance;
	sInstance = nullptr;
}

void TimerManager::AddTimer(void f(std::string time), int seconds, bool ever) {
	Timer *timer = new Timer(f, seconds, ever);
	v.push_back(timer);
}

void TimerManager::Update() {
	for (std::vector<Timer *>::iterator it = v.begin(); it != v.end(); ++it) {
		if (!v.empty()) {
			(*it)->Tick();
			if ((*it)->readyForDelete() && (*it)->usedTheEvent())
				v.erase(it);
		}
		if (it == v.end())
			break;
	}
	// for (auto timer : v) {
	// 	timer->Tick();
	// 	if (timer->readyForDelete() && timer->usedTheEvent()) {
	// 		v.erase(v[i]);
	// 	}
	// 	i++;
	// 	// std::cout << timer->getStartTime() << std::endl;
	// 	// std::cout << timer->RemainingTime() << std::endl;
	// }
}
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

void TimerManager::Update() {
	for (std::vector<TimerBase *>::iterator it = v.begin(); it != v.end(); ++it) {
		if (!v.empty()) {
			(*it)->Tick();
			if ((*it)->readyForDelete() && (*it)->usedTheEvent())
				v.erase(it);
		}
		if (it == v.end())
			break;
	}
}
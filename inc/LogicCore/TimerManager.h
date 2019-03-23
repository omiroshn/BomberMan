/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerManager.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 19:32:19 by omiroshn          #+#    #+#             */
/*   Updated: 2019/02/14 19:32:19 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include "Timer.h"
#include <vector>
#include <iostream>

class TimerManager
{
private:
	static TimerManager* sInstance;
	std::vector<Timer *> v;
public:
	static TimerManager *Instance();
	static void Release();

	void AddTimer(void (*f)(std::string time), int seconds, bool ever);
	void Update();
private:
	TimerManager();
	~TimerManager();
};

#endif

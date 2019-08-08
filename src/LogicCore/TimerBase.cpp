/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerBase.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omiroshn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 20:43:29 by omiroshn          #+#    #+#             */
/*   Updated: 2019/06/29 20:43:31 by omiroshn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LogicCore/TimerBase.h"

bool TimerBase::readyForDelete() {
    return toDelete;
}

bool TimerBase::usedTheEvent() {
    return usedEvent;
}

TimerBase::~TimerBase() {

}
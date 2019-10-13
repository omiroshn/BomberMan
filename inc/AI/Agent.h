#pragma once
#include <memory>
#include "AI/AIController.h"

/*!
\brief Templated class for all entities which are controlled by some kind of StateMachine.

It's just a simple wrapper for creating a pair of controller-entity.
*/
template <class Pawn, class Controller>
struct Agent
{
    std::shared_ptr<Pawn>	pawn;
    Controller				controller;

    operator Pawn*() {return pawn.get();}

    template<typename... Args>
    Agent(Args ...arg)
    : pawn(std::make_shared<Pawn>(arg...))
    {
    }
};
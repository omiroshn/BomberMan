#pragma once
#include <memory>

template <class Pawn, class Controller>
struct Agent
{
    std::shared_ptr<Pawn>	pawn;
    Controller				controller;

    operator Pawn*() {return pawn.get();}
};
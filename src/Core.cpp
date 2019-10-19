#include "Core.hpp"
#include <algorithm>
#include "AI/AIController.h"

std::vector<Tickable *> Tickable::sTickables;

Tickable::Tickable()
{
	sTickables.push_back(this);
}

Tickable::Tickable(Tickable&& Other)
{
	for (auto& It : sTickables)
	{
		if (It == &Other)
		{
			It = this;
			break;
		}
	}
}

Tickable::~Tickable()
{
	const auto Predicate = [this](Tickable *element) { return element == this; };
	auto It = sTickables.erase(std::remove_if(sTickables.begin(), sTickables.end(), Predicate), sTickables.end());
	sTickables.erase(It, sTickables.end());
}

void Tickable::tickTickables(float DeltaTime)
{
	for_each(sTickables.begin(), sTickables.end(), [DeltaTime](Tickable *element) {element->tick(DeltaTime);});
}

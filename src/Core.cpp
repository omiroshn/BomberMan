#include "Core.hpp"
#include <algorithm>

namespace bm {
	/** Logging stuff */
	void SetLogLevels()
	{
		for (LogCategory It = Default; It < MAX; (*((int*)&It))++)
			SDL_LogSetPriority((int)It, SDL_LOG_PRIORITY_VERBOSE);
	}

	/** Tickable start */
	std::vector<Tickable *> Tickable::sTickables;

	Tickable::Tickable()
	{
		sTickables.push_back(this);
	}

	Tickable::~Tickable()
	{
		const auto Predicate = [this](Tickable *element) { return element == this; };
		auto It = std::remove_if(sTickables.begin(), sTickables.end(), Predicate);
		sTickables.erase(It, sTickables.end());
	}

	void Tickable::tickTickables(float DeltaTime)
	{
		for_each(sTickables.begin(), sTickables.end(), [DeltaTime](Tickable *element) {element->tick(DeltaTime);});
	}
	/** Tickable end */
}

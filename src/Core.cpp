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
	Tickable::Tickable()
	{
		s_Tickables.push_back(this);
	}

	Tickable::~Tickable()
	{
		const auto Predicate = [this](Tickable *element) { return element == this; };
		std::remove_if(s_Tickables.begin(), s_Tickables.end(), Predicate);
	}

	void Tickable::tickTickables()
	{
		for_each(s_Tickables.begin(), s_Tickables.end(), [](Tickable *element) {element->tick();});
	}

	std::vector<Tickable *> Tickable::s_Tickables;
	/** Tickable end */
}

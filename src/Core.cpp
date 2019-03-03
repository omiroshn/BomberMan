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
	std::vector<Tickable *> Tickable::s_Tickables;

	Tickable::Tickable()
	{
		s_Tickables.push_back(this);
	}

	Tickable::~Tickable()
	{
		const auto Predicate = [this](Tickable *element) { return element == this; };
		auto It = std::remove_if(s_Tickables.begin(), s_Tickables.end(), Predicate);
		s_Tickables.erase(It, s_Tickables.end());
	}

	void Tickable::tickTickables(float DeltaTime)
	{
		for_each(s_Tickables.begin(), s_Tickables.end(), [DeltaTime](Tickable *element) {element->tick(DeltaTime);});
	}
	/** Tickable end */
}

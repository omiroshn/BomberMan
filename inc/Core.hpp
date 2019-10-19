#ifndef _CORE_H_
# define _CORE_H_

#include <SDL.h>
#include <vector>

#if (_MSC_VER)
	#undef main
#endif

class Tickable
{
public:
	static void tickTickables(float DeltaTime);

protected:
	Tickable();
	Tickable(Tickable&& Other);
	Tickable(Tickable const&) = delete;
    Tickable& operator=(Tickable const&) = delete;
	virtual ~Tickable();
	
private:
	static std::vector<Tickable *> sTickables;
	virtual void tick(float DeltaTime) = 0;
};

/** Be sure to use this only when everything is really bad */
#define BM_CRITICAL(...)		SDL_LogCritical(SDL_LOG_CATEGORY_CUSTOM, __VA_ARGS__)
#define ARRAY_COUNT(x)			(sizeof(x) / sizeof(x[0]))

# endif

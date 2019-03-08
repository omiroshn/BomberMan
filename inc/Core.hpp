#ifndef _CORE_H_
# define _CORE_H_

#include "SDL.h"
#include <functional>
#include <utility>

namespace bm
{
	/**
	 *	If someone wants a specific category,
	 *	just add it here and use BM_CAT_LOG(YourCategory, ...)
	 */
	enum LogCategory
	{
		Default = SDL_LOG_CATEGORY_CUSTOM,
		Init,
		StateMachine,
		MAX
	};

	void SetLogLevels();

	class Tickable
	{
	public:
		static void tickTickables(float DeltaTime);
	protected:
		Tickable();
		~Tickable();
	private:
		static std::vector<Tickable *> sTickables;
		virtual void tick(float DeltaTime) = 0;
	};
}

/**
 *	A few macro for logging.
 *	*_CAT_* versions use a specific category, so you could
 *	clearly see every log you print.
 */
#ifdef _DEBUG
#define BM_ASSERT(x)			SDL_assert(x)
#define BM_INIT_LOG()			bm::SetLogLevels()
#define BM_LOG(...)				SDL_LogInfo(bm::LogCategory::Default, __VA_ARGS__)
#define BM_WARN(...)			SDL_LogWarn(bm::LogCategory::Default, __VA_ARGS__)
#define BM_CAT_LOG(x, ...)		SDL_LogInfo(bm::LogCategory::##x, #x ": " __VA_ARGS__)
#define BM_CAT_WARN(x, ...)		SDL_LogWarn(bm::LogCategory::##x, #x ": " __VA_ARGS__)
#elif !defined RELEASE
#define BM_ASSERT(x)			SDL_assert(x)
#define BM_INIT_LOG()			bm::SetLogLevels()
#define BM_LOG(...)			
#define BM_WARN(...)			SDL_LogWarn(bm::LogCategory::Default, __VA_ARGS__)
#define BM_CAT_LOG(x, ...)	
#define BM_CAT_WARN(x, ...)		SDL_LogWarn(bm::LogCategory::##x, #x ": " __VA_ARGS__)
#else
#define BM_ASSERT(x)			x
#define BM_INIT_LOG()
#define BM_LOG(...)
#define BM_WARN(...)
#define BM_CAT_LOG(x, ...) 
#define BM_CAT_WARN(x, ...) 
#endif

/** Be sure to use this only when everything is really bad */
#define BM_ASSERT_ALWAYS(x)		SDL_assert(x)
#define BM_CRITICAL(...)		SDL_LogCritical(bm::LogCategory::Default, __VA_ARGS__)
#define BM_CAT_CRITICAL(x, ...) SDL_LogCritical(bm::LogCategory::##x, #x ": " __VA_ARGS__)

# endif

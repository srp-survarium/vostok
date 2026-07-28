#include "pch.h"
#include "anchor.h"

#include <vostok/ai_navigation/api.h>
#include <vostok/ai_navigation/world.h>

namespace vostok
{
	void use_ai_navigation()
	{
		ai::navigation::engine* engine = NULL;
		render::scene_ptr scene = render::scene_ptr();
		render::debug::renderer* renderer = NULL;

		// keeps create_world (-> navigation_world ctor) reachable past /OPT:REF;
		// game's real caller (initialize_ai_navigation) is commented out.
		ai::navigation::world* world = ai::navigation::create_world( *engine, scene, *renderer );
		ai::navigation::destroy_world( world );
	}

	void anchor_ai_navigation( )
	{
		use_ai_navigation();
	}
}

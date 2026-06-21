#include "pch.h"

// Game-module /OPT:REF reachability anchor for the animated_model_instance_cook.cpp
// translation unit. The real caller is game::register_cooks() (still a stub body),
// so without this anchor the linker /OPT:REF-strips every cook symbol (the TU then
// shows up unpaired in objdiff).
//
// Self-guarded like the other anchor_game_* helpers: the heavy ctor never runs
// (s_run stays false), the compiler still emits every reference, so the cook
// carcass survives into the base EXE for matching. Driven from anchor_game().
//
// Constructing the cook keeps its ctor/dtor/vtable; the public virtuals are
// address-taken so /OPT:REF keeps them. The private on_* callbacks are reached
// through the boost::bind sites inside translate_query / on_config_loaded /
// on_subresources_loaded (all in the same TU), so /OPT:REF keeps them too.
//
// Retire once game::register_cooks() is matched and constructs this for real.

#include "animated_model_instance_cook.h"
#include <vostok/resources_query_result.h>

namespace vostok {
	static pcvoid volatile s_animated_model_cook_sink = 0;

	template < typename T >
	static void keep( T m ) { s_animated_model_cook_sink = *( pcvoid const* )&m; }
}

namespace survarium {

void use_game_animated_model_cook( )
{
	static volatile bool s_run = false;
	if( s_run )
	{
		animated_model_instance_cook cook;
	}

	// on_config_loaded / on_subresources_loaded / on_hit_params_loaded are private
	// (AAE); they are reached transitively from the public translate_query bind
	// chain (translate_query -> on_config_loaded -> on_subresources_loaded ->
	// on_hit_params_loaded), so /OPT:REF keeps them without an (inaccessible)
	// explicit keep().
	typedef survarium::animated_model_instance_cook amic;
	vostok::keep( &amic::translate_request_path );
	vostok::keep( &amic::translate_query );
	vostok::keep( &amic::delete_resource );
}

} // namespace survarium

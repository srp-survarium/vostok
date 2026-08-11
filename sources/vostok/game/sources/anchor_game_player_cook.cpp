#include "pch.h"

// Game-module /OPT:REF reachability anchor for the player_cook methods not yet
// retained by its real registration path.
//
// Self-guarded like the other anchor_game_* helpers: the heavy ctors never run
// (s_run stays false), the compiler still emits every reference, so the cook
// carcass survives into the base EXE for matching. Driven from anchor_game().
//
// Constructing the cook keeps its ctor/dtor/vtable; calling the public virtual
// translate_query keeps the resource-query call graph.

#include "player_cook.h"
#include <vostok/resources_query_result.h>

namespace survarium {

void use_game_player_cook( )
{
	static volatile bool s_run = false;
	if( !s_run )
		return;

	static pcvoid volatile	s_sink	= 0;

	resources::query_result_for_cook& parent = *( resources::query_result_for_cook* )NULL;

	player_cook				pc;
	pc.translate_query		( parent );
	pc.delete_resource		( NULL );

	// on_config_loaded is stubbed (cross-module base-ctor block), so the bind chain
	// no longer reaches on_subresources_loaded / on_hit_params_loaded - address-take
	// them directly so /OPT:REF keeps these matched bodies.
	void ( player_cook::* const m_sub )( resources::queries_result&, player_creation_params*, inventory_cooker_data*, player_parameters_cooker_data* )	= &player_cook::on_subresources_loaded;
	void ( player_cook::* const m_hit )( resources::queries_result&, player_creation_params* )	= &player_cook::on_hit_params_loaded;
	s_sink = *( pcvoid const* )&m_sub;
	s_sink = *( pcvoid const* )&m_hit;

}

} // namespace survarium

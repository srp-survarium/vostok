////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// /OPT:REF reachability for the remaining game_world NPC methods without live
// callers. Member pointers escape through a volatile sink without executing them.

#include "pch.h"

#include "game_world.h"

namespace survarium {

void anchor_game_world_npc( game_world& w )
{
	static volatile bool s_run = false;
	if ( !s_run )
		return;

	static pcvoid volatile s_sink = 0;

	void ( game_world::* const m_init_ai )( )					= &game_world::initialize_ai;
	void ( game_world::* const m_init_nav )( )					= &game_world::initialize_ai_navigation;
	void ( game_world::* const m_on_behaviour )( resources::queries_result& )	= &game_world::on_behaviour_created;
	void ( game_world::* const m_finish )( human_npc_ptr&, human_npc::npc_game_attributes& )	= &game_world::finish_npc_creation;
	void ( game_world::* const m_query_dict )( )				= &game_world::query_npc_dictionary;
	void ( game_world::* const m_tick_bm )( bool )				= &game_world::tick_bullet_manager_engine;

	s_sink = *( pcvoid const* )&m_init_ai;
	s_sink = *( pcvoid const* )&m_init_nav;
	s_sink = *( pcvoid const* )&m_on_behaviour;
	s_sink = *( pcvoid const* )&m_finish;
	s_sink = *( pcvoid const* )&m_query_dict;
	s_sink = *( pcvoid const* )&m_tick_bm;

	// Static method: plain function pointer.
	void ( * const m_kill )( human_npc_ptr& )	= &game_world::kill_npc;
	s_sink = ( pcvoid )m_kill;
}

} // namespace survarium

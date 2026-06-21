////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for the game_world NPC management
// methods (game_world_npc.cpp). These are private/static members (plus the free
// delete_weapons) that the real game call graph does not yet reach - game_world's
// own tick/load/unload bodies are still stubs, so the linker would /OPT:REF-strip
// initialize_ai / tick_npcs / update_npc_stats / on_behaviour_created /
// finish_npc_creation / query_npc_dictionary / kill_npc / on_npc_attributes_received
// and the delinker would have nothing to score. The anchor address-takes each
// (member pointers through a volatile sink) so the symbols survive into the base
// EXE without running any code.
//
// Retire once game_world::tick / load / unload / human_npc_cook reach these for
// real (they call tick_npcs+update_npc_stats, initialize_ai+query_npc_dictionary,
// delete_weapons and on_npc_attributes_received respectively in the target).

#include "pch.h"

#include "game_world.h"

namespace survarium {

void delete_weapons( human_npc_ptr& owner );

void anchor_game_world_npc( game_world& w )
{
	static volatile bool s_run = false;
	if ( !s_run )
		return;

	static pcvoid volatile s_sink = 0;

	void ( game_world::* const m_init_ai )( )					= &game_world::initialize_ai;
	void ( game_world::* const m_init_nav )( )					= &game_world::initialize_ai_navigation;
	void ( game_world::* const m_update_stats )( )				= &game_world::update_npc_stats;
	void ( game_world::* const m_on_behaviour )( resources::queries_result& )	= &game_world::on_behaviour_created;
	void ( game_world::* const m_finish )( human_npc_ptr&, human_npc::npc_game_attributes& )	= &game_world::finish_npc_creation;
	void ( game_world::* const m_query_dict )( )				= &game_world::query_npc_dictionary;
	void ( game_world::* const m_tick_npcs )( const u32, const bool )	= &game_world::tick_npcs;
	void ( game_world::* const m_on_attrs )( configs::binary_config_value const&, human_npc_ptr )	= &game_world::on_npc_attributes_received;
	void ( game_world::* const m_tick_bm )( bool )				= &game_world::tick_bullet_manager_engine;

	s_sink = *( pcvoid const* )&m_init_ai;
	s_sink = *( pcvoid const* )&m_init_nav;
	s_sink = *( pcvoid const* )&m_update_stats;
	s_sink = *( pcvoid const* )&m_on_behaviour;
	s_sink = *( pcvoid const* )&m_finish;
	s_sink = *( pcvoid const* )&m_query_dict;
	s_sink = *( pcvoid const* )&m_tick_npcs;
	s_sink = *( pcvoid const* )&m_on_attrs;
	s_sink = *( pcvoid const* )&m_tick_bm;

	// static method + free function: plain function pointers.
	void ( * const m_kill )( human_npc_ptr& )	= &game_world::kill_npc;
	void ( * const m_del_weapons )( human_npc_ptr& )	= &delete_weapons;
	s_sink = ( pcvoid )m_kill;
	s_sink = ( pcvoid )m_del_weapons;
}

} // namespace survarium

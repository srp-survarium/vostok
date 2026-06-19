////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "single_position_animation_controller.h"
// mixing::expression returned by value -> needs the complete type at the definition
#include <vostok/animation/mixing_expression.h>
// resource_ptr<animation_space_graph> dtor instantiates ->destroy() -> needs complete type
#include "animation_space_graph.h"
// NEW/DELETE the m_search_service / m_target_vertex members
#include "game_memory.h"
#include "animations_search_service.h"
#include "animation_space_vertex_id.h"
// m_owner.on_movement_end() needs the complete human_npc
#include "human_npc.h"

namespace survarium {

single_position_animation_controller::single_position_animation_controller(
	animation_space_graph_ptr const&	graph,
	ai::navigation::world const&		ai_navigation_world,
	human_npc&							owner
) :
	m_animation_space_graph( graph ),
	m_search_service( NEW( animations_search_service ) ),
	m_target_vertex( NEW( animation_space_vertex_id ) ),
	m_ai_navigation_world( ai_navigation_world ),
	m_owner( owner ),
	m_navigation_path( g_allocator ),
	m_next_key_point( u32( -1 ) )
{
}

single_position_animation_controller::~single_position_animation_controller( )
{
	DELETE( m_search_service );
	DELETE( m_target_vertex );
}

// claude@NOTE: 2-statement init of *m_target_vertex (identity quaternion built from a
// zero float4x4 -> vertex orientation; owner virtual @vtable[8] -> vertex position).
// PARKED void buildability stub; needs animation_space_vertex_id layout + the human_npc
// virtual confirmed. Structure @0x7667c0 (--view target).
void single_position_animation_controller::initialize( )
{
}

animation::mixing::expression single_position_animation_controller::try_finalize( base_animation_controller& next_controller, mutable_buffer& buffer )
{
	return													animation::mixing::expression( );
}

void single_position_animation_controller::query_new_target_if_needed( )
{
	if ( m_next_key_point > m_navigation_path.size( ) - 1 ) {
		// claude@NOTE: target emits a real `call human_npc::on_movement_end`; our base
		// inlines it to nothing because human_npc::on_movement_end is still an empty STUB
		// (human_npc.cpp - a different unit). Statement is correct; recovered when that
		// unit is matched.
		m_owner.on_movement_end( );
		m_current_parameters.reset( );
		m_target_parameters.reset( );
	}
}

// STATE[STUB]
// claude@NOTE: ~133-statement movement-path lexeme builder (12 named locals,
// A*-search path -> per-segment animation/weight lexemes). PARKED as a buildability
// stub returning the empty expression. Recovering it needs the navigation_path /
// animation_space lexeme helpers (animation_space_graph::*, create_*_lexeme,
// make_pair) wired; structure is in --view target @0x766bc0. NEXT STEP: reconstruct
// statement-by-statement once the per-segment lexeme helpers are confirmed.
animation::mixing::expression single_position_animation_controller::selected_animations( mutable_buffer& buffer )
{
	return													animation::mixing::expression( );
}

void single_position_animation_controller::set_target( animation_controller_parameters const& target )
{
	m_target_parameters												= static_cast_checked< movement_animation_controller_parameters const& >( target );
}

// claude@NOTE: 13-statement debug renderer draw (origin + per-segment cube/arrow over
// m_navigation_path; 1 named local `e`). PARKED void buildability stub; needs the
// math::create_matrix/create_translation/mul4x3 + render::debug::renderer draw_* calls
// reconstructed. Structure @0x776a70 (--view target).
void single_position_animation_controller::debug_draw( render::game::renderer& render, render::scene_ptr const& scene ) const
{
}

} // namespace survarium

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
#include <vostok/ai_navigation/world.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/animation/mixing_multiplication_lexeme.h>
#include <vostok/animation/mixing_weight_lexeme.h>
// debug_draw: render.debug().draw_origin/draw_cube/draw_arrow + matrix/color builders
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/debug_renderer.h>
#include <vostok/math_float4x4.h>

namespace survarium {

static float s_aim_transition_time = 0.3f;

// claude@NOTE: ctor STRUCTURE MATCH (all members in the init list, 0 body statements).
// Residual is LTCG: the target inlines vectora<float3>::vectora( base_allocator* ) (the
// COMDAT does not exist in the target index) into 4 stores (begin/end/eos/allocator) and
// reuses `xor ebx,ebx` for the zero stores; our base out-of-lines `call vectora<...>::
// vectora` and emits `mov dword,0` per field. Same systematic vectora-inline gap hits
// game_world / messaging_client ctors. Not source-steerable from this TU.
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

// claude@NOTE: dtor structure matches (the two DELETEs + the inlined member/base teardown).
// Residual is LTCG/codegen: our base emits extra vtable resets the target omits during the
// inlined member dtors (mov [ebp+30h]/[ebp+4],??_7movement_animation_controller_parameters
// and mov [ebp],??_7base_animation_controller) and allocates `this` to ebp vs the target's
// edi. The vtable writes come from the inlined empty ~movement_animation_controller_parameters
// (fully inlined, no standalone symbol); not source-steerable from this TU.
single_position_animation_controller::~single_position_animation_controller( )
{
	DELETE( m_search_service );
	DELETE( m_target_vertex );
}

void single_position_animation_controller::initialize( )
{
	m_target_vertex->rotation										= math::quaternion( float3( 0.f, 0.f, 0.f ) );
	m_target_vertex->translation									= m_owner.get_position( float3( 0.f, 0.f, 0.f ) );
}

// claude@NOTE: structure is correct (return empty expression). UNPAIRED because the
// target inlines mixing::expression()'s intrusive_ptr default ctor to a single zero-store
// (mov dword ptr [eax],0 / [eax+4],0) while our base out-of-lines the intrusive_ptr ctor
// (extra `call intrusive_ptr<...>`); the structural divergence stops objdiff pairing it.
// Pairs/matches once the intrusive_ptr default ctor inlines (toolchain inline threshold).
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

animation::mixing::expression single_position_animation_controller::selected_animations( mutable_buffer& buffer )
{
	if ( m_current_parameters != m_target_parameters )
		m_current_parameters = m_target_parameters;

	m_target_vertex->rotation = math::create_quaternion_from_direction_vector( m_current_parameters.eyes_direction );

	float3 const& movement_position = m_owner.get_position( float3( 0.f, 0.f, 0.f ) );
	if ( !m_target_vertex->translation.is_similar( movement_position ) )
		m_target_vertex->translation = movement_position;

	u32 const start_node_id = m_ai_navigation_world.get_node_id_at( movement_position );
	u32 const target_node_id = m_ai_navigation_world.get_node_id_at( m_target_vertex->translation );
	if ( start_node_id != u32( -1 ) && target_node_id != u32( -1 ) )
	{
		if (
			m_ai_navigation_world.find_path(
				start_node_id,
				movement_position,
				target_node_id,
				m_target_vertex->translation,
				m_animation_space_graph->agent_radius( ),
				m_navigation_path
			) &&
			!m_navigation_path.empty( )
		)
			m_next_key_point = 1;
		else
			m_next_key_point = u32( -1 );
	}
	else
		m_next_key_point = u32( -1 );

	float3 target_position;
	if ( m_next_key_point == u32( -1 ) )
		target_position = movement_position;
	else {
		for ( u32 i = m_next_key_point; i < m_navigation_path.size( ); ++i ) {
			float const previous_to_current_length = ( m_navigation_path[i] - m_navigation_path[i - 1] ).length( );
			if (
				( ( movement_position - m_navigation_path[i - 1] ) | ( m_navigation_path[i] - m_navigation_path[i - 1] ) / previous_to_current_length ) < previous_to_current_length &&
				( movement_position - m_navigation_path[i] ).length( ) > s_aim_transition_time
			)
				break;

			++m_next_key_point;
		}

		if ( m_next_key_point >= m_navigation_path.size( ) ) {
			LOG_INFO( "path is over!" );
			m_next_key_point = u32( -1 );
			target_position = movement_position;
		}
		else
			target_position = m_navigation_path[m_next_key_point];
	}

	m_target_vertex->translation = target_position;

	animation_space_vertex_id start_vertex_id = {
		math::quaternion( m_owner.get_transform( ).get_angles_xyz( ) ),
		target_position
	};

	vector< u32 > path;
	if ( !m_search_service->search( m_animation_space_graph, &path, start_vertex_id, *m_target_vertex ) || path.empty( ) ) {
		while ( m_next_key_point < m_navigation_path.size( ) - 1 ) {
			++m_next_key_point;
			m_target_vertex->translation = m_navigation_path[m_next_key_point];
			if ( m_search_service->search( m_animation_space_graph, &path, start_vertex_id, *m_target_vertex ) && !path.empty( ) )
				break;
		}

		if ( m_next_key_point >= m_navigation_path.size( ) - 1 ) {
			m_next_key_point = u32( -1 );
			m_target_vertex->translation = movement_position;

			animation::mixing::animation_lexeme lexeme(
				animation::mixing::animation_lexeme_parameters(
					buffer,
					NULL,
					m_animation_space_graph->get_animations( )[0].animation,
					NULL,
					NULL
				).weight_interpolator( animation::linear_interpolator( 0.25f ) )
			);
			return animation::mixing::expression( lexeme );
		}
	}

	m_target_vertex->translation = movement_position;

	float const time_scale = m_animation_space_graph->edge( path[0] ).mixable_pair->first->length / m_animation_space_graph->edge( path[0] ).animation_length;

	animation::mixing::animation_lexeme right_animation(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			NULL,
			m_animation_space_graph->edge( path[0] ).mixable_pair->first->animation,
			NULL,
			NULL
		)
		.weight_interpolator( animation::linear_interpolator( 0.25f ) )
		.time_scale_interpolator( animation::linear_interpolator( 0.25f ) )
		.time_scale( time_scale )
	);

	animation::mixing::weight_lexeme left_weight(
		buffer,
		m_animation_space_graph->edge( path[0] ).first_animation_weight,
		animation::linear_interpolator( 0.25f )
	);

	animation::mixing::animation_lexeme left_animation(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			NULL,
			m_animation_space_graph->edge( path[0] ).mixable_pair->second->animation,
			&right_animation,
			NULL
		)
		.time_scale( time_scale )
	);

	return animation::mixing::expression( left_animation*( 1.f - left_weight ) + right_animation*left_weight );
}

// claude@NOTE: structure correct (the downcast assignment). Residual is an LTCG call-boundary
// effect: the compiler-generated movement_animation_controller_parameters::operator= took a
// custom register convention (this=esi, src=eax) so our base saves/restores `this` across it
// (`push ecx`/`pop ecx`, reading the arg at [esp+8]); the target reads the arg at [esp+4] with
// no temp slot. static_cast vs static_cast_checked makes no difference (both reduce to the same
// cast in MASTER_GOLD - tested). Not source-steerable from this TU.
void single_position_animation_controller::set_target( animation_controller_parameters const& target )
{
	m_target_parameters												= static_cast_checked< movement_animation_controller_parameters const& >( target );
}

void single_position_animation_controller::debug_draw( render::game::renderer& render, render::scene_ptr const& scene ) const
{
	if ( m_target_vertex ) {
		render.debug( ).draw_origin(
			scene,
			math::mul4x3(
				math::create_matrix(
					math::quaternion( ),
					float3( m_current_parameters.position.x, m_current_parameters.position.y + 0.5f, m_current_parameters.position.z )
				),
				math::create_translation(
					float3( m_current_parameters.position.x, m_current_parameters.position.y + 0.5f, m_current_parameters.position.z )
				)
			),
			0.5f,
			true
		);

		for ( float3 const* e = m_navigation_path.begin( ); e != m_navigation_path.end( ); ++e ) {
			render.debug( ).draw_cube( scene, math::create_translation( *e ), float3( 0.1f, 0.1f, 0.1f ), math::color( 0xff0000ff ), true );
			if ( e != m_navigation_path.begin( ) )
				render.debug( ).draw_arrow( scene, *( e - 1 ), *e, math::color( 0xff00ff00 ), true );
		}
	}
}

} // namespace survarium

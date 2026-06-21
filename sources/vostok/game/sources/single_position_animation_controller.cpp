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

// STATE[STUB]
// claude@NOTE: 46-statement movement-path lexeme builder (13 named locals:
// target_position, movement_position, path<u32>, time_scale, right_animation,
// start_vertex_id, left_weight, left_animation, target_node_id, i,
// previous_to_current_length, lexeme). Runs animations_search_service::search over the
// A*-path, then builds per-segment animation/weight/multiplication/addition lexemes
// (mixing::animation_lexeme/weight_lexeme + operator*/operator+) into the result
// expression. PARKED as a buildability stub returning the empty expression: a faithful
// reconstruction needs the mixing-lexeme construction idioms (animation_lexeme_parameters
// ::create_animation_intervals/animation_intervals_count, the operator*/operator+ lexeme
// combinators, expression<T>(T&) converting ctor) confirmed and the search-service API
// wired. Structure @0x776bc0 (--view target). NEXT STEP: reconstruct statement-by-statement
// once the per-segment lexeme combinators are confirmed (start from the simple_animation_
// controller::selected_animations idiom, which uses emitter->emit).
animation::mixing::expression single_position_animation_controller::selected_animations( mutable_buffer& buffer )
{
	return													animation::mixing::expression( );
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

// STATE[STUB]
// claude@NOTE: 6-statement debug renderer draw (1 named local `e`, a `float3 const*`
// iterator over m_navigation_path). Decoded shape (structure @0x776a70, --view target):
//   if ( m_target_vertex ) {                                                    // line 212
//       render.debug( ).draw_origin( scene,                                     // line 215
//           math::mul4x3(
//               math::create_matrix( <quaternion>, float3( p.x, p.y + 0.5f, p.z ) ),
//               math::create_translation( float3( p.x, p.y + 0.5f, p.z ) ) ),
//           0.5f, true );           // p = m_current_parameters.position ([this+8])
//       for ( float3 const* e = m_navigation_path.begin( );                     // line 219
//             e != m_navigation_path.end( ); ++e ) {
//           render.debug( ).draw_cube( scene, math::create_translation( *e ),   // line 221
//               float3( nscl, nscl, nscl ), math::color( 0xff0000ff ), true );  // red cube
//           if ( e != m_navigation_path.begin( ) )                              // line 222
//               render.debug( ).draw_arrow( scene, *( e - 1 ), *e,              // line 223
//                   math::color( 0xff00ff00 ), true );                         // green arrow
//       }
//   }
// PARKED void buildability stub. OPEN: (1) the create_matrix quaternion arg is 3 zeroed
// floats with NO sin/cos and the MASTER_GOLD quaternion() default ctor is a no-op, so it
// is NOT quaternion()/quaternion(float3 angles) - needs the exact spelling resolved via
// --view diff; (2) the color is stored as a raw u32 (0xff0000ff / 0xff00ff00) yet
// math::color is a 4-float struct - confirm the color(u32 argb) form the renderer takes;
// (3) the `nscl` cube-size constant. NEXT STEP: write the decoded body, build, and iterate
// the three OPEN items off --view diff (needs game_renderer.h + debug_renderer.h +
// math_float4x4.h includes).
void single_position_animation_controller::debug_draw( render::game::renderer& render, render::scene_ptr const& scene ) const
{
}

} // namespace survarium

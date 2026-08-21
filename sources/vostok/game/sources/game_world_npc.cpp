////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_world.h"
#include "game_memory.h"	// DELETE (delete_weapons)

#include "game.h"	// get_game().hide_game_stats / ui_world() (update_npc_stats)
#include "npc_stats.h"	// npc_stats::set_stats / draw (update_npc_stats)

#include <vostok/ai/api.h>	// ai::create_world (initialize_ai)
#include <vostok/ai/world.h>	// ai::world::get_weapon_name_by_id (on_npc_attributes_received)
#include <vostok/ai_navigation/api.h>	// ai::navigation::create_world (initialize_ai_navigation)
#include <vostok/configs_binary_config_value.h>	// config value [] / iterator (on_npc_attributes_received)
#include <vostok/physics/world.h>	// physics::world::ray_query (ray_query)
#include <vostok/render/facade/game_renderer.h>	// renderer().debug() (draw_ray / draw_frustum)
#include <vostok/render/facade/debug_renderer.h>	// debug::renderer::draw_arrow / draw_frustum
#include <vostok/resources_queries_result.h>	// queries_result [] (on_behaviour_created)
#include <vostok/resources_query_result.h>	// query_result get_unmanaged_resource (on_behaviour_created)
#include <vostok/ui/world.h>	// ui::world::get_renderer (update_npc_stats)
#include <vostok/collision/common_types.h>	// ray_triangle_result (ray_query_predicate)
#include <vostok/physics/ray_result.h>	// closest_ray_result (get_first_npc... predicate)

namespace survarium {

void game_world::initialize_ai( )
#line 26
{
	m_ai_world = ai::create_world( *this );
}

void game_world::initialize_ai_navigation( )
#line 35
{
	m_ai_navigation_world = ai::navigation::create_world( *this, render_scene(), renderer().debug() );
}

void game_world::get_colliding_objects( math::aabb const& query_aabb, vectora< ai::game_object const* >& results )
#line 42
{
}

void game_world::get_visible_objects(
	math::cuboid const&		cuboid,
	boost::function< void( ai::game_object const& ) > const&	update_callback
)
#line 54
{
}

// TU-local (canonical headers/get_first_npc_in_camera_direction_predicate.h;
// legacy lineage game_unused.cpp::get_first_npc_in_camera_direction - the new
// shape operates on physics::closest_ray_result; consumer is game_world.h's
// inline find_npc_in_camera_direction)
struct get_first_npc_in_camera_direction_predicate : public boost::noncopyable {
	inline			get_first_npc_in_camera_direction_predicate( ) { /* no source */ }

	inline	bool	operator()	( physics::closest_ray_result const& arg_0 ) { /* no source */ return false; }

	inline			~get_first_npc_in_camera_direction_predicate( ) { /* no source */ }

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	human_npc*		first_npc;
}; // struct get_first_npc_in_camera_direction_predicate

STATIC_SIZE_ASSERT(get_first_npc_in_camera_direction_predicate, 0x4);

// TU-local (canonical headers/ray_query_predicate.h; legacy lineage
// game.cpp/game_unused.cpp::ray_query - this compiland carries that family)
struct ray_query_predicate : public boost::noncopyable {
	inline			ray_query_predicate	(
						float&								arg_0,
						collision::object const* const		arg_1,
						collision::object const* const		arg_2,
						const float							arg_3
					) :
		visibility_value( arg_0 ),
		requested_object( arg_1 ),
		object_to_ignore( arg_2 ),
		transparency_threshold( arg_3 ) { /* no source */ }

	inline	bool	predicate			( collision::ray_triangle_result const& arg_0 ) { /* no source */ return false; }

	inline			~ray_query_predicate( ) { /* no source */ }

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	float&								visibility_value;
	/* 0x0004 */	collision::object const* const		requested_object;
	/* 0x0008 */	collision::object const* const		object_to_ignore;
	/* 0x000c */	const float							transparency_threshold;
	/* 0x0010 */	bool								requested_object_was_found;
}; // struct ray_query_predicate

STATIC_SIZE_ASSERT(ray_query_predicate, 0x14);

// claude@NOTE: structure matches (3 stmts). Byte residual is LTCG: the target
// inlines the vectora_allocator<void*> ctor at the game_objects init site (base
// keeps it out-of-line as a call) plus this-in-register; both are call-boundary.
bool game_world::ray_query(
	ai::collision_object const* const		object_to_pick,
	ai::collision_object const* const		object_to_ignore,
	float3 const&							origin,
	float3 const&							direction,
	const float								max_distance,
	const float								transparency_threshold,
	float&									visibility_value
) const
#line 120
{
	vectora< physics::closest_ray_result > game_objects( g_allocator );
	get_physics_world( )->ray_query( origin, direction, max_distance, game_objects, 0xffff, 0x40 );
	return false;
}

void game_world::draw_frustum(
	float				fov_in_radians,
	float				far_plane_distance,
	float				aspect_ratio,
	float3 const&		position,
	float3 const&		direction,
	math::color			color
) const
#line 149
{
	renderer( ).debug( ).draw_frustum(
		m_render_scene,
		fov_in_radians,
		0.f,
		far_plane_distance,
		aspect_ratio,
		position,
		direction,
		float3( 0.f, 1.f, 0.f ),
		color
	);
}

void game_world::draw_ray( float3 const& start_point, float3 const& end_point, bool sees_something ) const
#line 164
{
	renderer().debug().draw_arrow( m_render_scene, start_point, end_point, sees_something ? math::color( 255, 0, 0 ) : math::color( 0, 255, 255 ) );
}

void game_world::get_available_weapons( ai::npc* owner, vectora< ai::weapon* >& list_to_be_filled ) const
#line 169
{
	static_cast_checked< human_npc* >( owner )->get_available_weapons( list_to_be_filled );
}

// claude@NOTE: structure matches (1 stmt). Byte residual is the repo-wide
// logging-not-gold-stubbed wall: target ICF-folds has_passed_filters/append onto
// empty_stub, base emits the real logging bodies. Verbosity/format from the asm.
#line 183
u32 game_world::get_node_by_name( pcstr node_name ) const
#line 175
{
	LOG_ERROR						( "node with name %s wasn't found", node_name );
	return u32( -1 );
}

// claude@NOTE: source is byte-identical to the target (STRUCTURE MATCH, 0x8d==0x8d,
// clean --view diff). Reports "unpaired" only because the delinker emits this free
// function's TARGET symbol DEMANGLED ("survarium::delete_weapons") while the base
// obj has the mangled "?delete_weapons@survarium@@YAX...", so objdiff can't pair the
// two. Systemic delinker gap for game-module free functions (create_wire_visual_source
// / parse_resolution / delete_weapons all hit it), NOT a matching gap - nothing to fix
// in source. NEXT: delinker target-side name recovery for namespace-scoped free funcs.
void delete_weapons( human_npc_ptr& owner )
{
	while ( object_weapon* weapon = owner->pop_weapon( ) )
		DELETE						( weapon );
}

// claude@NOTE: cross-module cap - human_npc::clear_resources is still a stub in
// base ({ret}), so the target's call inlines to nothing here; matches once that
// sibling (human_npc.cpp) is bodied. Residual otherwise is LTCG this-in-register.
void game_world::kill_npc( human_npc_ptr& condemned )
#line 291
{
	delete_weapons					( condemned );
	condemned->clear_resources		( );
}

void game_world::on_behaviour_created( resources::queries_result& data )
#line 327
{
	resources::unmanaged_resource_ptr new_behaviour = data[ 0 ].get_unmanaged_resource( );
	if ( m_selected_npc )
		m_selected_npc->set_behaviour	( new_behaviour );
}

// claude@NOTE: structure (23 stmts) matches. The trailing finish_npc_creation is
// inlined here, so the same human_npc::enable stub cap applies (TRGT_ONLY tail);
// otherwise byte residual is the config operator[] / float-conversion codegen.
void game_world::on_npc_attributes_received( configs::binary_config_value const& attributes_config, human_npc_ptr owner )
#line 337
{
	human_npc::npc_game_attributes		attributes;
	attributes.group_id					= attributes_config[ "group_id" ];
	attributes.class_id					= attributes_config[ "class_id" ];
	attributes.outfit_id				= attributes_config[ "outfit_id" ];
	float3 color						= ( float3 )attributes_config[ "debug_draw_color" ];
	attributes.debug_draw_color			= math::color( ( u32 )color.x, ( u32 )color.y, ( u32 )color.z );
	attributes.initial_velocity			= attributes_config[ "initial_velocity" ];
	attributes.initial_luminosity		= attributes_config[ "initial_luminosity" ];
	attributes.description				= attributes_config[ "description" ];
	attributes.initial_position			= ( float3 )attributes_config[ "initial_position" ];
	attributes.initial_rotation			= ( float3 )attributes_config[ "initial_rotation" ];
	attributes.initial_scale			= ( float3 )attributes_config[ "initial_scale" ];
	attributes.name						= attributes_config[ "name" ];
	attributes.id						= attributes_config[ "id" ];
	configs::binary_config_value const& weapons = attributes_config[ "weapons" ];

	configs::binary_config_value::const_iterator it		= weapons.begin( );
	configs::binary_config_value::const_iterator it_end	= weapons.end( );

	for ( ; it != it_end; ++it )
	{
		configs::binary_config_value const& gun		= *it;
		u32 const type								= gun[ "type" ];
		ai::weapon_types_enum const weapon_type		= ( ai::weapon_types_enum )type;
		u32 const weapon_id							= gun[ "id" ];
		attributes.weapons.push_back				( NEW( object_weapon )( weapon_type, m_ai_world->get_weapon_name_by_id( weapon_type, weapon_id ), weapon_id, weapon_id ) );
	}

	finish_npc_creation					( owner, attributes );
}

// claude@NOTE: cross-module cap - human_npc::enable is still a stub in base
// ({ret}), so the target's enable() call inlines to nothing (a TRGT_ONLY
// statement); structure (3 stmts) is correct, byte-matches once human_npc.cpp's
// enable is bodied. Residual otherwise is LTCG this-in-register.
void game_world::finish_npc_creation( human_npc_ptr& new_npc, human_npc::npc_game_attributes& attributes )
#line 370
{
	new_npc->set_attributes			( attributes );
	new_npc->enable					( );
	m_npcs.push_back				( new_npc );
}

void game_world::query_npc_dictionary( )
#line 377
{
}

// claude@NOTE: cross-module cap - human_npc::tick is still a stub in base ({ret}),
// so the target's per-npc tick() call inlines away (the loop-body resource_ptr
// ref-count statements show as TRGT_ONLY); matches once human_npc.cpp's tick is
// bodied.
void game_world::tick_npcs( const u32 current_frame_id, const bool is_game_paused )
#line 402
{
	for ( human_npc_ptr it_npc = m_npcs.front( ); it_npc; it_npc = m_npcs.get_next_of_object( it_npc ) )
		it_npc->tick				( current_frame_id, is_game_paused );
}

// claude@NOTE: cross-module cap - npc_stats::set_stats is still a stub in base
// ({ret}) so its call inlines to nothing (a TRGT_ONLY statement); also the
// game/ui::world vtable layouts differ in base (ui_world()/get_renderer() land at
// different vtable slots), shifting two call offsets. Structure is correct.
void game_world::update_npc_stats( )
#line 408
{
	if ( m_active_npc_set && m_selected_npc )
	{
		get_game( ).hide_game_stats	= true;
		m_active_npc_stats->set_stats	( m_selected_npc.c_ptr( ) );
		m_active_npc_stats->draw		( get_game( ).ui_world( ).get_renderer( ), render_scene_view( ) );
	}
	else
		get_game( ).hide_game_stats	= false;
}


} // namespace survarium

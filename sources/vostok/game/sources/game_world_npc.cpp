// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "game_world.h"
#include "game_memory.h"	// survarium::g_allocator (delete_weapons)
#include "ai_collision_object.h"

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
#include <vostok/physics/contact_test_predicate.h>

namespace survarium {

struct find_closest_collision_predicate {
	inline find_closest_collision_predicate( ) :
		m_result			( NULL ),
		m_closest_fraction	( 1.0f )
	{
	}

	inline void operator()( physics::closest_ray_result const& result );

public:
	/* 0x0000 */ physics::closest_ray_result const*	m_result;
	/* 0x0004 */ float							m_closest_fraction;
};

STATIC_SIZE_ASSERT(find_closest_collision_predicate, 0x8);

struct test_objects_in_shape_predicate : public physics::contact_test_predicate {
	inline test_objects_in_shape_predicate( ) :
		m_in_shape( false )
	{
	}

	virtual float add_single_result(
		void*,
		physics::primitive_type,
		float4x4 const&,
		float3 const&,
		physics::primitive_type,
		float4x4 const&,
		float3 const&
	) override;

public:
	/* 0x0000 */ /* physics::contact_test_predicate */
	/* 0x0004 */ bool	m_in_shape;
};

STATIC_SIZE_ASSERT(test_objects_in_shape_predicate, 0x8);

void game_world::initialize_ai( )
{
	m_ai_world = ai::create_world( *this );
}

void game_world::initialize_ai_navigation( )
{
	m_ai_navigation_world = ai::navigation::create_world( *this, render_scene(), renderer().debug() );
}

void game_world::get_colliding_objects( math::aabb const& query_aabb, vectora< ai::game_object const* >& results )
{
}

void game_world::get_visible_objects(
	math::cuboid const&		cuboid,
	boost::function< void( ai::game_object const& ) > const&	update_callback
)
{
}

// TU-local (canonical headers/get_first_npc_in_camera_direction_predicate.h;
// legacy lineage game_unused.cpp::get_first_npc_in_camera_direction - the new
// shape operates on physics::closest_ray_result; consumer is game_world.h's
// inline find_npc_in_camera_direction)
struct get_first_npc_in_camera_direction_predicate : private boost::noncopyable {
	inline			get_first_npc_in_camera_direction_predicate( ) { /* no source */ }

	inline	bool	operator()	( physics::closest_ray_result const& arg_0 ) { /* no source */ return false; }

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	human_npc*		first_npc;
}; // struct get_first_npc_in_camera_direction_predicate

STATIC_SIZE_ASSERT(get_first_npc_in_camera_direction_predicate, 0x4);

// TU-local (canonical headers/ray_query_predicate.h; legacy lineage
// game.cpp/game_unused.cpp::ray_query - this compiland carries that family)
struct ray_query_predicate : private boost::noncopyable {
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
{
	renderer().debug().draw_arrow( m_render_scene, start_point, end_point, sees_something ? math::color( 255, 0, 0 ) : math::color( 0, 255, 255 ) );
}

void game_world::get_available_weapons( ai::npc* owner, vectora< ai::weapon* >& list_to_be_filled ) const
{
	static_cast_checked< human_npc* >( owner )->get_available_weapons( list_to_be_filled );
}

u32 game_world::get_node_by_name( pcstr node_name ) const
{
#line 183
	LOG_ERROR						( "node with name %s wasn't found", node_name );
#line 185
	return u32( -1 );
}

void delete_weapons( human_npc_ptr& owner )
{
	while ( object_weapon* weapon = owner->pop_weapon( ) )
		VOSTOK_DELETE_IMPL			( ::survarium::g_allocator, weapon );
}

// Target LTCG eliminates the faithful delete_weapons/clear_resources cleanup from
// its PDB body while base retains the real calls. Do not remove cleanup to chase
// the optimized projection.
void game_world::kill_npc( human_npc_ptr& condemned )
{
	delete_weapons					( condemned );
	condemned->clear_resources		( );
}

void game_world::on_behaviour_created( resources::queries_result& data )
{
	resources::unmanaged_resource_ptr new_behaviour = data[ 0 ].get_unmanaged_resource( );
	if ( m_selected_npc )
		m_selected_npc->set_behaviour	( new_behaviour );
}

void game_world::on_npc_attributes_received( configs::binary_config_value const& attributes_config, human_npc_ptr owner )
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
		attributes.weapons.push_back				( VOSTOK_NEW_IMPL( ::survarium::g_allocator, object_weapon )( weapon_type, m_ai_world->get_weapon_name_by_id( weapon_type, weapon_id ), weapon_id ) );
	}

	finish_npc_creation					( owner, attributes );
}

void game_world::finish_npc_creation( human_npc_ptr& new_npc, human_npc::npc_game_attributes& attributes )
{
	new_npc->set_attributes			( attributes );
	new_npc->enable					( );
	m_npcs.push_back				( new_npc );
}

void game_world::query_npc_dictionary( )
{
}

void game_world::tick_npcs( const u32 current_frame_id, const bool is_game_paused )
{
	for ( human_npc_ptr it_npc = m_npcs.front( ); it_npc; it_npc = m_npcs.get_next_of_object( it_npc ) )
		it_npc->tick				( current_frame_id, is_game_paused );
}

void game_world::update_npc_stats( )
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

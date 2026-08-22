////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/booby_trap_set_core.h>

#include <vostok/game_core/booby_trap_core.h>
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/game_material.h>
#include <vostok/game_core/game_material_manager.h>
#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/game_world_object.h>

#include <vostok/animation/animation_player.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/physics/world.h>
#include <vostok/physics/ray_result.h>
#include <vostok/physics/base_physics_object.h>
#include <vostok/physics/rigid_body_base.h>
#include <vostok/physics/ghost_object.h>

namespace survarium {

booby_trap_set_core::booby_trap_set_core( ) :
	inventory_item		( use_silent ),
	m_traps				( NULL, 0 ),
	m_damage_parameters	( NULL, 0 ),
	m_traps_buffer		( NULL )
{
}
 booby_trap_set_core::~booby_trap_set_core( )
{
	ASSERT( UNKNOWN_EXPRESSION ); pcvoid damage_parms_buffer = m_damage_parameters.begin( );
	m_damage_parameters.clear( );
	VOSTOK_FREE_IMPL( g_allocator, damage_parms_buffer );	// claude@MATCH: target keeps free_helper out-of-line (free_helper_impl); base inlines it.

	m_traps.clear( );
	VOSTOK_FREE_IMPL( g_allocator, m_traps_buffer );
}

void booby_trap_set_core::load( configs::binary_config_value const& config )
{
	m_traps.clear( );
	ASSERT( UNKNOWN_EXPRESSION );

	m_traps_buffer = (booby_trap_core_ptr*)VOSTOK_MALLOC_IMPL( g_allocator, sizeof( booby_trap_core_ptr* ) * amount( ), "traps_buffer" );
	m_traps = buffer_vector< booby_trap_core_ptr >( m_traps_buffer, amount( ) );

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	ASSERT( UNKNOWN_EXPRESSION );

	m_config.max_distance		= (float)config["max_deploy_distance"];
	m_config.max_slope_cos		= math::cos( math::deg2rad( (float)config["max_slope_angle"] ) );

	const float armed_life_time		= (float)config["armed_life_time"];
	m_config.armed_life_time		= math::floor( armed_life_time * 1000.0f );
	const float fired_life_time		= (float)config["fired_life_time"];
	m_config.fired_life_time		= math::floor( fired_life_time * 1000.0f );
	const float disarmed_life_time	= (float)config["disarmed_life_time"];
	m_config.disarmed_life_time		= math::floor( disarmed_life_time * 1000.0f );
	const float defuse_time			= (float)config["defuse_time"];
	m_config.defuse_time			= math::floor( defuse_time * 1000.0f );
	m_config.defuse_by_hit			= (bool)config["defuse_by_hit"];

	m_config.material_can_place_test	= (bool)config["material_can_place_test"];
	m_config.material_can_stick_test	= (bool)config["material_can_stick_test"];

	configs::binary_config_value const&	apply_dmg	= config["damage_parameters"];

	const u32 							count		= apply_dmg.size( );
	m_damage_parameters.clear( );	// sushi@NOTE: Interesting how we didn't free the previous one.
	m_damage_parameters	= buffer_vector< apply_damage >( VOSTOK_MALLOC_IMPL( g_allocator, sizeof( apply_damage ) * count, "damage_parameters" ), count );

	for ( u32 i = 0 ; i < count ; ++i )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		ASSERT( UNKNOWN_EXPRESSION );
		ASSERT( UNKNOWN_EXPRESSION );
		ASSERT( UNKNOWN_EXPRESSION );

		configs::binary_config_value const& conf_entry = apply_dmg[i];
		m_damage_parameters.push_back( apply_damage( ) );
		booby_trap_set_core::apply_damage& ad = m_damage_parameters.back( );

		strings::copy(ad.body_part, 16, (pcstr)conf_entry["body_part"]);
		strings::copy(ad.hit_type,  16, (pcstr)conf_entry["hit_type"]);
		ad.amount			= (float)conf_entry["amount"];
		ad.armor_piercing	= (float)conf_entry["armor_piercing"];

		ASSERT( UNKNOWN_EXPRESSION );
		ASSERT( UNKNOWN_EXPRESSION );
	}

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
}

static bool find_free_trap_predicate( booby_trap_core_ptr trap )
{
	return !trap->is_active( );
}

void booby_trap_set_core::remove_trap( booby_trap_core& trap )
{
	remove_trap_impl( trap );
}

booby_trap_core_ptr* booby_trap_set_core::try_place_trap( )
{
	if ( amount( ) == 0 )
		return m_traps.end( );

	float4x4 place_transform;
	if ( !get_visible_place_transform( place_transform ) )
		return m_traps.end( );

	booby_trap_core_ptr* trap_iter = std::find_if( m_traps.begin( ), m_traps.end( ), find_free_trap_predicate );

	if ( trap_iter == m_traps.end( ) )
		return trap_iter; // sushi@TODO

	insert_trap( **trap_iter, place_transform );
	set_amount( amount( ) - 1 );

	return trap_iter;
}

void booby_trap_set_core::remove_trap_if_active( booby_trap_core_ptr& trap )
{
	ASSERT( UNKNOWN_EXPRESSION );

	if ( trap->is_active( ) )
		remove_trap_impl( *trap );
}

void booby_trap_set_core::remove( )
{
	std::for_each(
		m_traps.begin( ),
		m_traps.end( ),
		boost::bind( &booby_trap_set_core::remove_trap_if_active, this, _1 )
	);
}

// sushi@TODO: Understand what it does exactly
static float4x4 create_place_matrix_for_looking_point(
	float3 const&	hit_point,
	float3 const&	normal,
	float4x4 const& head_transform
)
{
	float4x4 result					= math::create_translation( hit_point );

	float3 const& head_forward		= head_transform.k.xyz( );
	float3 const& right_candidate	= normal ^ head_forward;

	if ( right_candidate.length( ) > math::epsilon_3 ) {
		float3 const& right				= math::normalize( right_candidate );
		float3 const& up				= normal;
		float3 const& forward			= math::normalize( right ^ up );
		result.i.xyz( ) = right;
		result.j.xyz( ) = up;
		result.k.xyz( ) = forward;
	} else {
		float3 const& head_right		= head_transform.i.xyz( );
		float3 const& forward_candidate	= head_right ^ normal;
		ASSERT( UNKNOWN_EXPRESSION_T( forward_candidate.length( ) > math::epsilon_3 ) );
		float3 const& forward			= math::normalize( forward_candidate );
		float3 const& up				= normal;
		float3 const& right				= math::normalize( up ^ forward );
		result.i.xyz( ) = right;
		result.j.xyz( ) = up;
		result.k.xyz( ) = forward;
	}

	return result;
}

bool booby_trap_set_core::get_visible_place_transform( float4x4& result )
{
	physics::world* world = get_inventory( ).holder( ).get_physics_world( );
	ASSERT( UNKNOWN_EXPRESSION_T( world ) );

	base_player* player = get_inventory( ).holder( ).cast_to_base_player( );
	ASSERT( UNKNOWN_EXPRESSION_T( player ) );

	float4x4 const& head_transform = player->get_head_transform( );


	float3		ray_from	= head_transform.c.xyz( );
	float3		ray_dir		= head_transform.k.xyz( );
	const float	ray_length	= config( ).max_distance;

	u16			group		= 1028;
	u16			mask		= 514;
	physics::closest_ray_result ray_result = world->ray_test( ray_from, ray_dir, ray_length, group, mask );

	if ( ray_result.object == NULL )
	{
		result = create_place_matrix_for_looking_point(
			ray_from + ( ray_dir * ray_length ),
			head_transform.j.xyz( ),
			head_transform
		);
		return false;
	}

	float4x4 looking_point_matrix = create_place_matrix_for_looking_point(
		ray_result.hit_point_world,
		ray_result.hit_normal_world,
		head_transform
	);
	const float slope_cos = ray_result.hit_normal_world.y;

	if ( slope_cos >= config( ).max_slope_cos )
	{
		result = looking_point_matrix;
		return false;
	}

	if ( ( ray_result.object->get_collision_group( ) & mask ) == 0 )
	{
		result = looking_point_matrix;
		return false;
	}

	physics::bt_rigid_body_base* body = static_cast_checked< physics::bt_rigid_body_base* >( ray_result.object );
	u16 game_material_id = body->get_triangle_material( ray_result.triangle_index, ray_result.is_shape_index );

	game_material const* material = get_game_material_manager( ).get_material( game_material_id );

	if ( !config( ).material_can_place_test || material->can_place_mine( ) )
	{
		result = looking_point_matrix;
		return false;
	}

	if ( !config( ).material_can_stick_test || material->can_stick_mine( ) )
	{
		result = looking_point_matrix;
		return false;
	}

	float4x4 matrix_a	= looking_point_matrix * math::create_translation( ( -ray_dir ) * 0.005f );
	float3 dir_to_head	= -ray_dir;
	dir_to_head.set_length( 0.5f );
	float4x4 matrix_b	= looking_point_matrix * math::create_translation( dir_to_head );

	ASSERT( UNKNOWN_EXPRESSION );
	booby_trap_core_ptr arbitrary_trap = traps( )[0];

	physics::bt_ghost_object* ghost = arbitrary_trap->get_collision_geometry( 0 )->ghost_object( );

	if ( !world->recover_from_penetrations( ghost->m_shape.c_ptr( ), matrix_a, result, group, mask ) )
	{
		result = looking_point_matrix;
		return false;
	} else {
		return true;
	}
}

void booby_trap_set_core::update_bones_matrices(
	animation::skeleton_ptr const&			user_skeleton,
	float4x4* const							user_matrices,
	const u32								user_matrices_count,
	const u32								current_time_in_ms,
	float4x4&								character_head_transform,
	float4x4&								character_transform,
	animation::animation_player const&		animation_player
)
{
	VOSTOK_UNREFERENCED_PARAMETERS(
		user_skeleton,
		user_matrices,
		user_matrices_count,
		current_time_in_ms,
		character_head_transform,
		character_transform,
		&animation_player
	);
}

static bool trap_is_active( booby_trap_core_ptr const& trap )
{
	return trap->is_active( );
}

void booby_trap_set_core::serialize_game_world_object_header( booby_trap_core const& trap, network_core::udp_match_packet& packet ) const
{
	ASSERT( UNKNOWN_EXPRESSION );

	packet.append( trap_index( trap ) );
}

void booby_trap_set_core::deserialize_game_world_object( network_core::packet_reader& reader )
{
	const u8			trap_index	= reader.r< bool >( );

	ASSERT( UNKNOWN_EXPRESSION );
	booby_trap_core&	trap		= *traps( )[ trap_index ];

	ASSERT( UNKNOWN_EXPRESSION );

	game_world_object&	object		= trap;
	object.deserialize( reader );
}

u8 booby_trap_set_core::trap_index( booby_trap_core const& trap ) const
{
	booby_trap_core_ptr const* trap_iter = std::find( m_traps.begin( ), m_traps.end( ), &trap );
	ASSERT( UNKNOWN_EXPRESSION_T( trap_iter != m_traps.end( ) ) );

	return u8( std::distance( m_traps.begin( ), trap_iter ) );
}

void booby_trap_set_core::remove_trap_impl( booby_trap_core& trap )
{
	ASSERT( UNKNOWN_EXPRESSION );

	inventory_holder& holder = get_inventory( ).holder( );
	holder.remove_game_world_object( trap );
	trap.remove( holder.scheduler( ) );
}

void booby_trap_set_core::insert_trap( booby_trap_core& trap, float4x4 const& transform )
{
	ASSERT( UNKNOWN_EXPRESSION );

	inventory_holder& holder = get_inventory( ).holder( );

	physics::world* world = holder.get_physics_world( );
	ASSERT( UNKNOWN_EXPRESSION_T( world ) );

	trap.insert( world, transform, holder.scheduler( ) );
	holder.insert_game_world_object( trap );
}

} // namespace survarium

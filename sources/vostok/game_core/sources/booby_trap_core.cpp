// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/booby_trap_core.h>

#include <vostok/game_core/base_player.h>
#include <vostok/game_core/booby_trap_set_core.h>
#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/collision_user.h>
#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/hit_receiver.h>

#include <vostok/collision/bone_collision_data.h>
#include <vostok/physics/base_physics_object.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

booby_trap_core::booby_trap_core( ) :
	m_owner			( NULL ),
	m_trap_state	( booby_trap_state_removed ),
	m_state_timer	( 0 )
{
	m_transform.identity( );
}

booby_trap_core::~booby_trap_core( )
{
	VOSTOK_DELETE_IMPL( g_allocator, usable_object::m_collision_geometries );
	VOSTOK_DELETE_IMPL( g_allocator, collision_sensor::m_collision_geometries );
}

inline void booby_trap_core::load_collision( configs::binary_config_value const& config )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	collision_sensor::load( config["collision_sensor"] );
	collision_sensor::m_collision_geometries[0] = VOSTOK_NEW_IMPL( g_allocator, collision_geometry );
	collision_sensor::m_collision_geometries[0]->load( config["collision_sensor"]["collision_geometries"][0] );

	usable_object::load( config["usable_object"] );
	usable_object::m_collision_geometries[0] = VOSTOK_NEW_IMPL( g_allocator, collision_geometry );
	usable_object::m_collision_geometries[0]->load( config["usable_object"]["collision_geometries"][0] );

	if ( config.value_exists("hittable_object") )
	{
		hittable_object::load( config["hittable_object"] );
	}
}

void booby_trap_core::load( configs::binary_config_value const& config )
{
	load_collision( config );
}

void booby_trap_core::load_aabb( configs::binary_config_value const& __formal )
{
}

void booby_trap_core::set_transform( float4x4 const& transform )
{
	m_transform = transform;

	collision_sensor::m_collision_geometries[0]->set_transform( transform );
	usable_object::m_collision_geometries[0]->set_transform( transform );

	ASSERT( UNKNOWN_EXPRESSION );
	if ( m_owner->config( ).defuse_by_hit )
		hittable_object::set_transform( transform );
}

void booby_trap_core::apply_damage( hit_initiator const* const initiator, hit_receiver* const receiver )
{
	buffer_vector< booby_trap_set_core::apply_damage > const& damage_parameters = m_owner->damage_parameters( );
	booby_trap_set_core::apply_damage const*	it	= damage_parameters.begin( );
	booby_trap_set_core::apply_damage const*	end	= damage_parameters.end( );
	for ( ; it != end ; ++it )
	{
		collision::bone_collision_data bcd( "", NULL, it->body_part );
		receiver->hit( initiator, bcd, it->hit_type, it->amount, it->armor_piercing, NULL );
	}
}

void booby_trap_core::on_enter( buffer_vector<physics::base_physics_object *> const& objects )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	physics::base_physics_object* const* it		= objects.begin( );
	physics::base_physics_object* const* end	= objects.end( );
	for ( ; it != end ; ++it )
	{
		hit_initiator const*	initiator	= m_owner->get_inventory( ).holder( ).cast_to_base_player( );
		hit_receiver*			receiver	= it[0]->user_data->cast_to_hit_receiver( );

		ASSERT( UNKNOWN_EXPRESSION_T( initiator ) );
		ASSERT( UNKNOWN_EXPRESSION_T( receiver ) );

		apply_damage( initiator, receiver );
	}

	if ( is_active( ) )
		switch_to_state( booby_trap_state_fired );
}

void booby_trap_core::tick( u32 const time_delta_ms, u32 const current_time_ms )
{
	ASSERT( UNKNOWN_EXPRESSION );

	if ( m_state_timer )
	{
		if ( m_state_timer <= time_delta_ms )
		{
			on_state_timer_finished( );
		}
		else
		{
			m_state_timer -= time_delta_ms;
		}
	}

	if ( m_trap_state == booby_trap_state_armed )
	{
		collision_sensor::tick( time_delta_ms, current_time_ms );
	}
}

void booby_trap_core::insert( physics::world* world, float4x4 const& transform, scheduler& scheduler )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	set_transform( transform );

	register_tick( scheduler );

	ASSERT( UNKNOWN_EXPRESSION );
	collision_sensor::insert( world );
	usable_object::insert( world );
	if ( m_owner->config( ).defuse_by_hit )
		hittable_object::insert( world );

	switch_to_state( booby_trap_state_armed );
}

void booby_trap_core::remove( scheduler& scheduler )
{
	ASSERT( UNKNOWN_EXPRESSION );

	unregister_tick( scheduler );

	switch_to_state( booby_trap_state_removed );
}

bool booby_trap_core::use_initialize( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	if ( !m_usable_object_users.empty( ) )
		return false;

	base_player* user_player = user->owner->cast_to_base_player( );
	ASSERT( UNKNOWN_EXPRESSION_T( user_player ) );

	if ( !can_defuse( user_player ) )
		return false;

	m_usable_object_users.push_back( user );
	user->current_object = this;
	user->start_using_time_ms = user->current_time_ms;

	return true;
}

bool booby_trap_core::use_execute( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	u32 const passed_ms = user->current_time_ms - user->start_using_time_ms;
	float const engineer_factor = user->owner->get_engineer_use_time_factor( );
	u32 config_defuse_time = m_owner->config( ).defuse_time;
	u32 const defuse_time_ms = math::floor( config_defuse_time * engineer_factor );

	user->current_progress = defuse_time_ms ? math::min( 100 * passed_ms / defuse_time_ms, u32(100) ) : 100;

	if ( defuse_time_ms == 0 || passed_ms >= defuse_time_ms )
	{
		defuse_completed( );
		return false;
	}

	return true;
}

bool booby_trap_core::use_finalize( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );


	ASSERT( UNKNOWN_EXPRESSION );

	ASSERT( UNKNOWN_EXPRESSION );
	user->current_object = NULL;
	user->current_progress = u32(-1);
	m_usable_object_users.erase( user );
	return true;
}

pcstr booby_trap_core::use_info( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION );

	base_player* user_player = user->owner->cast_to_base_player( );
	ASSERT( UNKNOWN_EXPRESSION_T( user_player ) );

	return can_defuse( user_player ) ? "st_defuse_trap" : "";
}

bool booby_trap_core::can_defuse( base_player const* user ) const
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	base_player const* owner = m_owner->get_inventory( ).holder( ).cast_to_base_player( );
	ASSERT( UNKNOWN_EXPRESSION_T( owner ) );

	return user == owner || user->team( ) != owner->team( );
}

void booby_trap_core::defuse_completed( )
{
	switch_to_state( booby_trap_state_disarmed );
}

// claude@NOTE: target records ZERO named locals (kept here, structure > %). It loads
// m_trap_state ONCE into a compiler temp at [ebp-8] (below the this-save) and compares
// that temp twice; reaching that single-load-temp-after-this layout needs an assignment
// to a variable, but a named local claims [ebp-4] and swaps the slots vs the target, so
// the faithful zero-local spelling reads the member directly twice (one extra member
// load). Byte residual only; named-local set matches the target (0).
void booby_trap_core::on_state_timer_finished( )
{
	if ( m_trap_state == booby_trap_state_armed || m_trap_state > booby_trap_state_disarmed )
		switch_to_state( booby_trap_state_disarmed );
	else
		m_owner->remove_trap( *this );
}

void booby_trap_core::switch_to_state( booby_trap_state new_state )
{
	if ( m_trap_state == booby_trap_state_armed )
	{
		ASSERT( UNKNOWN_EXPRESSION );

		usable_object::remove( );
		collision_sensor::remove( );
		if ( m_owner->config( ).defuse_by_hit )
			hittable_object::remove( );
	}

	switch ( new_state )
	{
		case booby_trap_state_removed:
		{
			ASSERT( UNKNOWN_EXPRESSION );
			break;
		}
		case booby_trap_state_armed:
		{
			ASSERT( UNKNOWN_EXPRESSION );

			m_state_timer = m_owner->config( ).armed_life_time;
			break;
		}
		case booby_trap_state_fired:
		{
			ASSERT( UNKNOWN_EXPRESSION );
			m_state_timer = m_owner->config( ).fired_life_time;

			if ( !m_state_timer )
			{
				m_owner->remove_trap( *this );
				return;
			}

			m_owner->on_trap_fired( *this );
			break;
		}
		case booby_trap_state_disarmed:
		{
			ASSERT( UNKNOWN_EXPRESSION );
			m_state_timer = m_owner->config( ).disarmed_life_time;

			if ( !m_state_timer )
			{
				m_owner->remove_trap( *this );
				return;
			}

			m_owner->on_trap_disarmed( *this );
			break;
		}
		default: NODEFAULT( ); // claude@MATCH: target jump table has no bounds check -> full contiguous range + NODEFAULT
	}

	m_trap_state = new_state;
}

void booby_trap_core::register_tick( scheduler& scheduler )
{
	scheduler.register_on_frame(
		&m_scheduler_identifier,
		boost::bind( &booby_trap_core::tick, this, _1, _2 ),
		true
	);
}

void booby_trap_core::unregister_tick( scheduler& scheduler )
{
	scheduler.unregister( &m_scheduler_identifier );
}

void booby_trap_core::serialize( network_core::udp_match_packet& packet ) const
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_owner->serialize_game_world_object_header( *this, packet );

	packet.append( m_trap_state );
	packet.append( (math::float3 const&)m_transform.c ); // sushi@TODO: implausible spelling - float4x4 should expose a getter (cf. get_angles_xyz below); find the real translation accessor and respell
	packet.append( m_transform.get_angles_xyz( ) );
}

void booby_trap_core::deserialize( network_core::packet_reader& reader )
{
	booby_trap_state	state		= (booby_trap_state)reader.r< bool >( );
	math::float3		position	= reader.r< math::float3 >( );
	math::float3		angles		= reader.r< math::float3 >( );

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	float4x4			transform	= math::create_rotation( angles ) * math::create_translation( position );
	m_owner->insert_trap( *this, transform );

	if ( state != booby_trap_state_armed )
		switch_to_state( state );
}

booby_trap_set_core const* booby_trap_core::owner( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_owner ) );
	return m_owner;
}

booby_trap_set_core* booby_trap_core::owner( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_owner ) );
	return m_owner;
}

void booby_trap_core::hit(
	hit_initiator const* const	initiator,
	u32	const					bone_index,
	pcstr						damage_type,
	float const					amount,
	float const					armor_piercing,
	bullet*	const				bullet
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( initiator, bone_index, damage_type, amount, armor_piercing, bullet );
	defuse_completed( );
}

void booby_trap_core::hit(
	hit_initiator const* const				initiator,
	collision::bone_collision_data const&	bone_data,
	pcstr									damage_type,
	float const								amount,
	float const								armor_piercing,
	bullet*	const							bullet
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( initiator, bone_data, damage_type, amount, armor_piercing, bullet );
	defuse_completed( );
}

} // namespace survarium

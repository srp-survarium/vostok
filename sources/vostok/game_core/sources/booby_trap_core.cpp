////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

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

// STATE[100%|DONE]
booby_trap_core::booby_trap_core( ) :
	m_owner			( NULL ),
	m_trap_state	( booby_trap_state_removed ),
	m_state_timer	( 0 )
{
	m_transform.identity( );
}

// STATE[14.86%|PARTIAL]: delete_helper out-of-line call in target vs inlined in base
booby_trap_core::~booby_trap_core( )
{
	VOSTOK_DELETE_IMPL( g_allocator, usable_object::m_collision_geometries );
	VOSTOK_DELETE_IMPL( g_allocator, collision_sensor::m_collision_geometries );

	// STRUCTURE DIFF: target 2 / base 2 stmts
	// SIZE +0x2e | 34 | VOSTOK_DELETE_IMPL( g_allocator, usable_object::m_collision_geometries );
	// SIZE +0x2f | 35 | VOSTOK_DELETE_IMPL( g_allocator, collision_sensor::m_collision_geometries );
	// VERDICT: STRUCTURE MATCH - both SIZE rows are delete_helper out-of-line in target vs inlined in base, LTCG inline-vs-call, non-steerable.
}

// STATE[90.68%|PARTIAL]: new_helper::call out-of-line in target vs inlined malloc in base
void booby_trap_core::load( configs::binary_config_value const& config )
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

	// STRUCTURE DIFF: target 13 / base 13 stmts
	// SIZE +0xf | 54 | collision_sensor::m_collision_geometries[0] = VOSTOK_NEW_IMPL( g_allocator, collision_geometry );
	// SIZE -0x2 | 55 | collision_sensor::m_collision_geometries[0]->load( config["collision_sensor"]["collision_geometries"][0] );
	// SIZE +0xf | 58 | usable_object::m_collision_geometries[0] = VOSTOK_NEW_IMPL( g_allocator, collision_geometry );
	// VERDICT: STRUCTURE MATCH - SIZE rows are new_helper<collision_geometry>::call out-of-line in target vs inlined in base, LTCG inline-vs-call, non-steerable.
}

// STATE[100%|DONE]
void booby_trap_core::load_aabb( configs::binary_config_value const& __formal )
{
}

// STATE[97.38%|PARTIAL]: config() out-of-line in target vs inlined in base
void booby_trap_core::set_transform( float4x4 const& transform )
{
	m_transform = transform;

	collision_sensor::m_collision_geometries[0]->set_transform( transform );
	usable_object::m_collision_geometries[0]->set_transform( transform );

	ASSERT( UNKNOWN_EXPRESSION );
	if ( m_owner->config( ).defuse_by_hit )
		hittable_object::set_transform( transform );

	// STRUCTURE DIFF: target 6 / base 6 stmts
	// SIZE -0x2 | 88 | if ( m_owner->config( ).defuse_by_hit )
	// VERDICT: STRUCTURE MATCH - sole SIZE is booby_trap_set_core::config() out-of-line call in target vs inlined member read in base, LTCG inline-vs-call, non-steerable.
}

// STATE[83.06%|PARTIAL]: bone_collision_data ctor out-of-line in target vs inlined in base
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

	// STRUCTURE DIFF: target 6 / base 6 stmts
	// SIZE +0x14 | 105 | collision::bone_collision_data bcd( "", NULL, it->body_part );
	// VERDICT: STRUCTURE MATCH - sole SIZE is bone_collision_data ctor out-of-line in target vs fixed_string members built inline in base, LTCG inline-vs-call, non-steerable.
}

// STATE[91.77%|PARTIAL]: holder() out-of-line in target vs inlined in base
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

	// STRUCTURE DIFF: target 13 / base 13 stmts
	// SIZE +0xd | 126 | hit_initiator const*	initiator	= m_owner->get_inventory( ).holder( ).cast_to_base_player( );
	// VERDICT: STRUCTURE MATCH - sole SIZE is inventory::holder() out-of-line in target vs inlined in base, LTCG inline-vs-call, non-steerable.
}

// STATE[100%|DONE]
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

// STATE[97.44%|PARTIAL]: config() out-of-line in target vs inlined in base
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

	// STRUCTURE DIFF: target 10 / base 10 stmts
	// SIZE -0x2 | 180 | if ( m_owner->config( ).defuse_by_hit )
	// VERDICT: STRUCTURE MATCH - sole SIZE is booby_trap_set_core::config() out-of-line call in target vs inlined member read in base, LTCG inline-vs-call, non-steerable.
}

// STATE[100%|DONE]
void booby_trap_core::remove( scheduler& scheduler )
{
	ASSERT( UNKNOWN_EXPRESSION );

	unregister_tick( scheduler );

	switch_to_state( booby_trap_state_removed );
}

// STATE[99.84%|PARTIAL]: structure exact, residual is LTCG slot/reg noise
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

	// STRUCTURE DIFF: target 12 / base 12 stmts (clean - no diverging rows)
	// VERDICT: STRUCTURE MATCH - exact; the <100% residual is register/stack-slot LTCG noise, non-steerable.
}

// STATE[82.75%|PARTIAL]: config() and math::min out-of-line in target vs inlined in base
bool booby_trap_core::use_execute( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	u32 passed_ms = user->current_time_ms - user->start_using_time_ms;
	float engineer_factor = user->owner->usable_object_user_data( )->booster_engineer_use_time_factor;
	u32 config_defuse_time = m_owner->config( ).defuse_time;
	u32 defuse_time_ms = math::floor( config_defuse_time * engineer_factor );

	user->current_progress = defuse_time_ms ? math::min( 100 * passed_ms / defuse_time_ms, u32(100) ) : 100;

	if ( defuse_time_ms == 0 || passed_ms >= defuse_time_ms )
	{
		defuse_completed( );
		return false;
	}

	return true;

	// STRUCTURE DIFF: target 13 / base 13 stmts
	// SIZE -0x2  | 252 | u32 config_defuse_time = m_owner->config( ).defuse_time;
	// SIZE +0x15 | 255 | user->current_progress = defuse_time_ms ? math::min( 100 * passed_ms / defuse_time_ms, u32(100) ) : 100;
	// VERDICT: STRUCTURE MATCH - SIZE rows are config() and math::min<u32> out-of-line calls in target vs inlined (member read / branchless sbb-min) in base, LTCG inline-vs-call, non-steerable.
}

// STATE[100%|DONE]
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

// STATE[100%|DONE]
pcstr booby_trap_core::use_info( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION );

	base_player* user_player = user->owner->cast_to_base_player( );
	ASSERT( UNKNOWN_EXPRESSION_T( user_player ) );

	return can_defuse( user_player ) ? "st_defuse_trap" : "";
}

// STATE[86.04%|PARTIAL]: holder() out-of-line in target vs inlined in base
bool booby_trap_core::can_defuse( base_player const* user ) const
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	base_player const* owner = m_owner->get_inventory( ).holder( ).cast_to_base_player( );
	ASSERT( UNKNOWN_EXPRESSION_T( owner ) );

	return user == owner || user->team( ) != owner->team( );

	// STRUCTURE DIFF: target 5 / base 5 stmts
	// SIZE +0xd | 305 | base_player const* owner = m_owner->get_inventory( ).holder( ).cast_to_base_player( );
	// VERDICT: STRUCTURE MATCH - sole SIZE is inventory::holder() out-of-line in target vs inlined in base, LTCG inline-vs-call, non-steerable.
}

// STATE[100%|DONE]
void booby_trap_core::defuse_completed( )
{
	switch_to_state( booby_trap_state_disarmed );
}

// STATE[99.67%|DONE]
// claude@MATCH: target merges the state load + both compares into ONE statement (one
// line-table entry), so the original assigned inside the condition. claude@NOTE: the
// target PDB records NO user local here and the temp sits BELOW the this-save (compiler
// temp order) - a switch-dispatch-like shape - but a real switch over the 4/5 dense enum
// values lowers to a jump table (cf. switch_to_state), not the target's je/jle chain, so
// the if shape is kept; residual is only the [ebp-4]/[ebp-8] slot swap.
void booby_trap_core::on_state_timer_finished( )
{
	booby_trap_state state;
	if ( ( state = m_trap_state ) == booby_trap_state_armed || state > booby_trap_state_disarmed )
		switch_to_state( booby_trap_state_disarmed );
	else
		m_owner->remove_trap( *this );

	// STRUCTURE DIFF: target 4 / base 4 stmts (clean - no diverging rows, 0x53 bytes both)
	// VERDICT: STRUCTURE MATCH - byte-identical except the this/state slot swap (allocation noise).
}

// STATE[93.08%|PARTIAL]: config() kept as a call in target (LTCG no-inline of booby_trap_set_core::config); we inline it
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

	// STRUCTURE DIFF: target 27 / base 27 stmts
	// SIZE -0x2 | 361 | if ( m_owner->config( ).defuse_by_hit )
	// SIZE -0x2 | 376 | m_state_timer = m_owner->config( ).armed_life_time;
	// SIZE -0x2 | 382 | m_state_timer = m_owner->config( ).fired_life_time;
	// SIZE -0x2 | 386 | m_owner->remove_trap( *this );
	// SIZE -0x2 | 396 | m_state_timer = m_owner->config( ).disarmed_life_time;
	// VERDICT: STRUCTURE MATCH - config() rows are the out-of-line call in target vs inlined member read in base (LTCG inline-vs-call); remove_trap row is a 2-byte virtual-call register shuffle, non-steerable.
}

// STATE[74.26%|PARTIAL]: register_on_frame LTCG
void booby_trap_core::register_tick( scheduler& scheduler )
{
	scheduler.register_on_frame(
		&m_scheduler_identifier,
		boost::bind( &booby_trap_core::tick, this, _1, _2 ),
		true
	);

	// STRUCTURE DIFF: target 1 / base 1 stmts
	// SIZE +0x28 | 480 | );
	// VERDICT: STRUCTURE MATCH - sole SIZE is scheduler::register_on_frame kept out-of-line in target (promoted bool-in-al convention) vs inlined into register_object + field stores in base, LTCG inline-vs-call, non-steerable from this TU (scheduler.h).
}

// STATE[99.75%|DONE]
void booby_trap_core::unregister_tick( scheduler& scheduler )
{
	scheduler.unregister( &m_scheduler_identifier );

	// STRUCTURE DIFF: target 1 / base 1 stmts (clean - no diverging rows)
	// VERDICT: STRUCTURE MATCH - exact; <100% residual is LTCG register noise, non-steerable.
}

// STATE[77.17%|PARTIAL]: ASSERT + owner serializes the world-object header, then trap
// state (u8), transform position (float3) and Euler angles (float3).
void booby_trap_core::serialize( network_core::udp_match_packet& packet ) const
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_owner->serialize_game_world_object_header( *this, packet );

	packet.append( m_trap_state );
	packet.append( (math::float3 const&)m_transform.c ); // sushi@TODO: implausible spelling - float4x4 should expose a getter (cf. get_angles_xyz below); find the real translation accessor and respell
	packet.append( m_transform.get_angles_xyz( ) );

	// STRUCTURE DIFF: target 5 / base 5 stmts
	// SIZE +0x7 | 409 | packet.append( m_trap_state );
	// SIZE -0x1 | 410 | packet.append( (math::float3 const&)m_transform.c );
	// SIZE +0x9 | 411 | packet.append( m_transform.get_angles_xyz( ) );
	// VERDICT: STRUCTURE MATCH - ASSERT + header-forward byte-match; residual rows are the network_core packet append inline-vs-call wall, non-steerable.
}

// STATE[18.10%|PARTIAL]: read trap state + position + angles, two ASSERT eaters, rebuild the
// transform (rotation * translation), let the owner insert the trap, then drive the trap
// to the read state when it is not the default armed state.
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

	// STRUCTURE DIFF: target 9 / base 9 stmts
	// SIZE +0x19 | 419 | booby_trap_state	state		= (booby_trap_state)reader.r< bool >( );
	// SIZE +0x3f | 420 | math::float3		position	= reader.r< math::float3 >( );
	// SIZE +0x3f | 421 | math::float3		angles		= reader.r< math::float3 >( );
	// VERDICT: STRUCTURE MATCH - both ASSERT eaters + transform build + insert_trap + guarded switch byte-shape match; residual rows are packet_reader::r<T> inlined in base vs called in target - the network_core r<T> wall, non-steerable.
}

// STATE[100%|DONE]
booby_trap_set_core const* booby_trap_core::owner( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_owner ) );
	return m_owner;
}

// STATE[100%|DONE]
booby_trap_set_core* booby_trap_core::owner( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_owner ) );
	return m_owner;
}

// STATE[100%|DONE]
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

// STATE[100%|DONE]
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

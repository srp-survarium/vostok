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

	// STRUCTURE DIFF[target 0x58be80 | base 0x466970]: target 2 / base 2 stmts
	// 0x054 <0x1c> | 0x053 <0x4a> | VOSTOK_DELETE_IMPL( g_allocator, usable_object::m_collision_geometries );   SIZE
	// 0x070 <0x1c> | 0x09d <0x4b> | VOSTOK_DELETE_IMPL( g_allocator, collision_sensor::m_collision_geometries );   SIZE
	// ; aligned 0, size-diffs 2, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - both SIZE diffs are delete_helper<doug_lea_allocator,collision_geometry> emitted out-of-line (pointer in edi) in target vs fully inlined in base, LTCG inline-vs-call, non-steerable. trail: booby_trap_core_dtor.md
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

	// STRUCTURE DIFF[target 0x58bf60 | base 0x466aa0]: target 17 / base 17 stmts
	// 0x05b <0x47> | 0x05b <0x56> | collision_sensor::m_collision_geometries[0] = VOSTOK_NEW_IMPL( g_allocator, collision_geometry );   SIZE
	// 0x0a2 <0x32> | 0x0b1 <0x30> | collision_sensor::m_collision_geometries[0]->load( config["collision_sensor"]["collision_geometries"][0] );   SIZE
	// 0x0ed <0x47> | 0x0fa <0x56> | usable_object::m_collision_geometries[0] = VOSTOK_NEW_IMPL( g_allocator, collision_geometry );   SIZE
	// ; aligned 14, size-diffs 3, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - SIZE diffs are new_helper<collision_geometry>::call<doug_lea_allocator> emitted out-of-line in target vs malloc_impl inlined one layer deeper in base (LTCG inline-vs-call); the load() size diff is downstream slot drift, non-steerable. trail: booby_trap_core_load.md
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

	// STRUCTURE DIFF[target 0x58b3c0 | base 0x465fb0]: target 8 / base 8 stmts
	// 0x06c <0x16> | 0x06c <0x14> | if ( m_owner->config( ).defuse_by_hit )   SIZE
	// ; aligned 7, size-diffs 1, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is booby_trap_set_core::config() emitted out-of-line in target vs inlined member read in base, LTCG inline-vs-call, non-steerable. trail: booby_trap_core_set_transform.md
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

	// STRUCTURE DIFF[target 0x58b9c0 | base 0x466510]: target 8 / base 7 stmts
	// 0x049 <0x13> | 0x048 <0x27> | collision::bone_collision_data bcd( "", NULL, it->body_part );   SIZE
	// <0>         | --          |    EMPTY only target
	// ; aligned 6, size-diffs 1, quantity-diffs 1
	// VERDICT: STRUCTURE MATCH (shape ok) - SIZE/EMPTY are bone_collision_data::bone_collision_data ctor emitted out-of-line in target (name in edx, this in esi) vs the two fixed_string members built inline in base, LTCG inline-vs-call, non-steerable. trail: booby_trap_core_apply_damage.md
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

	// STRUCTURE DIFF[target 0x58bb10 | base 0x4665c0]: target 18 / base 18 stmts
	// 0x049 <0x41> | 0x049 <0x4e> | hit_initiator const*	initiator	= m_owner->get_inventory( ).holder( ).cast_to_base_player( );   SIZE
	// ; aligned 17, size-diffs 1, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is inventory::holder() emitted out-of-line in target vs inlined member read in base, LTCG inline-vs-call, non-steerable. trail: booby_trap_core_on_enter.md
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

	// STRUCTURE DIFF[target 0x58ba60 | base 0x466460]: target 14 / base 14 stmts
	// 0x073 <0x16> | 0x073 <0x14> | if ( m_owner->config( ).defuse_by_hit )   SIZE
	// ; aligned 13, size-diffs 1, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is booby_trap_set_core::config() emitted out-of-line in target vs inlined member read in base, LTCG inline-vs-call, non-steerable. trail: booby_trap_core_insert.md
}

// STATE[100%|DONE]
void booby_trap_core::remove( scheduler& scheduler )
{
	ASSERT( UNKNOWN_EXPRESSION );

	unregister_tick( scheduler );

	switch_to_state( booby_trap_state_removed );

	// CALL SITE INFO
	// <0x59b3a4> -> void <unknown>(scheduler&)
	// <0x59b3b3> -> void <unknown>(booby_trap_state)
	// ******

	// FUNCTION BODY
	// <0x59b389>|0x009|+0x00c:'169'
	// <0>
	// <0x59b395>|0x015|+0x011:'171'
	// <0>
	// <0x59b3a6>|0x026|+0x00f:'173'
	// ******
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

	// STRUCTURE DIFF[target 0x58b5c0 | base 0x466380]: target 17 / base 17 stmts
	// .. same ..
	// ; aligned 17, size-diffs 0, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (exact) - 0 size/quantity diffs; the <100% residual is register/stack-slot LTCG noise, non-steerable. trail: booby_trap_core_use_initialize.md
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

	// STRUCTURE DIFF[target 0x58b460 | base 0x466280]: target 19 / base 19 stmts
	// 0x057 <0x11> | 0x057 <0xf>  | u32 config_defuse_time = m_owner->config( ).defuse_time;   SIZE
	// 0x08a <0x32> | 0x088 <0x47> | user->current_progress = defuse_time_ms ? math::min( 100 * passed_ms / defuse_time_ms, u32(100) ) : 100;   SIZE
	// ; aligned 17, size-diffs 2, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - SIZE diffs are booby_trap_set_core::config() and math::min<u32> emitted out-of-line in target vs inlined (member read / branchless min) in base, LTCG inline-vs-call, non-steerable. trail: booby_trap_core_use_execute.md
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

	// STRUCTURE DIFF[target 0x58b270 | base 0x458ec0]: target 7 / base 7 stmts
	// 0x022 <0x26> | 0x022 <0x33> | base_player const* owner = m_owner->get_inventory( ).holder( ).cast_to_base_player( );   SIZE
	// ; aligned 6, size-diffs 1, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - the return ternary was rewritten to `user == owner || user->team() != owner->team()` to match the target's branch (jne) codegen; sole residual SIZE is inventory::holder() emitted out-of-line in target vs inlined member read in base, LTCG inline-vs-call, non-steerable. trail: booby_trap_core_can_defuse.md
}

// STATE[100%|DONE]
void booby_trap_core::defuse_completed( )
{
	switch_to_state( booby_trap_state_disarmed );

	// FUNCTION BODY
	// <0x59b0f7>|0x007|+0x00f:'260'
	// ******
}

// STATE[99.67%|DONE]: byte-identical stream, only the state/this stack-slot order swapped
void booby_trap_core::on_state_timer_finished( )
{
	booby_trap_state const state = m_trap_state;
	if ( state == booby_trap_state_armed
		|| state > booby_trap_state_disarmed // claude@NOTE: target caches m_trap_state into one temp slot, compared twice
	)
		switch_to_state( booby_trap_state_disarmed );
	else
		m_owner->remove_trap( *this );

	// FUNCTION BODY
	// <0x59b099>|0x009|+0x018:'265'
	// <0>
	// <1>
	// <2>
	// <0x59b0b1>|0x021|+0x00f:'269'		switch_to_state( booby_trap_state_disarmed );
	// <0x59b0c0>|0x030|+0x002:'270'	else
	// <0>
	// <3>
	// <0x59b0c2>|0x032|+0x01d:'275'		m_owner->remove_trap( *this );
	// <0>
	// <5>
	// ******
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

	// FUNCTION BODY
	// <0x59b809>|0x009|+0x00c:'286'
	// <0>
	// <0x59b815>|0x015|+0x00c:'288'
	// <0>
	// <0x59b821>|0x021|+0x00e:'290'
	// <0x59b82f>|0x02f|+0x00e:'291'
	// <0x59b83d>|0x03d|+0x016:'292'
	// <0x59b853>|0x053|+0x00e:'293'
	// <0>
	// <1>
	// <0x59b861>|0x061|+0x010:'296'	switch ( new_state )
	// <0>								{
	// <1>									case booby_trap_state_removed:
	// <2>									{
	// <0x59b871>|0x071|+0x00c:'300'			ASSERT( UNKNOWN_EXPRESSION );
	// <0x59b87d>|0x07d|+0x005:'301'			break;
	// <0>									}
	// <1>									case booby_trap_state_armed:
	// <2>									{
	// <0x59b882>|0x082|+0x00c:'305'			ASSERT( UNKNOWN_EXPRESSION );
	// <0>
	// <0x59b88e>|0x08e|+0x01a:'307'			m_state_timer = m_owner->config( ).armed_life_time;
	// <0x59b8a8>|0x0a8|+0x005:'308'			break;
	// <0>									}
	// <1>									case booby_trap_state_fired:
	// <2>									{
	// <0x59b8ad>|0x0ad|+0x00c:'312'			ASSERT( UNKNOWN_EXPRESSION );
	// <0x59b8b9>|0x0b9|+0x01a:'313'			m_state_timer = m_owner->config( ).fired_life_time;
	// <0>
	// <0x59b8d3>|0x0d3|+0x00c:'315'			if ( !m_state_timer )
	// <0>										{
	// <0x59b8df>|0x0df|+0x01f:'317'				m_owner->remove_trap( *this );
	// <0x59b8fe>|0x0fe|+0x005:'318'				return;
	// <0>										}
	// <1>
	// <0x59b903>|0x103|+0x020:'321'			m_owner->on_trap_fired( *this );
	// <0x59b923>|0x123|+0x002:'322'			break;
	// <0>									}
	// <1>									case booby_trap_state_disarmed:
	// <2>									{
	// <0x59b925>|0x125|+0x00c:'326'			ASSERT( UNKNOWN_EXPRESSION );
	// <0x59b931>|0x131|+0x01a:'327'			m_state_timer = m_owner->config( ).disarmed_life_time;
	// <0>
	// <0x59b94b>|0x14b|+0x00c:'329'			if ( !m_state_timer )
	// <0>										{
	// <0x59b957>|0x157|+0x01d:'331'				m_owner->remove_trap( *this );
	// <0x59b974>|0x174|+0x002:'332'				break;
	// <0>										}
	// <1>
	// <0x59b976>|0x176|+0x020:'335'			m_owner->on_trap_disarmed( *this );
	// <0x59b996>|0x196|+0x002:'336'			break;
	// <0>									}
	// <1>								}
	// <2>
	// <3>
	// <4>
	// <0x59b998>|0x198|+0x00c:'342'
	// ******
}

// STATE[87.85%|PARTIAL]: register_on_frame LTCG
void booby_trap_core::register_tick( scheduler& scheduler )
{
	scheduler.register_on_frame(
		&m_scheduler_identifier,
		boost::bind( &booby_trap_core::tick, this, _1, _2 ),
		true
	);

	// FUNCTION BODY
	// <0x59bc30>|0x010|+0x0d4:'347'
	// ******
}

// STATE[99.75%|DONE]
void booby_trap_core::unregister_tick( scheduler& scheduler )
{
	scheduler.unregister( &m_scheduler_identifier );

	// FUNCTION BODY
	// <0x59b7e9>|0x009|+0x011:'352'
	// ******
}

// STATE[77.17%|PARTIAL]: a leading assert eater, owner serializes the world-object header,
// then trap state (u8), transform position (float3) and Euler angles (float3).
// trail: booby_trap_core_serialize.md
void booby_trap_core::serialize( network_core::udp_match_packet& packet ) const
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_owner->serialize_game_world_object_header( *this, packet );

	packet.append( m_trap_state );
	packet.append( (math::float3 const&)m_transform.c ); // sushi@TODO: implausible spelling - float4x4 should expose a getter (cf. get_angles_xyz below); find the real translation accessor and respell
	packet.append( m_transform.get_angles_xyz( ) );

	// STRUCTURE DIFF: target 5 stmts / base 5 stmts
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|b.line|b.code
	// ---------+--------+--------+----+----+------+------
	// SIZE +0x7|0x58b78d|0x47278d|0x13|0x1a|602   |	packet.append( m_trap_state );
	// SIZE -0x1|0x58b7a0|0x4727a7|0x15|0x14|603   |	packet.append( (math::float3 const&)m_transform.c );
	// SIZE +0x9|0x58b7b5|0x4727bb|0x19|0x22|604   |	packet.append( m_transform.get_angles_xyz( ) );
	// VERDICT: STRUCTURE MATCH (shape ok) - 5/5 after adding the leading ASSERT eater (target stmt#1 is the 0xc triple; the header-forward is stmt#2, a virtual call); SIZE rows are append/get_angles_xyz LTCG inline (base) vs call (target), non-steerable.
}

// STATE[54.95%|PARTIAL]: read trap state + position + angles, two assert eaters, rebuild the
// transform (rotation * translation), let the owner insert the trap, then drive the trap to
// the read state when it is not the default armed state. trail: booby_trap_core_serialize.md
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

	// STRUCTURE DIFF: target 9 stmts / base 9 stmts
	// b.diff    |t.addr  |b.addr  |t.sz|b.sz|b.line|b.code
	// ----------+--------+--------+----+----+------+------
	// SIZE +0xf |0x58b68f|0x4728cf|0xe |0x1d|621   |	booby_trap_state	state		= (booby_trap_state)reader.r< bool >( );
	// SIZE +0x2e|0x58b69d|0x4728ec|0xb |0x39|622   |	math::float3		position	= reader.r< math::float3 >( );
	// SIZE +0x2e|0x58b6a8|0x472925|0xb |0x39|623   |	math::float3		angles		= reader.r< math::float3 >( );
	// VERDICT: STRUCTURE MATCH (shape ok) - 9/9 after adding the two ASSERT eaters (target stmts #4/#5 are 0xc triples at L374/L375); SIZE rows are r<bool>/r<float3> LTCG inline (base) vs call (target), non-steerable.
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

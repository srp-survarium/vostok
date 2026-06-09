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

// STATE[94.90%|DONE]
booby_trap_core::booby_trap_core( ) :
	m_owner			( NULL ),
	m_trap_state	( booby_trap_state_removed ),
	m_state_timer	( 0 )
{
	m_transform.identity( );

	// FUNCTION BODY
	// <0x59be6a>|0x0da|+0x00e:'26'
	// ******
}

// STATE[14.86%|PARTIAL]
booby_trap_core::~booby_trap_core( )
{
	VOSTOK_DELETE_IMPL( g_allocator, usable_object::m_collision_geometries ); // sushi@MATCH: Deleter helper inlined
	VOSTOK_DELETE_IMPL( g_allocator, collision_sensor::m_collision_geometries );

	// FUNCTION BODY
	// <0x59bed4>|0x054|+0x01c:'31'
	// <0x59bef0>|0x070|+0x01c:'32'
	// ******
}

// STATE[84.87%|PARTIAL]
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
	usable_object::m_collision_geometries[0] = VOSTOK_NEW_IMPL( g_allocator, collision_geometry ); // sushi@MATCH: Inlined one layer deeper in base
	usable_object::m_collision_geometries[0]->load( config["usable_object"]["collision_geometries"][0] );

	if ( config.value_exists("hittable_object") )
	{
		hittable_object::load( config["hittable_object"] );
	}

	// FUNCTION BODY
	// <0x59bf69>|0x009|+0x00c:'37'
	// <0x59bf75>|0x015|+0x00c:'38'
	// <0x59bf81>|0x021|+0x00c:'39'
	// <0x59bf8d>|0x02d|+0x00c:'40'
	// <0x59bf99>|0x039|+0x00c:'41'
	// <0>
	// <0x59bfa5>|0x045|+0x016:'43'
	// <0x59bfbb>|0x05b|+0x047:'44'
	// <0x59c002>|0x0a2|+0x032:'45'
	// <0>
	// <0x59c034>|0x0d4|+0x019:'47'
	// <0x59c04d>|0x0ed|+0x047:'48'
	// <0x59c094>|0x134|+0x032:'49'
	// <0>
	// <0x59c0c6>|0x166|+0x014:'51'
	// <0>
	// <0x59c0da>|0x17a|+0x019:'53'
	// <0>
	// ******
}

// STATE[100%|DONE]
void booby_trap_core::load_aabb( configs::binary_config_value const& __formal )
{
}

// STATE[97.38%|DONE]
void booby_trap_core::set_transform( float4x4 const& transform )
{
	m_transform = transform;

	collision_sensor::m_collision_geometries[0]->set_transform( transform );
	usable_object::m_collision_geometries[0]->set_transform( transform );

	ASSERT( UNKNOWN_EXPRESSION );
	if ( m_owner->config( ).defuse_by_hit )
		hittable_object::set_transform( transform );

	// FUNCTION BODY
	// <0x59b3cb>|0x00b|+0x013:'69'
	// <0>
	// <0x59b3de>|0x01e|+0x021:'71'
	// <0x59b3ff>|0x03f|+0x021:'72'
	// <0>
	// <0x59b420>|0x060|+0x00c:'74'
	// <0x59b42c>|0x06c|+0x016:'75'
	// <0x59b442>|0x082|+0x012:'76'
	// ******
}

// STATE[99.85%|DONE]
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

	// FUNCTION BODY
	// <0>
	// <1>
	// <0x59b9d0>|0x010|+0x015:'86'
	// <0x59b9e5>|0x025|+0x008:'87'
	// <0x59b9ed>|0x02d|+0x009:'88'
	// <0x59b9f6>|0x036|+0x013:'89'
	// <0>
	// <0x59ba09>|0x049|+0x013:'91'
	// <0>
	// <8>
	// <0x59ba1c>|0x05c|+0x034:'101'
	// <0>
	// ******
}

// STATE[98.75%|DONE]
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

	// FUNCTION BODY
	// <0x59bb19>|0x009|+0x00c:'107'
	// <0x59bb25>|0x015|+0x00c:'108'
	// <0>
	// <0x59bb31>|0x021|+0x008:'110'
	// <0x59bb39>|0x029|+0x009:'111'
	// <0x59bb42>|0x032|+0x017:'112'
	// <0>
	// <0x59bb59>|0x049|+0x041|[1]:'114'
	// <0x59bb9a>|0x08a|+0x019:'115'
	// <0>
	// <0x59bbb3>|0x0a3|+0x00c:'117'
	// <0x59bbbf>|0x0af|+0x00c:'118'
	// <0>
	// <0x59bbcb>|0x0bb|+0x016:'120'
	// <0x59bbe1>|0x0d1|+0x005:'121'
	// <0>
	// <0x59bbe6>|0x0d6|+0x013:'123'
	// <0x59bbf9>|0x0e9|+0x019:'124'
	// ******
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

// STATE[97.44%|DONE]
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

	// FUNCTION BODY
	// <0x59ba69>|0x009|+0x00c:'151'
	// <0x59ba75>|0x015|+0x00c:'152'
	// <0>
	// <0x59ba81>|0x021|+0x011:'154'
	// <0>
	// <0x59ba92>|0x032|+0x011:'156'
	// <0>
	// <0x59baa3>|0x043|+0x00c:'158'
	// <0x59baaf>|0x04f|+0x012:'159'
	// <0x59bac1>|0x061|+0x012:'160'
	// <0x59bad3>|0x073|+0x016:'161'
	// <0x59bae9>|0x089|+0x012:'162'
	// <0>
	// <0x59bafb>|0x09b|+0x00f:'164'
	// ******
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

// STATE[99.84%|DONE] sushi@TODO: What does this function do
bool booby_trap_core::use_initialize( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	if ( !m_usable_object_users.empty( ) ) // sushi@TODO: What does that mean
		return false;

	base_player* user_player = user->owner->cast_to_base_player( );
	ASSERT( UNKNOWN_EXPRESSION_T( user_player ) );

	if ( !can_defuse( user_player ) )
		return false;

	m_usable_object_users.push_back( user );
	user->current_object = this;
	user->start_using_time_ms = user->current_time_ms;

	return true;

	// FUNCTION BODY
	// <0x59b5c9>|0x009|+0x00c:'178'
	// <0x59b5d5>|0x015|+0x00c:'179'
	// <0>
	// <0x59b5e1>|0x021|+0x013:'181'
	// <0x59b5f4>|0x034|+0x004:'182'
	// <0>
	// <0x59b5f8>|0x038|+0x014:'184'
	// <0x59b60c>|0x04c|+0x00c:'185'
	// <0>
	// <0x59b618>|0x058|+0x019:'187'
	// <0x59b631>|0x071|+0x004:'188'
	// <0>
	// <0x59b635>|0x075|+0x011:'190'	m_usable_object_users.push_back( user );
	// <0x59b646>|0x086|+0x023:'191'
	// <0x59b669>|0x0a9|+0x00c:'192'
	// <0>
	// <0x59b675>|0x0b5|+0x002:'194'
	// ******
}

// STATE[97.35%|DONE]
bool booby_trap_core::use_execute( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	u32 passed_ms = user->current_time_ms - user->start_using_time_ms;
	float engineer_factor = user->owner->usable_object_user_data( )->booster_engineer_use_time_factor;
	u32 config_defuse_time = m_owner->config( ).defuse_time; // sushi@MATCH: config didn't inline in target
	u32 defuse_time_ms = math::floor( config_defuse_time * engineer_factor );

	user->current_progress = defuse_time_ms ? math::min( 100 * passed_ms / defuse_time_ms, u32(100) ) : 100;

	if ( defuse_time_ms == 0 || passed_ms >= defuse_time_ms )
	{
		defuse_completed( );
		return false;
	}

	return true;

	// FUNCTION BODY
	// <0x59b469>|0x009|+0x00c:'199'
	// <0x59b475>|0x015|+0x00c:'200'
	// <0>
	// <0x59b481>|0x021|+0x00c:'202'
	// <0x59b48d>|0x02d|+0x00c:'203'
	// <0>
	// <0x59b499>|0x039|+0x00f:'205'
	// <0x59b4a8>|0x048|+0x00f:'206'
	// <0x59b4b7>|0x057|+0x011:'207'
	// <0x59b4c8>|0x068|+0x022:'208'	u32 defuse_time_ms = math::floor( config_defuse_time * engineer_factor );
	// <0>
	// <0x59b4ea>|0x08a|+0x032:'210'	user->current_progress = defuse_time_ms ? math::min( 100 * passed_ms / defuse_time_ms, 100 ) : 100;
	// <0>
	// <0x59b51c>|0x0bc|+0x00e:'212'	if ( defuse_time_ms == 0 || passed_ms >= defuse_time_ms )
	// <0>
	// <1>
	// <0x59b52a>|0x0ca|+0x017:'215'
	// <0x59b541>|0x0e1|+0x004:'216'
	// <0>
	// <1>
	// <0x59b545>|0x0e5|+0x002:'219'
	// ******
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

// STATE[95.27%|PARTIAL]
bool booby_trap_core::can_defuse( base_player const* user ) const
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	base_player const* owner = m_owner->get_inventory( ).holder( ).cast_to_base_player( );
	ASSERT( UNKNOWN_EXPRESSION_T( owner ) );

	return user != owner // sushi@MATCH: This is written somehow differently
		? user->team( ) != owner->team( )
		: true;

	// FUNCTION BODY
	// <0x59b27a>|0x00a|+0x00c:'249'
	// <0x59b286>|0x016|+0x00c:'250'
	// <0>
	// <0x59b292>|0x022|+0x026:'252'
	// <0x59b2b8>|0x048|+0x00c:'253' 0x56
	// <0>
	// <0x59b2c4>|0x054|+0x03b:'255'
	// ******
}

// STATE[100%|DONE]
void booby_trap_core::defuse_completed( )
{
	switch_to_state( booby_trap_state_disarmed );

	// FUNCTION BODY
	// <0x59b0f7>|0x007|+0x00f:'260'
	// ******
}

// STATE[86.20%|PARTIAL]
void booby_trap_core::on_state_timer_finished( )
{
	if ( m_trap_state == booby_trap_state_armed		// sushi@MATCH: Maybe some inlined function
		|| m_trap_state > booby_trap_state_disarmed // sushi@MATCH: This should never hit
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

// STATE[BLOCKED] switch
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

// STATE[PARTIAL]: owner serializes the world-object header, then trap state (u8), transform
// position (float3) and Euler angles (float3). ASSERTs compiled out. trail: booby_trap_core_serialize.md
void booby_trap_core::serialize( network_core::udp_match_packet& packet ) const
{
	m_owner->serialize_game_world_object_header( *this, packet );

	packet.append( m_trap_state );
	packet.append( (math::float3 const&)m_transform.c );
	packet.append( m_transform.get_angles_xyz( ) );

	// STRUCTURE DIFF[target 0x58b750 | base 0x45f940]: target 5 / base 4 stmts
	//   1: 0x00b <0xc> | 0x00b <0x24> | m_owner->serialize_game_world_object_header( *this, packet );   SIZE
	//   2: 0x017 <0x26> | --          | L359   ONLY target
	//   3: 0x03d <0x13> | 0x02f <0x1a> | packet.append( m_trap_state );   SIZE
	//   4: 0x050 <0x15> | 0x049 <0x14> | packet.append( (math::float3 const&)m_transform.c );   SIZE
	//   5: 0x065 <0x19> | 0x05d <0x22> | packet.append( m_transform.get_angles_xyz( ) );   SIZE
	// ; aligned 0, size-diffs 4, quantity-diffs 1, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - same header-forward + 3 appends; SIZE/quantity are LTCG inline-vs-call of append/get_angles_xyz (target inlines the get_angles_xyz body as its own stmt L359), non-steerable.
}

// STATE[PARTIAL]: read trap state + position + angles, rebuild the transform (rotation *
// translation), let the owner insert the trap, then drive the trap to the read state when it
// is not the default armed state. ASSERTs compiled out. trail: booby_trap_core_serialize.md
void booby_trap_core::deserialize( network_core::packet_reader& reader )
{
	booby_trap_state	state		= (booby_trap_state)reader.r< bool >( );
	math::float3		position	= reader.r< math::float3 >( );
	math::float3		angles		= reader.r< math::float3 >( );

	float4x4			transform	= math::create_rotation( angles ) * math::create_translation( position );
	m_owner->insert_trap( *this, transform );

	if ( state != booby_trap_state_armed )
		switch_to_state( state );

	// STRUCTURE DIFF[target 0x58b680 | base 0x45faa0]: target 9 / base 7 stmts
	//   1: 0x00f <0xe> | 0x00f <0x27> | booby_trap_state	state		= (booby_trap_state)reader.r< bool >( );   SIZE
	//   2: 0x01d <0xb> | 0x036 <0x4a> | math::float3		position	= reader.r< math::float3 >( );   SIZE
	//   3: 0x028 <0xb> | 0x080 <0x4a> | math::float3		angles		= reader.r< math::float3 >( );   SIZE
	//   4: 0x033 <0xc> | --          | L374   ONLY target
	//   5: 0x03f <0xc> | --          | L375   ONLY target
	//   7: 0x07f <0x2a> | 0x0fe <0x2c> | m_owner->insert_trap( *this, transform );   SIZE
	// ; aligned 3, size-diffs 4, quantity-diffs 2, blank-gaps 1
	// VERDICT: STRUCTURE MATCH (shape ok) - same 3 reads + transform build (rotation*translation) + insert_trap + guarded switch_to_state; SIZE/quantity are LTCG inline-vs-call of r<float3>/create_rotation/create_translation (target inlines them into L374/L375 stmts), non-steerable.
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

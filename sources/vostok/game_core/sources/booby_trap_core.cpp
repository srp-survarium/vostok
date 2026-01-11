////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/booby_trap_core.h>

#include <vostok/game_core/base_player.h>
#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/collision_user.h>
#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/hit_receiver.h>

#include <vostok/collision/bone_collision_data.h>
#include <vostok/physics/base_physics_object.h>

namespace survarium {

// STATE[UNCHECKED]
booby_trap_core::booby_trap_core( )
{
	m_transform.identity( );

	// FUNCTION BODY
	// <0x59be6a>|0x0da|+0x00e:'26'
	// ******
}

// STATE[UNCHECKED]
booby_trap_core::~booby_trap_core( )
{
	VOSTOK_DELETE_IMPL( g_allocator, usable_object::m_collision_geometries );
	VOSTOK_DELETE_IMPL( g_allocator, collision_sensor::m_collision_geometries );

	// FUNCTION BODY
	// <0x59bed4>|0x054|+0x01c:'31'
	// <0x59bef0>|0x070|+0x01c:'32'
	// ******
}

// STATE[UNCHECKED]
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

// STATE[UNCHECKED]
void booby_trap_core::load_aabb( configs::binary_config_value const& __formal )
{
	// FUNCTION BODY
	// <0x59b110>|0x000|+0x007:'63'	{
	// <0>
	// <0x59b117>|0x007|      :'65'	}
	// ******
}

// STATE[UNCHECKED]
void booby_trap_core::set_transform( float4x4 const& transform )
{
	m_transform = transform;

	collision_sensor::m_collision_geometries[0]->set_transform( transform );
	usable_object::m_collision_geometries[0]->set_transform( transform );

	ASSERT( UNKNOWN_EXPRESSION );
	if ( m_owner->config( ).defuse_by_hit )
		hittable_object::set_transform( transform );

	// CALL SITE INFO
	// <0x59b3fd> -> void <unknown>(float4x4 const&)
	// <0x59b41e> -> void <unknown>(float4x4 const&)
	// ******

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

// STATE[UNCHECKED]
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

// STATE[UNCHECKED]
void booby_trap_core::on_enter( buffer_vector<physics::base_physics_object *> const& objects )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	physics::base_physics_object* const* it		= objects.begin( );
	physics::base_physics_object* const* end	= objects.end( );
	for ( ; it != end ; ++it )
	{
		hit_initiator const*	initiator	= m_owner->get_inventory( ).holder( ).cast_to_base_player( );
		hit_receiver*			receiver	= (*it)->user_data->cast_to_hit_receiver( );

		ASSERT( UNKNOWN_EXPRESSION_T( initiator ) );
		ASSERT( UNKNOWN_EXPRESSION_T( receiver ) );

		apply_damage( initiator, receiver );
	}

	if ( m_trap_state != booby_trap_state_removed )
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

// STATE[UNCHECKED]
void booby_trap_core::tick( u32 const time_delta_ms, u32 const current_time_ms )
{
	ASSERT( UNKNOWN_EXPRESSION );

	if ( m_state_timer )
	{
		if ( m_state_timer > time_delta_ms )
		{
			m_state_timer -= time_delta_ms;
		}
		else
		{
			on_state_timer_finished( );
		}
	}

	if ( m_trap_state == booby_trap_state_armed )
	{
		collision_sensor::tick( time_delta_ms, current_time_ms );
	}

	// FUNCTION BODY
	// <0x59bd19>|0x009|+0x00c:'129'	ASSERT( UNKNOWN_EXPRESSION );
	// <0>
	// <0x59bd25>|0x015|+0x00c:'131'	if ( m_state_timer > time_delta_ms )
	// <0>
	// <0x59bd31>|0x021|+0x00e:'133'		if ( m_state_timer > time_delta_ms )
	// <0>
	// <0x59bd3f>|0x02f|+0x00e:'135'			m_state_timer -= time_delta_ms;
	// <0>
	// <0x59bd4d>|0x03d|+0x002:'137'		else
	// <0>
	// <0x59bd4f>|0x03f|+0x015:'139'			on_state_timer_finished( );
	// <0>
	// <1>
	// <2>
	// <0x59bd64>|0x054|+0x009:'143'
	// <0>
	// <0x59bd6d>|0x05d|+0x010:'145'
	// <0>
	// ******
}

// STATE[UNCHECKED]
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

// STATE[UNCHECKED]
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

// STATE[UNCHECKED] sushi@TODO: What does this function do
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

// STATE[UNCHECKED]
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

// STATE[UNCHECKED]
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

	// FUNCTION BODY
	// <0x59b559>|0x009|+0x00c:'224'
	// <0x59b565>|0x015|+0x00c:'225'
	// <0>
	// <1>
	// <0x59b571>|0x021|+0x00c:'228'
	// <0>
	// <0x59b57d>|0x02d|+0x00c:'230'
	// <0x59b589>|0x039|+0x00a:'231'
	// <0x59b593>|0x043|+0x00a:'232'
	// <0x59b59d>|0x04d|+0x00f:'233'
	// <0x59b5ac>|0x05c|+0x002:'234'
	// ******
}

// STATE[UNCHECKED]
pcstr booby_trap_core::use_info( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION );

	base_player* user_player = user->owner->cast_to_base_player( );
	ASSERT( UNKNOWN_EXPRESSION_T( user_player ) );

	return can_defuse( user_player ) ? "st_defuse_trap" : "";

	// FUNCTION BODY
	// <0x59b319>|0x009|+0x00c:'239'
	// <0>
	// <0x59b325>|0x015|+0x014:'241'
	// <0x59b339>|0x029|+0x00c:'242'
	// <0>
	// <0x59b345>|0x035|+0x02c:'244'
	// ******
}

// STATE[UNCHECKED]
bool booby_trap_core::can_defuse( base_player const* user ) const
{
	// CALL SITE INFO
	// <0x59b2b3> -> base_player* <unknown>()
	// <0x59b2d7> -> game_team_id <unknown>() const
	// <0x59b2e6> -> game_team_id <unknown>() const
	// ******

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	base_player const* owner = m_owner->get_inventory( ).holder( ).cast_to_base_player( );
	ASSERT( UNKNOWN_EXPRESSION_T( owner ) );

	return user == owner ? true : user->team( ) != owner->team( );

	// FUNCTION BODY
	// <0x59b27a>|0x00a|+0x00c:'249'
	// <0x59b286>|0x016|+0x00c:'250'
	// <0>
	// <0x59b292>|0x022|+0x026:'252'
	// <0x59b2b8>|0x048|+0x00c:'253'
	// <0>
	// <0x59b2c4>|0x054|+0x03b:'255'
	// ******
}

// STATE[UNCHECKED]
void booby_trap_core::defuse_completed( )
{
	switch_to_state( booby_trap_state_disarmed );

	// FUNCTION BODY
	// <0x59b0f7>|0x007|+0x00f:'260'
	// ******
}

// STATE[UNCHECKED]
void booby_trap_core::on_state_timer_finished( )
{
	if ( m_trap_state == booby_trap_state_armed
		|| m_trap_state > booby_trap_state_disarmed // sushi@TODO: This should never hit
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

// STATE[UNCHECKED]
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

// STATE[UNCHECKED]
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

// STATE[UNCHECKED]
void booby_trap_core::unregister_tick( scheduler& scheduler )
{
	scheduler.unregister( &m_scheduler_identifier );

	// FUNCTION BODY
	// <0x59b7e9>|0x009|+0x011:'352'
	// ******
}

// STATE[BLOCKED]
void booby_trap_core::serialize( network_core::udp_match_packet& packet ) const
{
	// FUNCTION BODY
	// <0>
	// <0x59b75b>|0x00b|+0x00c:'358'
	// <0x59b767>|0x017|+0x026:'359'
	// <0>
	// <0x59b78d>|0x03d|+0x013:'361'
	// <0x59b7a0>|0x050|+0x015:'362'
	// <0x59b7b5>|0x065|+0x019:'363'
	// ******
}

// STATE[BLOCKED]
void booby_trap_core::deserialize( network_core::packet_reader& reader )
{
	// LOCALS
	// booby_trap_state 			state
	// float4x4 					transform
	// float3 						angles
	// float3 						position
	// ******

	// CALL SITE INFO
	// <0x59b727> -> void <unknown>(booby_trap_core&, float4x4 const&)
	// <0x59b744> -> void <unknown>(booby_trap_state)
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <0x59b68f>|0x00f|+0x00e:'370'
	// <0x59b69d>|0x01d|+0x00b:'371'
	// <0x59b6a8>|0x028|+0x00b:'372'
	// <0>
	// <0x59b6b3>|0x033|+0x00c:'374'
	// <0x59b6bf>|0x03f|+0x00c:'375'
	// <0>
	// <0x59b6cb>|0x04b|+0x034:'377'
	// <0x59b6ff>|0x07f|+0x02a:'378'
	// <0>
	// <0x59b729>|0x0a9|+0x006:'380'
	// <0x59b72f>|0x0af|+0x017:'381'
	// ******
}

// STATE[UNCHECKED]
booby_trap_set_core const* booby_trap_core::owner( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_owner ) );
	return m_owner;

	// FUNCTION BODY
	// <0x59b249>|0x009|+0x00c:'386'
	// <0x59b255>|0x015|+0x009:'387'
	// ******
}

// STATE[UNCHECKED]
booby_trap_set_core* booby_trap_core::owner( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_owner ) );
	return m_owner;

	// FUNCTION BODY
	// <0x59b219>|0x009|+0x00c:'392'
	// <0x59b225>|0x015|+0x009:'393'
	// ******
}

// STATE[BLOCKED]
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

	// CALL SITE INFO: sushi@TODO: Maybe I can get function name here somehow!
	// <0x59b203> -> void <unknown>()
	// ******

	// FUNCTION BODY
	// <0>
	// <6>
	// <0x59b1a9>|0x009|+0x045:'412'
	// <0>
	// <0x59b1ee>|0x04e|+0x017:'414'
	// ******
}

// STATE[BLOCKED]
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
	// void (__thiscall *)(vostok::vfs::base_node<1> **)


	// FUNCTION BODY
	// <0>
	// <6>
	// <0x59b12b>|0x00b|+0x053:'433'
	// <0>
	// <0x59b17e>|0x05e|+0x017:'435'
	// ******
}

} // namespace survarium

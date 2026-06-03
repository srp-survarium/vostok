////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/medkit.h>

#include <vostok/game_core/base_player.h>
#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/player_stamina.h>

namespace survarium {

// STATE[100%|DONE]
medkit::medkit( ) :
	inventory_item				( use_silent ),
	m_influences				( NULL ),
	m_influences_count			( 0 ),
	m_affects					( NULL ),
	m_affects_count				( 0 ),
	m_damage_protect			( NULL ),
	m_damage_protect_count		( 0 ),
	m_config_activity_time_ms	( 1 ),
	m_config_delay_ms			( 0 ),
	m_active					( false ),
	m_add_stamina_regen			( 0.0f )
{
}

// STATE[UNCHECKED]
 medkit::~medkit( )
{
	VOSTOK_DELETE_IMPL( g_allocator, m_influences );
	VOSTOK_DELETE_IMPL( g_allocator, m_affects );
	VOSTOK_DELETE_IMPL( g_allocator, m_damage_protect );

	// FUNCTION BODY[0x760180]: 3
	// <0x760192>|0x012|+0x040:'32'
	// <0x7601d2>|0x052|+0x040:'33'
	// <0x760212>|0x092|+0x040:'34'
	// ******
}

// STATE[91.02%|DONE]: sushi@TODO: This function is broken. Allocations are done incorrectly. Match seems to be correct though.
void medkit::load( configs::binary_config_value config )
{
	float activity_time_sec = (float)config["activity_time_sec"];
	activity_time_sec = math::max( activity_time_sec, math::epsilon_3 );
	m_config_activity_time_ms = math::floor( 1000.0f * activity_time_sec );

	float activation_delay_sec = (float)config["activation_delay_sec"];
	m_config_delay_ms = math::floor( 1000.0f * activation_delay_sec );

	configs::binary_config_value influences = config["influences"];

	m_influences_count	= (u8)influences.size( );
	m_influences		= (item_influence*)VOSTOK_MALLOC_IMPL( g_allocator, 20 * m_influences_count, "medkit_influences" );

	m_add_stamina_regen	= (float)config["add_stamina_regen"];

	for ( u32 i = 0 ; i < m_influences_count ; ++i )
	{	// sushi@TODO: Is this UB? I don't see where memory is allocated
		item_influence& infl = m_influences[i]; // sushi@TODO: Do not hardcode 0x10
		strings::copy( infl.body_part_name, 0x10, influences[i]["body_part"] );
		infl.health_amount = (float)influences[i]["amount"];
		infl.health_amount /= activity_time_sec;
	}

	configs::binary_config_value remove_affects = config["remove_affects"];

	m_affects_count = (u8)remove_affects.size( );
	m_affects		= (affect*)VOSTOK_MALLOC_IMPL( g_allocator, sizeof( affect ) * m_affects_count, "medkit_affects" );

	for ( u32 i = 0 ; i < m_affects_count ; ++i )
	{
		affect& affct = m_affects[i];
		strings::copy( affct.body_part_name, 0x10, remove_affects[i]["body_part"] );
		affct.type = (hit_affects_type_enum)(u32)remove_affects[i]["affect"];
	}

	configs::binary_config_value damage_protect = config["damage_protection"];

	m_damage_protect_count  = (u8)damage_protect.size( );
	m_damage_protect		= (damage_protection*)VOSTOK_MALLOC_IMPL( g_allocator, sizeof( damage_protection ) * m_damage_protect_count, "medkit_damage_protect" );

	for ( u32 i = 0 ; i < m_damage_protect_count ; ++i )
	{
		damage_protection& dmgp = m_damage_protect[i];
		new( &dmgp ) damage_protector( );
		dmgp.protector.reduce_damage_functor = boost::bind( &medkit::reduce_damage, this, _1, _2, _3, _4 );
		strings::copy( dmgp.body_part_name, 0x10, damage_protect[i]["body_part"] );
		strings::copy( dmgp.hit_type, 0x10, damage_protect[i]["hit_type"] );
		dmgp.hit_coeff = (float)damage_protect[i]["hit_coeff"];
		dmgp.threshold = (float)damage_protect[i]["threshold"];
	}



	// FUNCTION BODY[0x75fc20]: 51
	// <0x75fc30>|0x010|+0x017:'39'
	// <0x75fc47>|0x027|+0x017:'40'
	// <0x75fc5e>|0x03e|+0x027:'41'
	// <0>
	// <0x75fc85>|0x065|+0x017:'43'
	// <0x75fc9c>|0x07c|+0x027:'44'
	// <0>
	// <0x75fcc3>|0x0a3|+0x030:'46'		configs::binary_config_value influences = (float)config["influences"];
	// <0>
	// <0x75fcf3>|0x0d3|+0x014:'48'		m_influences_count = influences.size( );
	// <0x75fd07>|0x0e7|+0x02f:'49'		m_influences		= (item_influence*)VOST
	// <0>
	// <0x75fd36>|0x116|+0x020:'51'		m_add_stamina_regen	= (float)config["add_stamina_regen"];
	// <0>
	// <0x75fd56>|0x136|+0x028|[1]:'53'	for ( u32 i = 0 ; i < m_influences_count ; ++i )
	// <0>
	// <0x75fd7e>|0x15e|+0x015:'55'			item_influence& infl = m_influences[i];
	// <0x75fd93>|0x173|+0x035:'56'
	// <0x75fdc8>|0x1a8|+0x024:'57'
	// <0x75fdec>|0x1cc|+0x015:'58'
	// <0x75fe01>|0x1e1|+0x005:'59'		}
	// <0>
	// <1>
	// <0x75fe06>|0x1e6|+0x030:'62'
	// <0>
	// <0x75fe36>|0x216|+0x014:'64'
	// <0x75fe4a>|0x22a|+0x02f:'65'	m_affects		= (affect*)VOSTOK_M
	// <0>
	// <0x75fe79>|0x259|+0x024|[2]:'67'
	// <0>
	// <0x75fe9d>|0x27d|+0x015:'69'
	// <0x75feb2>|0x292|+0x035:'70'
	// <0x75fee7>|0x2c7|+0x022:'71'
	// <0x75ff09>|0x2e9|+0x005:'72'
	// <0>
	// <1>
	// <0x75ff0e>|0x2ee|+0x030:'75'
	// <0>
	// <0x75ff3e>|0x31e|+0x014:'77'
	// <0x75ff52>|0x332|+0x02f:'78'
	// <0>
	// <0x75ff81>|0x361|+0x028|[3]:'80'	for ( u32 i = 0 ; i < m_damage_protect_count ; ++i )
	// <0>
	// <0x75ffa9>|0x389|+0x015:'82'			damage_protection& dmgp = m_damage_protect[i];
	// <0x75ffbe>|0x39e|+0x031:'83'			new( &dmgp ) damage_protector( );
	// <0x75ffef>|0x3cf|+0x0c9:'84'			dmgp.protector.reduce_damage_functor = boost:
	// <0x7600b8>|0x498|+0x038:'85'
	// <0x7600f0>|0x4d0|+0x038:'86'
	// <0x760128>|0x508|+0x024:'87'
	// <0x76014c>|0x52c|+0x029:'88'
	// <0>
	// ******
}

// STATE[UNCHECKED]
void medkit::set_active( bool bactive )
{
	m_active = bactive;

	if ( m_active )
	{
		m_activity_time_ms	= m_config_activity_time_ms;
		m_delay_ms			= m_config_delay_ms;

		m_inventory->holder( ).scheduler( ).register_for_update(
			&m_scheduler_identifier,
			boost::bind( &medkit::active_tick, this, _1 ),
			true,
			300,
			1,
			0	// sushi@TODO: Is this a delay before medkit is working?
		);
	}
	else
	{	// sushi@TODO: When is this called?
		player_stamina& stamina = m_inventory->holder( ).cast_to_base_player( )->stamina( );
		stamina.set_regeneration_speed( stamina.get_regeneration_speed( ) - m_add_stamina_regen );

		m_inventory->holder( ).scheduler( ).unregister( &m_scheduler_identifier ); // sushi@TODO: Isn't it deleted automatically?
	}

	for ( u32 i = 0 ; i < m_damage_protect_count ; ++i )
	{
		damage_protection& dmgp = m_damage_protect[i];
		if ( m_active )
			m_inventory->holder( ).damage_model( )->register_body_part_damage_protector(
				dmgp.body_part_name,
				&dmgp.protector
			);
		else
			m_inventory->holder( ).damage_model( )->unregister_body_part_damage_protector(
				dmgp.body_part_name,
				&dmgp.protector
			);
	}


	// FUNCTION BODY[0x75f730]: 27
	// <0x75f73d>|0x00d|+0x00c:'94'	m_active = bactive;
	// <0>
	// <0x75f749>|0x019|+0x012:'96'	if ( m_active )
	// <0>
	// <0x75f75b>|0x02b|+0x012:'98'
	// <0x75f76d>|0x03d|+0x012:'99'
	// <0>
	// <0x75f77f>|0x04f|+0x0a3:'101'
	// <0>
	// <0x75f822>|0x0f2|+0x002:'103'	else
	// <0>								{
	// <0x75f824>|0x0f4|+0x031|[1]:'105'	player_stamina& stamina = m_inventory->holder( )
	// <0x75f855>|0x125|+0x02b:'106'		stamina.set_regeneration_spe
	// <0>
	// <0x75f880>|0x150|+0x023:'108'
	// <0>
	// <1>
	// <0x75f8a3>|0x173|+0x025|[1]:'111'	for ( u32 i = 0 ; i < m_damage_protect_count ; ++i )
	// <0>
	// <0x75f8c8>|0x198|+0x012:'113'			damage_protection& dmgp = m_damage_protect[i];
	// <0x75f8da>|0x1aa|+0x00e:'114'			if ( m_active )
	// <0x75f8e8>|0x1b8|+0x035:'115'
	// <0x75f91d>|0x1ed|+0x002:'116'			else
	// <0x75f91f>|0x1ef|+0x043:'117'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[96.17%|PARTIAL]
void medkit::action( bool key_down )
{
	if ( !key_down )
		return;

	if ( m_active )
		return;

	if ( amount( ) > 0 )
	{
		set_active( true );
		set_amount( amount( ) - 1 );	// sushi@MATCH: LTCG for set_amount
	}


	// FUNCTION BODY[0x75f970]: 11
	// <0x75f977>|0x007|+0x008:'125'
	// <0x75f97f>|0x00f|+0x002:'126'
	// <0>
	// <0x75f981>|0x011|+0x00e:'128'
	// <0x75f98f>|0x01f|+0x002:'129'
	// <0>
	// <0x75f991>|0x021|+0x00f:'131'
	// <0>
	// <0x75f9a0>|0x030|+0x00a:'133'
	// <0x75f9aa>|0x03a|+0x016:'134'
	// <0>
	// ******
}

// STATE[UNCHECKED]
void medkit::active_tick( const u32 frame_time_ms )
{
	ASSERT( UNKNOWN_EXPRESSION );

	u32 time_left_ms = frame_time_ms;	// time_delta

	if ( m_delay_ms )
	{
		const u32 delay_time = math::min( m_delay_ms, time_left_ms );
		m_delay_ms -= delay_time;

		if ( m_delay_ms )
			return;

		time_left_ms -= delay_time;
		if ( !time_left_ms )
			return;
	}

	if ( m_activity_time_ms == m_config_activity_time_ms )	// if ( starting )
	{
		remove_affects( );

		player_stamina& stamina = m_inventory->holder( ).cast_to_base_player( )->stamina( );
		stamina.set_regeneration_speed( stamina.get_regeneration_speed( ) + m_add_stamina_regen );
	}

	const u32 medkit_time = math::min( m_activity_time_ms, time_left_ms );
	m_activity_time_ms -= medkit_time;

	for ( u32 i = 0 ; i < m_influences_count ; ++i )
	{
		item_influence const& infl = m_influences[i];
		const float health_amount = medkit_time / 1000.0f * infl.health_amount;
		m_inventory->holder( ).damage_model( )->apply_med_kit(
			infl.body_part_name,
			health_amount
		);
	}

	if ( !m_activity_time_ms )
		set_active( false );

	// FUNCTION BODY[0x75fa50]: 48
	// <0x75fa59>|0x009|+0x00c:'140'
	// <0>
	// <0x75fa65>|0x015|+0x006:'142'
	// <0>
	// <0x75fa6b>|0x01b|+0x00c:'144'	if ( m_delay_ms )
	// <0>
	// <0x75fa77>|0x027|+0x014:'146'		const u32 delay_time = math::min( m_delay_ms, time_left_ms );
	// <0x75fa8b>|0x03b|+0x015:'147'		m_delay_ms -= delay_time;
	// <0>
	// <1>
	// <0x75faa0>|0x050|+0x00c:'150'		if ( !m_delay_ms )
	// <0x75faac>|0x05c|+0x005:'151'			return;
	// <0>
	// <0x75fab1>|0x061|+0x009:'153'		time_left_ms -= delay_time;
	// <0>
	// <0x75faba>|0x06a|+0x002:'155'		if ( !time_left_ms )
	// <0x75fabc>|0x06c|+0x005:'156'			return;
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x75fac1>|0x071|+0x014:'161'	if ( m_activity_time_ms == m_config_activity_time_ms )
	// <0>
	// <1>
	// <0x75fad5>|0x085|+0x008:'164'		remove_affects( );
	// <0>
	// <0x75fadd>|0x08d|+0x031:'166'		player_stamina& stamina = m_inventory->holder( ).c
	// <0x75fb0e>|0x0be|+0x02b:'167'		stamina.set_regeneration_speed
	// <0>
	// <1>
	// <0x75fb39>|0x0e9|+0x014:'170'	const u32 medkit_time = math::min( m_activity_time_ms, time_left_ms );
	// <0x75fb4d>|0x0fd|+0x015:'171'	m_activity_time_ms -= medkit_time;
	// <0>
	// <1>
	// <0x75fb62>|0x112|+0x021|[1]:'174'	for ( u32 i = 0 ; i < m_influences_count ; ++i )
	// <0>
	// <0x75fb83>|0x133|+0x012:'176'		item_influence const& infl = m_influences[i];
	// <0x75fb95>|0x145|+0x01f:'177'		const float health_amount = medkit_time
	// <0>
	// <0x75fbb4>|0x164|+0x035:'179'
	// <0>
	// <0x75fbe9>|0x199|+0x002:'181'		}
	// <0>
	// <0x75fbeb>|0x19b|+0x016:'183'
	// <0>
	// <0x75fc01>|0x1b1|+0x00a:'185'
	// <0>
	// <1>
	// ******
}

// STATE[UNCHECKED]
void medkit::remove_affects( )
{
	for ( u32 i = 0 ; i < m_affects_count; ++i )
	{
		medkit::affect const& affct = m_affects[i];
		m_inventory->holder( ).damage_model( )->cancel_affect( affct.body_part_name, affct.type ); // sushi@MATCH: Target pushed those two args before calling damage_model
	}

	// FUNCTION BODY[0x75f9d0]: 6
	// <0x75f9d9>|0x009|+0x021|[1]:'192'
	// <0>
	// <0x75f9fa>|0x02a|+0x012|[2]:'194'
	// <0x75fa0c>|0x03c|+0x037:'195'
	// <0>
	// <1>
	// ******
}

// STATE[99.90%|DONE]: 4 less bytes on stack in base
medkit::damage_protection const* medkit::find_damage_protection( pcstr body_part_name, pcstr hit_type )
{
	for ( u32 i = 0 ; i < m_damage_protect_count ; ++i )
	{
		medkit::damage_protection const& dmgp = m_damage_protect[i];

		if ( strings::equal( dmgp.body_part_name, body_part_name ) && strings::equal( dmgp.hit_type, hit_type ) )
			return &dmgp;
	}
	return NULL;

	// FUNCTION BODY[0x75f660]: 8
	// <0x75f669>|0x009|+0x021|[1]:'202'
	// <0>
	// <0x75f68a>|0x02a|+0x012|[2]:'204'
	// <0>
	// <0x75f69c>|0x03c|+0x02a:'206'
	// <0x75f6c6>|0x066|+0x005:'207'
	// <0x75f6cb>|0x06b|+0x002:'208'
	// <0x75f6cd>|0x06d|+0x002:'209'
	// ******
}

// STATE[100%|DONE]
float medkit::reduce_damage(
	pcstr		body_part_name,
	pcstr		damage_type,
	float		amount,
	float		armor_piercing
)
{
	VOSTOK_UNREFERENCED_PARAMETER( armor_piercing );

	medkit::damage_protection const* dmgp = find_damage_protection( body_part_name, damage_type );

	if ( !dmgp )
		return amount;

	if ( dmgp->threshold > amount )
		return 0.0f;

	return ( amount - dmgp->threshold ) * dmgp->hit_coeff;
}

} // namespace survarium

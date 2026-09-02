// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/medkit.h>

#include <vostok/game_core/base_player.h>
#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/player_stamina.h>

namespace survarium {

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

 medkit::~medkit( )
{
	VOSTOK_FREE_IMPL( g_allocator, m_influences );
	VOSTOK_FREE_IMPL( g_allocator, m_affects );
	VOSTOK_FREE_IMPL( g_allocator, m_damage_protect );
}

// sushi@NOTE: allocations look off but reproduce the target.
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
		strings::copy( infl.body_part_name, 0x10, static_cast_checked< pcstr >( influences[i]["body_part"] ) );
		infl.health_amount = (float)influences[i]["amount"];
		infl.health_amount /= activity_time_sec;
	}

	configs::binary_config_value remove_affects = config["remove_affects"];

	m_affects_count = (u8)remove_affects.size( );
	m_affects		= (affect*)VOSTOK_MALLOC_IMPL( g_allocator, sizeof( affect ) * m_affects_count, "medkit_affects" );

	for ( u32 i = 0 ; i < m_affects_count ; ++i )
	{
		affect& affct = m_affects[i];
		strings::copy( affct.body_part_name, 0x10, static_cast_checked< pcstr >( remove_affects[i]["body_part"] ) );
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
		strings::copy( dmgp.body_part_name, 0x10, static_cast_checked< pcstr >( damage_protect[i]["body_part"] ) );
		strings::copy( dmgp.hit_type, 0x10, static_cast_checked< pcstr >( damage_protect[i]["hit_type"] ) );
		dmgp.hit_coeff = (float)damage_protect[i]["hit_coeff"];
		dmgp.threshold = (float)damage_protect[i]["threshold"];
	}
}

// claude@NOTE: walled by inventory::holder()/inventory_holder::scheduler() LTCG inlining
// (both kept out-of-line in the target, always inlined by our /GL build). remove_affects /
// active_tick share the same wall (active_tick additionally needs math::min out-of-line).
// Source kept faithful: (*holder().damage_model()) via operator*, matching the target.
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
			( *m_inventory->holder( ).damage_model( ) ).register_body_part_damage_protector(
				dmgp.body_part_name,
				&dmgp.protector
			);
		else
			( *m_inventory->holder( ).damage_model( ) ).unregister_body_part_damage_protector(
				dmgp.body_part_name,
				&dmgp.protector
			);
	}
}

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
}

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
		( *m_inventory->holder( ).damage_model( ) ).apply_med_kit(
			infl.body_part_name,
			health_amount
		);
	}

	if ( empty( ) )
		set_active( false );
}

void medkit::remove_affects( )
{
	for ( u32 i = 0 ; i < m_affects_count; ++i )
	{
		medkit::affect const& affct = m_affects[i];
		( *m_inventory->holder( ).damage_model( ) ).cancel_affect( affct.body_part_name, affct.type );
	}
}

medkit::damage_protection const* medkit::find_damage_protection( pcstr body_part_name, pcstr hit_type )
{
	for ( u32 i = 0 ; i < m_damage_protect_count ; ++i )
	{
		medkit::damage_protection const& dmgp = m_damage_protect[i];

		if ( strings::equal( dmgp.body_part_name, body_part_name ) && strings::equal( dmgp.hit_type, hit_type ) )
			return &dmgp;
	}
	return NULL;
}

float medkit::reduce_damage(
	pcstr		body_part_name,
	pcstr		damage_type,
	const float		amount,
	const float		armor_piercing
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

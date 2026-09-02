// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/oxygen_tank.h>

#include <vostok/game_core/inventory_item_props.h>
#include <vostok/game_core/inventory_holder.h>

namespace survarium {

oxygen_tank::oxygen_tank( ) :
	inventory_item	( use_silent ),
	m_active		( false ),
	m_amount_ms		( 0 ),
	m_max_amount	( 0 ),
	m_influences	( NULL ) // sushi@NOTE: Target forgot m_influences_count
{
}

 oxygen_tank::~oxygen_tank( )
{	// sushi@NOTE: No check that it wasn't initialized, but this is fine
	for ( u32 i = 0 ; i < m_influences_count ; ++i )
	{
		item_influence& infl = m_influences[i];
		// explicit VIRTUAL dtor call (push 0; call [vptr]) - the target destroys only the
		// protector member; an `infl.~item_influence( )` devirtualizes to a direct call.
		infl.protector.~damage_protector( );
	}
	VOSTOK_FREE_IMPL( g_allocator, m_influences );
}

void oxygen_tank::load( configs::binary_config_value config )
{
	m_amount_ms = math::floor( (float)config["amount_time_sec"] * 1000.f );

	m_max_amount = m_amount_ms;

	configs::binary_config_value influences = config["influences"];

	m_influences_count = (u8)influences.size( );
	m_influences = (item_influence*)VOSTOK_MALLOC_IMPL( g_allocator, sizeof( item_influence ) * m_influences_count, "oxygen_tank_influences" );

	for ( u32 i = 0 ; i < m_influences_count ; ++i )
	{
		item_influence& infl = m_influences[i];
		new ( &infl ) damage_protector( );
		infl.protector.reduce_damage_functor = boost::bind( &oxygen_tank::reduce_damage, this, _1, _2, _3, _4 );
		strings::copy( infl.body_part_name, 0x10, static_cast_checked< pcstr >( influences[i]["body_part"] ) );
		strings::copy( infl.hit_type,		0x10, static_cast_checked< pcstr >( influences[i]["hit_type"] ) );
		infl.hit_coeff = (float)influences[i]["hit_coeff"];
		infl.threshold = (float)influences[i]["threshold"];
	}
}

void oxygen_tank::action( bool key_down )
{
	if ( !key_down )
		return;

	if ( !empty( ) )
		set_active( !m_active );
}

// claude@NOTE: walled by inventory::holder()/inventory_holder::scheduler() LTCG inlining
// (both out-of-line in target, inlined by our /GL). LOG_INFO args recovered. A ~13-byte
// residual remains in the LOG __FUNCSIG__ string immediate (push 0x61 vs target 0x54);
// the delinker truncates both funcsig strings identically so the tail can't be compared,
// and a matched LOG sibling (collision_geometry::load) proves funcsig CAN match - this
// looks like a delinker string-resolution artifact, not source.
void oxygen_tank::set_active( bool bactive )
{
	m_active = bactive;

	if ( m_active )
		m_inventory->holder( ).scheduler( ).register_for_update(
			&m_scheduler_identifier,
			boost::bind( &oxygen_tank::active_tick, this, _1 ),
			true,
			100,
			1,
			0
		);
	else
		m_inventory->holder( ).scheduler( ).unregister( &m_scheduler_identifier );

	for ( u32 i = 0 ; i < m_influences_count ; ++i )
	{
		item_influence /*const*/& infl = m_influences[i]; // sushi@TODO
		if ( m_active )
			( *m_inventory->holder( ).damage_model( ) ).register_body_part_damage_protector(
				infl.body_part_name,
				&infl.protector
			);
		else
			( *m_inventory->holder( ).damage_model( ) ).unregister_body_part_damage_protector(
				infl.body_part_name,
				&infl.protector
			);
	}

	LOG_INFO( "Oxygen Tank switched to [%s]. amount= %dms", bactive ? "ON" : "OFF", m_amount_ms );
}

void oxygen_tank::active_tick( const u32 frame_time_ms )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_active ) );

	m_amount_ms -= math::min( m_amount_ms, frame_time_ms );
	LOG_INFO( "amount is: %dms", m_amount_ms );

	if ( empty( ) )
		set_active( false );
}

oxygen_tank::item_influence const* oxygen_tank::find_influence( pcstr body_part_name, pcstr hit_type )
{
	for ( u32 i = 0 ; i < m_influences_count ; ++i )
	{
		item_influence const& infl = m_influences[i];
		if ( strings::equal( infl.body_part_name, body_part_name )
			&& strings::equal( infl.hit_type, hit_type ) )
			return &infl;
	}

	return NULL;
}

float oxygen_tank::reduce_damage(
	pcstr			body_part_name,
	pcstr			damage_type,
	const float		amount,
	const float		armor_piercing
)
{
	item_influence const* infl = find_influence( body_part_name, damage_type );

	if ( !infl )
		return amount;

	if ( infl->threshold > amount )
		return 0.0f;

	return ( amount - infl->threshold ) * infl->hit_coeff;
}

bool oxygen_tank::get_item_props( inventory_item_props& props )
{
	inventory_item::get_item_props( props );

	props.m_amount_ms = m_amount_ms;

	props.cooldown = (u8)( (float)m_amount_ms / m_max_amount * 100.0f );

	return m_active;
}

} // namespace survarium

////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/oxygen_tank.h>

#include <vostok/game_core/inventory_item_props.h>
#include <vostok/game_core/inventory_holder.h>

namespace survarium {

// STATE[100%|DONE]
oxygen_tank::oxygen_tank( ) :
	inventory_item	( use_silent ),
	m_active		( false ),
	m_amount_ms		( 0 ),
	m_max_amount	( 0 ),
	m_influences	( NULL ) // sushi@NOTE: Target forgot m_influences_count
{
}

// STATE[UNCHECKED]
 oxygen_tank::~oxygen_tank( )
{	// sushi@NOTE: No check that it wasn't initialized, but this is fine
	for ( u32 i = 0 ; i < m_influences_count ; ++i )
	{
		item_influence& infl = m_influences[i];
		infl.~item_influence( );
	}
	VOSTOK_DELETE_IMPL( g_allocator, m_influences );

	// FUNCTION BODY[0x6fa480]: 6
	// <0x6fa492>|0x012|+0x021|[1]:'24'
	// <0>
	// <0x6fa4b3>|0x033|+0x012|[2]:'26'
	// <0x6fa4c5>|0x045|+0x00e:'27'
	// <0x6fa4d3>|0x053|+0x002:'28'
	// <0x6fa4d5>|0x055|+0x040:'29'
	// ******
}

// STATE[91.94%|DONE]: Registers differ because of LTCG. Seems like that's it
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
		strings::copy( infl.body_part_name, 0x10, influences[i]["body_part"] );
		strings::copy( infl.hit_type,		0x10, influences[i]["hit_type"] );
		infl.hit_coeff = (float)influences[i]["hit_coeff"];
		infl.threshold = (float)influences[i]["threshold"];
	}

	// FUNCTION BODY[0x6fa1c0]: 21
	// <0x6fa1d0>|0x010|+0x034:'34'
	// <0>
	// <0x6fa204>|0x044|+0x018:'36'
	// <0>
	// <0x6fa21c>|0x05c|+0x030:'38'	configs::binary_config_value influences
	// <0>
	// <0x6fa24c>|0x08c|+0x014:'40'
	// <0x6fa260>|0x0a0|+0x02f:'41'	m_influences = VOSTOK_M
	// <0>
	// <1>
	// <0x6fa28f>|0x0cf|+0x028|[1]:'44'
	// <0>
	// <0x6fa2b7>|0x0f7|+0x015:'46'		item_influence& infl = m_influences[i];
	// <0x6fa2cc>|0x10c|+0x031:'47'
	// <0x6fa2fd>|0x13d|+0x0bb:'48'		infl.protector.reduce_damage_functor
	// <0x6fa3b8>|0x1f8|+0x038:'49'		strings::copy
	// <0x6fa3f0>|0x230|+0x038:'50'
	// <0x6fa428>|0x268|+0x024:'51'
	// <0x6fa44c>|0x28c|+0x029:'52'
	// <0>
	// <1>
	// ******
}

// STATE[100%|DONE]
void oxygen_tank::action( bool key_down )
{
	if ( !key_down )
		return;

	if ( !empty( ) )
		set_active( !m_active );
}

// STATE[50.42%|PARTIAL]. holder( ).scheduler( )
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
			m_inventory->holder( ).damage_model( )->register_body_part_damage_protector(
				infl.body_part_name,
				&infl.protector
			);
		else
			m_inventory->holder( ).damage_model( )->unregister_body_part_damage_protector(
				infl.body_part_name,
				&infl.protector
			);
	}

	LOG_INFO( "Oxygen Tank switched to [%s]. amount= %dms" );

	// FUNCTION BODY[0x6f9dc0]: 17
	// <0x6f9dd7>|0x017|+0x00f:'68'
	// <0>
	// <0x6f9de6>|0x026|+0x015:'70'
	// <0x6f9dfb>|0x03b|+0x0a9:'71'
	// <0x6f9ea4>|0x0e4|+0x002:'72'
	// <0x6f9ea6>|0x0e6|+0x02a:'73'
	// <0>
	// <0x6f9ed0>|0x110|+0x028|[1]:'75'
	// <0>
	// <0x6f9ef8>|0x138|+0x015|[2]:'77'
	// <0x6f9f0d>|0x14d|+0x011:'78'
	// <0x6f9f1e>|0x15e|+0x050:'79'			m_inventory->h
	// <0x6f9f6e>|0x1ae|+0x002:'80'		else
	// <0x6f9f70>|0x1b0|+0x050:'81'			m_inventory->h
	// <0x6f9fc0>|0x200|+0x005:'82'
	// <0>
	// <0x6f9fc5>|0x205|+0x0a8:'84'
	// ******
}

// STATE[92.30%|PARTIAL]: Logging, as always
void oxygen_tank::active_tick( const u32 frame_time_ms )
{
	ASSERT( UNKNOWN_EXPRESSION );

	m_amount_ms -= math::min( m_amount_ms, frame_time_ms );
	LOG_INFO( "amount is: %dms", m_amount_ms );

	if ( empty( ) )
		set_active( false );

	// FUNCTION BODY[0x6fa0d0]: 7
	// <0x6fa0e1>|0x011|+0x00c:'89'
	// <0>
	// <0x6fa0ed>|0x01d|+0x025:'91'
	// <0x6fa112>|0x042|+0x081:'92'
	// <0>
	// <0x6fa193>|0x0c3|+0x016:'94'
	// <0x6fa1a9>|0x0d9|+0x00a:'95'
	// ******
}

// STATE[99.90%|DONE]
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

	// FUNCTION BODY[0x6f9cf0]: 8
	// <0x6f9cf9>|0x009|+0x021|[1]:'100'
	// <0>
	// <0x6f9d1a>|0x02a|+0x012|[2]:'102'
	// <0>
	// <0x6f9d2c>|0x03c|+0x02a:'104'
	// <0x6f9d56>|0x066|+0x005:'105'
	// <0x6f9d5b>|0x06b|+0x002:'106'
	// <0x6f9d5d>|0x06d|+0x002:'107'
	// ******
}

// STATE[100%|DONE]
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

// STATE[100%|DONE]
bool oxygen_tank::get_item_props( inventory_item_props& props )
{
	inventory_item::get_item_props( props );

	props.m_amount_ms = m_amount_ms;

	props.cooldown = (u8)( (float)m_amount_ms / m_max_amount * 100.0f );

	return m_active;
}

} // namespace survarium
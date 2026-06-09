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

// STATE[97.53%|DONE]: LTCG - target spills the binary_config_value temp to a stack slot on the two strings::copy lines; otherwise byte-identical
// STRUCTURE DIFF[target 0x6ea1c0 | base 0x44abb0]: target 18 / base 18 stmts
// .. same ..
// 0x1f8 <0x38> | 0x1f8 <0x2c> | strings::copy( infl.body_part_name, 0x10, influences[i]["body_part"] );   SIZE
// 0x230 <0x38> | 0x224 <0x2c> | strings::copy( infl.hit_type,		0x10, influences[i]["hit_type"] );   SIZE
// .. same ..
// ; aligned 16, size-diffs 2, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole diffs are the target materializing the influences[i][...] binary_config_value temp into a stack slot before strings::copy (extra mov to/from [ebp-A0h]); LTCG temp-spill at the call boundary, non-steerable. trail: oxygen_tank.md
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
}

// STATE[100%|DONE]
void oxygen_tank::action( bool key_down )
{
	if ( !key_down )
		return;

	if ( !empty( ) )
		set_active( !m_active );
}

// STATE[48.62%|PARTIAL]: inline-vs-call wall - target out-of-line-calls holder()/scheduler()/register_for_update(); our base inlines them
// STRUCTURE DIFF[target 0x6e9dc0 | base 0x44a720]: target 17 / base 20 stmts
// .. same ..
// --          | <0>         |    EMPTY only base
// 0x03b <0xa9> | 0x03b <0xf5> | );   SIZE
// .. same ..
// 0x0e6 <0x2a> | 0x132 <0x3e> | m_inventory->holder( ).scheduler( ).unregister( &m_scheduler_identifier );   SIZE
// .. same ..
// --          | <0>         |    EMPTY only base
// 0x15e <0x50> | 0x1be <0x73> | );   SIZE
// .. same ..
// --          | <0>         |    EMPTY only base
// 0x1b0 <0x50> | 0x233 <0x73> | );   SIZE
// .. same ..
// 0x205 <0xa8> | 0x2ab <0x77> | LOG_INFO( "Oxygen Tank switched to [%s]. amount= %dms" );   SIZE
// ; aligned 12, size-diffs 5, quantity-diffs 3
// VERDICT: STRUCTURE MATCH (shape ok) - target emits inventory::holder/inventory_holder::scheduler/scheduler::register_for_update OUT-OF-LINE (rva 0x86b70/0x82cc0/0x82da0) and calls them; our base inlines all three (base has no standalone copy). EMPTY-only-base rows are cosmetic blank-line gaps. Inline-vs-call inliner decision on inline accessors, non-steerable (de-inlining the accessor would break every other matched caller). trail: oxygen_tank.md
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
}

// STATE[75.86%|PARTIAL]: math::min inline-vs-call + logging-macro LTCG, non-steerable
// STRUCTURE DIFF[target 0x6ea0d0 | base 0x44aaa0]: target 7 / base 7 stmts
// .. same ..
// 0x01d <0x25> | 0x01d <0x3d> | m_amount_ms -= math::min( m_amount_ms, frame_time_ms );   SIZE
// 0x042 <0x81> | 0x05a <0x82> | LOG_INFO( "amount is: %dms", m_amount_ms );   SIZE
// .. same ..
// ; aligned 5, size-diffs 2, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - target INLINES math::min(u32,u32) to branchless cmp/sbb/and/add; our base out-of-line-calls it (target also keeps a standalone copy at 0x3fbb0). LOG_INFO is the usual logging-macro LTCG. Both inline-vs-call inliner decisions, non-steerable. trail: oxygen_tank.md
void oxygen_tank::active_tick( const u32 frame_time_ms )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_active ) );

	m_amount_ms -= math::min( m_amount_ms, frame_time_ms );
	LOG_INFO( "amount is: %dms", m_amount_ms );

	if ( empty( ) )
		set_active( false );
}

// STATE[99.90%|DONE]: byte-identical; lone diff is a cosmetic blank-line gap (size-diffs 0)
// STRUCTURE DIFF[target 0x6e9cf0 | base 0x44a650]: target 8 / base 9 stmts
// .. same ..
// --          | <0>         |    EMPTY only base
// .. same ..
// ; aligned 8, size-diffs 0, quantity-diffs 1
// VERDICT: STRUCTURE MATCH - all statements byte-for-byte aligned; the single quantity row is a collapsed blank source-line between `return &infl;`'s `}` and `return NULL;`, non-steerable cosmetic gap. trail: oxygen_tank.md
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
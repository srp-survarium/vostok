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

// STATE[100%|DONE]: was 61.61 with `infl.~item_influence( )` + VOSTOK_DELETE_IMPL.
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

// STATE[95.17%|DONE]: the two strings::copy SIZE rows were the static_cast_checked< pcstr >
// return-temp (+0xc each), now restored - 13/13 stmts, 0x2bc bytes BOTH sides; the % residual
// (was 97.53 with the wrong-shape plain casts) is purely the ICF fold-name relocs on the
// kept-out-of-line cast instantiations, same class as medkit::load.
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

	// STRUCTURE DIFF: target 13 stmts / base 13 stmts (0x2bc both) - no diverging rows
	// VERDICT: STRUCTURE MATCH - residual is ICF fold-name relocs on the kept-out-of-line cast instantiations (same class as medkit::load).
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

	// STRUCTURE DIFF: target 13 stmts / base 13 stmts
	// SIZE +0x4c | 113 | );
	// SIZE +0x14 | 115 | m_inventory->holder( ).scheduler( ).unregister( &m_scheduler_identifier );
	// SIZE +0x23 | 124 | );
	// SIZE +0x23 | 129 | );
	// SIZE -0x31 | 132 | LOG_INFO( "Oxygen Tank switched to [%s]. amount= %dms" );
	// VERDICT: STRUCTURE MATCH (13/13) - target keeps inventory::holder/inventory_holder::scheduler/scheduler::register_for_update out-of-line (rva 0x86b70/0x82cc0/0x82da0); base inlines all three. LOG_INFO is the logging-macro LTCG. Inline-vs-call, non-steerable from this TU.
}

// STATE[75.86%|PARTIAL]: math::min inline-vs-call + logging-macro LTCG, non-steerable
void oxygen_tank::active_tick( const u32 frame_time_ms )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_active ) );

	m_amount_ms -= math::min( m_amount_ms, frame_time_ms );
	LOG_INFO( "amount is: %dms", m_amount_ms );

	if ( empty( ) )
		set_active( false );

	// STRUCTURE DIFF: target 5 stmts / base 5 stmts
	// SIZE +0x18 | 147 | m_amount_ms -= math::min( m_amount_ms, frame_time_ms );
	// SIZE +0x1  | 148 | LOG_INFO( "amount is: %dms", m_amount_ms );
	// VERDICT: STRUCTURE MATCH (5/5) - target inlines math::min(u32,u32) to branchless cmp/sbb/and/add while base calls it out-of-line; LOG_INFO is the logging-macro LTCG. Both inline-vs-call, non-steerable.
}

// STATE[99.90%|DONE]: byte-identical; residual is the empty_stub/ICF reloc fold-name only
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

	// STRUCTURE DIFF: target 6 stmts / base 6 stmts (0x75 both) - no diverging rows
	// VERDICT: STRUCTURE MATCH - clean 6/6 alignment; 99.90 residual is reloc fold-naming noise.
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
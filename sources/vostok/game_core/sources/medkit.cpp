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

// STATE[67.06%|PARTIAL]: VOSTOK_DELETE_IMPL inlined element-dtor differs (LTCG), non-steerable. trail: medkit.md
// STRUCTURE DIFF[target 0x750180 | base 0x44bc60]: target 3 / base 3 stmts
// 0x012 <0x40> | 0x012 <0x4b> | VOSTOK_DELETE_IMPL( g_allocator, m_influences );   SIZE
// 0x052 <0x40> | 0x05d <0x4a> | VOSTOK_DELETE_IMPL( g_allocator, m_affects );   SIZE
// 0x092 <0x40> | 0x0a7 <0x1d> | VOSTOK_DELETE_IMPL( g_allocator, m_damage_protect );   SIZE
// ; aligned 0, size-diffs 3, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - all 3 statements align; each VOSTOK_DELETE_IMPL inlines the array element-dtor + free differently (whole-program inline form), non-steerable.
 medkit::~medkit( )
{
	VOSTOK_DELETE_IMPL( g_allocator, m_influences );
	VOSTOK_DELETE_IMPL( g_allocator, m_affects );
	VOSTOK_DELETE_IMPL( g_allocator, m_damage_protect );
}

// STATE[96.66%|PARTIAL]: strings::copy / for-loop counter inline-vs-call LTCG, non-steerable. trail: medkit.md
// STRUCTURE DIFF[target 0x74fc20 | base 0x44b480]: target 48 / base 48 stmts
// 0x136 <0x28> | 0x136 <0x24> | for ( u32 i = 0 ; i < m_influences_count ; ++i )   SIZE
// 0x173 <0x35> | 0x16f <0x29> | strings::copy( infl.body_part_name, 0x10, influences[i]["body_part"] );   SIZE
// 0x292 <0x35> | 0x282 <0x29> | strings::copy( affct.body_part_name, 0x10, remove_affects[i]["body_part"] );   SIZE
// 0x2e9 <0x5> | 0x2cd <0x2> | }   SIZE
// 0x498 <0x38> | 0x479 <0x2c> | strings::copy( dmgp.body_part_name, 0x10, damage_protect[i]["body_part"] );   SIZE
// 0x4d0 <0x38> | 0x4a5 <0x2c> | strings::copy( dmgp.hit_type, 0x10, damage_protect[i]["hit_type"] );   SIZE
// ; aligned 42, size-diffs 6, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - 48/48 statements; SIZE on strings::copy (inline-vs-call of the config-value -> pcstr conversion arg) and a loop-counter slot; whole-program LTCG, non-steerable. sushi@NOTE: allocations look off but reproduce the target.
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
}

// STATE[47.04%|PARTIAL]: register_for_update(boost::bind(...)) + resource_ptr accessors inlined deeper in base, non-steerable. trail: medkit.md
// STRUCTURE DIFF[target 0x74f730 | base 0x44b0d0]: target 23 / base 25 stmts
// 0x00d <0xc> | 0x00f <0xf> | m_active = bactive;   SIZE
// --          | 0x01e <0x15> | if ( m_active )   ONLY base
// 0x019 <0x12> | 0x033 <0x18> | m_activity_time_ms	= m_config_activity_time_ms;   SIZE
// --          | 0x04b <0x18> | m_delay_ms			= m_config_delay_ms;   ONLY base
// 0x02b <0x12> | 0x063 <0xf5> | );   SIZE
// 0x04f <0xa3> | 0x158 <0x5> | else   SIZE
// 0x0f2 <0x2> | 0x15d <0x50> | player_stamina& stamina = m_inventory->holder( ).cast_to_base_player( )->stamina( );   SIZE
// 0x0f4 <0x31> | 0x1db <0x33> | m_inventory->holder( ).scheduler( ).unregister( &m_scheduler_identifier );   SIZE
// 0x150 <0x23> | 0x20e <0x28> | for ( u32 i = 0 ; i < m_damage_protect_count ; ++i )   SIZE
// 0x173 <0x25> | 0x236 <0x15> | damage_protection& dmgp = m_damage_protect[i];   SIZE
// 0x198 <0x12> | 0x25c <0x73> | );   SIZE
// 0x1ef <0x43> | 0x2d1 <0x78> | );   SIZE
// ; aligned 9, size-diffs 10, quantity-diffs 10
// VERDICT: STRUCTURE MATCH (shape ok) - the ONLY base/target rows are multi-line-statement attribution boundaries of the inlined register_for_update(boost::bind(...)) and the holder()/damage_model() resource_ptr accessor calls; base inlines these one layer deeper than the target, enlarging the frame (0xC0 vs 0x84) and shifting every slot; whole-program LTCG, non-steerable.
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
}

// STATE[96.17%|PARTIAL]: set_amount inline-vs-call LTCG, non-steerable. trail: medkit.md
// STRUCTURE DIFF[target 0x74f970 | base 0x44b420]: target 10 / base 10 stmts
// 0x03a <0x16> | 0x03a <0x17> | set_amount( amount( ) - 1 );	// sushi@MATCH: LTCG for set_amount   SIZE
// ; aligned 9, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE on set_amount( amount() - 1 ): whole-program inline-vs-call of set_amount, non-steerable.
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

// STATE[53.50%|PARTIAL]: math::min template + resource_ptr accessor inline-vs-call LTCG, non-steerable. trail: medkit.md
// STRUCTURE DIFF[target 0x74fa50 | base 0x44ba50]: target 39 / base 36 stmts
// 0x027 <0x14> | 0x027 <0x2c> | const u32 delay_time = math::min( m_delay_ms, time_left_ms );   SIZE
// 0x08d <0x31> | 0x0a5 <0x38> | player_stamina& stamina = m_inventory->holder( ).cast_to_base_player( )->stamina( );   SIZE
// 0x0e9 <0x14> | 0x108 <0x2c> | const u32 medkit_time = math::min( m_activity_time_ms, time_left_ms );   SIZE
// 0x112 <0x21> | 0x149 <0x25> | for ( u32 i = 0 ; i < m_influences_count ; ++i )   SIZE
// 0x164 <0x35> | 0x19f <0x4f> | );   SIZE
// --          | 0x1ee <0x5> | }   ONLY base
// 0x199 <0x2> | 0x1f3 <0xc> | if ( !m_activity_time_ms )   SIZE
// ; aligned 29, size-diffs 6, quantity-diffs 5
// VERDICT: STRUCTURE MATCH (shape ok) - 2 SIZE on math::min (template inlined to a tighter cmov in target), the others + the ONLY base/target rows are the inlined apply_med_kit / holder() resource_ptr accessor's multi-line attribution; base inlines deeper, shifting slots; whole-program LTCG, non-steerable.
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
}

// STATE[60.46%|PARTIAL]: holder()/damage_model() resource_ptr accessor inlined as a by-value temp in base, non-steerable. trail: medkit.md
// STRUCTURE DIFF[target 0x74f9d0 | base 0x44b9b0]: target 4 / base 4 stmts
// 0x03c <0x37> | 0x03c <0x51> | m_inventory->holder( ).damage_model( )->cancel_affect( affct.body_part_name, affct.type );   SIZE
// ; aligned 3, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE on the cancel_affect call: base inlines damage_model() (returns a resource_ptr by value) with the temp-construct/test materialization while the target out-of-lines it; cascades a bigger frame (0x20 vs 0x14); whole-program LTCG, non-steerable.
void medkit::remove_affects( )
{
	for ( u32 i = 0 ; i < m_affects_count; ++i )
	{
		medkit::affect const& affct = m_affects[i];
		m_inventory->holder( ).damage_model( )->cancel_affect( affct.body_part_name, affct.type ); // sushi@MATCH: Target pushed those two args before calling damage_model
	}
}

// STATE[99.90%|DONE]: frame 4 bytes smaller in base (slot allocation), non-steerable. trail: medkit.md
// STRUCTURE DIFF[target 0x74f660 | base 0x44b000]: target 8 / base 8 stmts
// .. same ..
// ; aligned 8, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - all 8 statements align with 0 diffs; residual is a 4-byte frame-size / slot-allocation difference, non-steerable.
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

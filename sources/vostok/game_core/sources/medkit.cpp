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

// STATE[100%|DONE]: was 67.06 with VOSTOK_DELETE_IMPL - the target's three statements are the
// free_helper shape exactly (null-check, free_impl, pointer=0, NO element dtor even for the
// non-trivial damage_protection), i.e. VOSTOK_FREE_IMPL, symmetric with the raw
// VOSTOK_MALLOC_IMPL allocations in load.
 medkit::~medkit( )
{
	VOSTOK_FREE_IMPL( g_allocator, m_influences );
	VOSTOK_FREE_IMPL( g_allocator, m_affects );
	VOSTOK_FREE_IMPL( g_allocator, m_damage_protect );
}

// STATE[96.20%|PARTIAL]: static_cast_checked< pcstr > on the four strings::copy args closed
// ALL six SIZE rows - 34/34 statements, 0x55c bytes BOTH sides; the sub-100 residual is purely
// the ICF fold-name relocs on the kept-out-of-line cast instantiations.
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

	// STRUCTURE DIFF: target 34 / base 34 stmts (no diverging rows, 0x55c bytes both)
	// VERDICT: STRUCTURE MATCH - residual is fold-name reloc pairing on the static_cast_checked instantiations only.
}

// STATE[47.04%|PARTIAL]: hand-pairing the new diff aligns ALL 15 statements 1:1 in order -
// no quantity divergence; the % is the holder()/cast_to_base_player()/stamina()/scheduler()/
// damage_model() accessor inline-vs-call walls plus the this-slot disp8->disp32 cascade
// (base frame grows past -0x80, +3 bytes on nearly every member-touching statement).
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

	// STRUCTURE DIFF: target 15 / base 15 stmts (sequential hand-pairing; the table's ONLY rows are aligner slides)
	// SIZE +0xe3 | 139 | ); (register_for_update bind)
	// SIZE +0x4e | 143 | player_stamina& stamina = m_inventory->holder( ).cast_to_base_player( )->stamina( );
	// SIZE +0x61 | 156 | ); (register_body_part_damage_protector)
	// SIZE +0x35 | 161 | ); (unregister_body_part_damage_protector)
	// VERDICT: STRUCTURE MATCH (shape ok) - base inlines the holder()/cast_to_base_player()/stamina()/scheduler()/damage_model() accessor chain one layer deeper than target (out-of-line there); the frame growth pushes this past -0x80 adding +3 per member statement; cross-unit headers, non-steerable.
}

// STATE[96.17%|PARTIAL]: set_amount inline-vs-call LTCG, non-steerable.
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

	// STRUCTURE DIFF: target 7 / base 7 stmts
	// SIZE +0x1 | 181 | set_amount( amount( ) - 1 );
	// VERDICT: STRUCTURE MATCH (shape ok) - whole-program inline-vs-call of set_amount, non-steerable.
}

// STATE[57.12%|PARTIAL]: the tail-if is empty() (the inlined cmp/sete/movzx bool shape gave
// the header stub its real body, 53.50 -> 57.12); residual is math::min(u32) and the
// holder()/damage_model() accessor inline-vs-call walls.
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

	if ( empty( ) )
		set_active( false );

	// STRUCTURE DIFF: target 23 / base 23 stmts
	// SIZE +0x18 | 204 | const u32 delay_time = math::min( m_delay_ms, time_left_ms );
	// SIZE +0x7  | 219 | player_stamina& stamina = m_inventory->holder( ).cast_to_base_player( )->stamina( );
	// SIZE +0x18 | 223 | const u32 medkit_time = math::min( m_activity_time_ms, time_left_ms );
	// SIZE +0x4  | 226 | for ( u32 i = 0 ; i < m_influences_count ; ++i )
	// SIZE +0x1a | 233 | ); (apply_med_kit)
	// BASE_ONLY  | 234 | } / TRGT_ONLY 0x2 - the same loop backjump, near in base vs short in target (size cascade)
	// VERDICT: STRUCTURE MATCH (shape ok) - base inlines math::min<u32> (cmp/sbb/neg/and branchless) where target calls the promoted out-of-line min; the stamina/apply_med_kit rows are the accessor inline walls; all cascade-only, non-steerable.
}

// STATE[60.46%|PARTIAL]: holder()/damage_model() resource_ptr accessor inlined as a by-value temp in base, non-steerable.
void medkit::remove_affects( )
{
	for ( u32 i = 0 ; i < m_affects_count; ++i )
	{
		medkit::affect const& affct = m_affects[i];
		m_inventory->holder( ).damage_model( )->cancel_affect( affct.body_part_name, affct.type ); // sushi@MATCH: Target pushed those two args before calling damage_model
	}

	// STRUCTURE DIFF: target 3 / base 3 stmts
	// SIZE +0x1a | 250 | m_inventory->holder( ).damage_model( )->cancel_affect( affct.body_part_name, affct.type );
	// VERDICT: STRUCTURE MATCH (shape ok) - base inlines damage_model() (by-value resource_ptr temp construct/test) where target calls it out-of-line; cross-unit header, non-steerable.
}

// STATE[99.90%|DONE]: frame 4 bytes smaller in base (slot allocation), non-steerable.
medkit::damage_protection const* medkit::find_damage_protection( pcstr body_part_name, pcstr hit_type )
{
	for ( u32 i = 0 ; i < m_damage_protect_count ; ++i )
	{
		medkit::damage_protection const& dmgp = m_damage_protect[i];

		if ( strings::equal( dmgp.body_part_name, body_part_name ) && strings::equal( dmgp.hit_type, hit_type ) )
			return &dmgp;
	}
	return NULL;

	// STRUCTURE DIFF: target 6 / base 6 stmts (no diverging rows)
	// VERDICT: STRUCTURE MATCH - residual is a 4-byte frame/slot allocation difference, non-steerable.
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

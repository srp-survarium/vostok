////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/base_player.h>

#include <vostok/game_core/base_player_creation_params.h>
#include <vostok/game_core/player_profile.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[INPROGRESS]: body is empty `{ }`; everything is the member-init list (decoded
// from the @0x72f0b0 asm: recoil_params<-params+0x10, dispersion_params<-params+0x20,
// breath_holding_params<-params+0x58, damage_model<-params+0x114, movement_speed_factor
// <-clear_value, then the 4 bools=0). NOT yet anchored (base_player is abstract) so it
// reads 0/unpaired in report.json. NEXT: (1) anchor via a concrete derived stub in
// temp_include_all.cpp that constructs a base_player_creation_params and escapes &obj;
// (2) like the dtor, full match is gated on the inventory_holder/collision_user/
// hit_initiator/hit_receiver base CTORS being out-of-line (target calls them out-of-line).
base_player::base_player( base_player_creation_params const& params, survarium::scheduler& the_scheduler ) :
	inventory_holder		( the_scheduler, params.inventory ),
	hit_initiator			( params.initial_info.id, params.initial_info.profile->is_local ),
	m_recoil_params			( params.recoil_params ),
	m_dispersion_params		( params.dispersion_params ),
	m_breath_holding_params	( params.breath_holding_params ),
	m_damage_model			( params.damage_model )
{
	// FUNCTION BODY
	// <0x73f0b0>|0x000|+0x1da:'31'	{
	// <0x73f28a>|0x1da|      :'32'	}
	// ******
}

// STATE[72.19%|PARTIAL]: gated on the base-class destructors (hit_receiver/hit_initiator/
// collision_user/inventory_holder) being OUT-OF-LINE: target calls them out-of-line and does
// NOT reset the 4 base vtables up front; our build inlines those base dtors. Other-unit
// headers - not steerable from here.
base_player::~base_player( )
{
	ASSERT( UNKNOWN_EXPRESSION );

	// STRUCTURE DIFF: target 1 stmt / base 1 stmt
	// VERDICT: STRUCTURE MATCH - sole source statement (the assert eater) aligns; the byte
	// residual (0xcd vs 0x108) is compiler-generated base/member destruction, inline-vs-call
	// of the four base-class dtors (other units).
}

// STATE[41.70%|PARTIAL]: full reconstruction from the @0x72ee40 asm (3.38 -> 41.70).
void base_player::tick_active_object( )
{
	( *m_current_active_object ).tick( );

	if ( m_current_active_object != m_target_active_object )
		if ( ( *m_current_active_object ).is_ready_to_be_deactivated( ) )
		{
			on_before_active_object_changed( m_current_active_object, m_target_active_object );
			( *m_current_active_object ).deactivate( );
			m_current_active_object	= m_target_active_object;
			( *m_current_active_object ).activate( *this, get_engine( ) );
			( *m_current_active_object ).tick( );
		}

	if ( m_current_active_object == m_target_active_object )
		if ( weapon_core const* const current_weapon = ( *m_current_active_object ).cast_weapon_core( ) )
			if ( u8 const broken_hands_count = ( *damage_model( ) ).broken_hands_count( ) )
				if ( !current_weapon->could_be_used( *this ) ) {
					profile_slot_enum another_weapon_slot_id = inventory( ).get_active_slot( ) != weapon1_slot ? weapon1_slot : weapon2_slot;
					inventory_item_ptr another_item = inventory( ).item_in_slot( another_weapon_slot_id );
					weapon_core const* const another_weapon = another_item ? ( *another_item ).cast_weapon_core( ) : NULL;
					if ( another_weapon && another_weapon->could_be_used( *this ) )
						inventory( ).action( another_weapon_slot_id, true );

				}

	// claude@MATCH: the trailing braced block is real - the target emits the cast (+0x54
	// vcall) and a dead store into a second block-scoped `current_weapon` local at the
	// function tail (lines 69-71 of the original; the value is never read - whatever
	// consumed it was compiled out of MASTER_GOLD).
	{
		weapon_core const* const current_weapon = ( *m_current_active_object ).cast_weapon_core( );
	}

	// STRUCTURE DIFF: target 19 stmts / base 19 stmts
	// (every statement pairs; all 18 remaining rows are SIZE +0x3..+0x38, base larger)
	// VERDICT: STRUCTURE MATCH - 19/19, blocks/jumps/ternary (sub;neg;sbb;and;add) byte-shape
	// aligned; every SIZE row is the engine-wide intrusive_ptr/resource_ptr deref-assign
	// inline-vs-call wall (target calls operator*/operator=/c_ptr/accessors out-of-line, our
	// /GL inlines them with their internal assert eaters). Non-steerable from this TU.
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::base_player::send_game_world_object(survarium::game_world_object const*, boost::function<vostok::network_core::udp_match_packet & __cdecl(void)> const&, boost::function<void __cdecl(vostok::network_core::udp_match_packet &)> const&) const
void base_player::send_game_world_object( game_world_object const* object, boost::function<network_core::udp_match_packet &()> const& reciver_packet_allocator, boost::function<void(network_core::udp_match_packet &)> const& reciver_enqueuer ) const
{
	// LOCALS
	// network_core::udp_match_packet& packet
	// inventory_item const* 		item
	// ******

	// CALL SITE INFO
	// <0x73ec30> -> inventory_item const* <unknown>() const
	// <0x73ec80> -> void <unknown>(network_core::udp_match_packet&) const
	// ******

	// FUNCTION BODY
	// <0x73ebe0>|0x000|+0x00f:'79'	{
	// <0x73ebef>|0x00f|+0x00b:'80'
	// <0>
	// <0x73ebfa>|0x01a|+0x00c:'82'
	// <0x73ec06>|0x026|+0x013:'83'
	// <0>
	// <0x73ec19>|0x039|+0x00c:'85'
	// <0x73ec25>|0x045|+0x010:'86'
	// <0>
	// <0x73ec35>|0x055|+0x00c:'88'
	// <0x73ec41>|0x061|+0x030:'89'
	// <0>
	// <0x73ec71>|0x091|+0x011:'91'
	// <0>
	// <0x73ec82>|0x0a2|+0x00c:'93'
	// <0x73ec8e>|0x0ae|      :'94'	}
	// ******
}

// STATE[0.00%|PARTIAL]: now emitted + PAIRED (anchored via temp_include_all; was DCE'd with
// no base symbol). report.json omits the percent because it is literally 0.0 (proto3
// default-value omission) - every statement is the packet_reader::r<T>/inventory accessor
// inline-vs-call wall, same class as player_stamina::deserialize.
void base_player::deserialize_game_world_object( network_core::packet_reader& reader )
{
	profile_slot_enum	slot	= (profile_slot_enum)reader.r< bool >( );

	ASSERT( UNKNOWN_EXPRESSION );

	inventory_item_ptr	item	= inventory( ).item_in_slot( slot );
	item->deserialize_game_world_object( reader );

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE +0x13 | 130 | profile_slot_enum	slot	= (profile_slot_enum)reader.r< bool >( );
	// SIZE +0x58 | 134 | inventory_item_ptr	item	= inventory( ).item_in_slot( slot );
	// SIZE +0x7  | 135 | item->deserialize_game_world_object( reader );
	// VERDICT: STRUCTURE MATCH - 4/4 (the assert-eater row aligns byte-exact); the SIZE rows
	// are r<bool>/inventory()/item_in_slot/copy-ctor kept out-of-line by target, inlined by
	// our /GL. Non-steerable from this TU.
}

// STATE[100%|DONE]
void base_player::subscribe_on_player_death( player_death_subscriber* subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_player_death_subscribers.push_back( subscriber );
}

// STATE[100%|DONE]
void base_player::unsubscribe_from_player_death( player_death_subscriber* subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_player_death_subscribers.erase( subscriber );
}

// STATE[100%|DONE]
static void call_player_death_subscriber_callback( player_death_subscriber const* const subscriber )
{
	subscriber->subscription_callback( );
}

// STATE[100%|DONE]
void base_player::on_player_death( )
{
	m_player_death_subscribers.for_each( call_player_death_subscriber_callback );
}

} // namespace survarium

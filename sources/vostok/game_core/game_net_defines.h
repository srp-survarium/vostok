////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_NET_DEFINES_H_INCLUDED
#define GAME_NET_DEFINES_H_INCLUDED

// the inline serialize bodies / value members below need the complete types
// (this header gained its first real includer with game's match_client.h)
#include <vostok/network_core/tcp_packet.h>
#include <vostok/game_core/game_mode_type.h>
#include <vostok/game_core/player_profile.h>

namespace vostok {
namespace network_core {
	class packet_reader;
	class udp_match_packet;
} // namespace network_core
} // namespace vostok

namespace survarium {

/*
// STATE[STUB]
// void survarium::`dynamic initializer for 'player_templates_count''()
void `dynamic initializer for 'player_templates_count''( )
{
	// FUNCTION BODY
	// <0x7d9ec0>|0x000|      :'512'	{
	// ******
}
*/

/* sushi@TODO: Those are in their own corresponding headers (move?)
player_profile::player_profile( )
skill_booster::skill_booster( )
void player_profile::deserialize( network_core::packet_reader& reader )
*/

struct relocate_item_descr {
	// STATE[0%|PARTIAL]: 6 appends (profile_id, item_id, item_dict_id, source/target slot u32,
	// amount u16) - order and member offsets (+0/+4/+8/+0xc/+0x10/+0x14) proven by the target
	// line table (0x97680, 121B: one row per append, L+1..L+6). UNPAIRED: the base never emits
	// this in-class COMDAT (the sole consumer, lobby_client::move_item, lives in the unbuilt
	// `game` module), and the target standalone is a frameless optimized LTCG fold (this=esi,
	// packet=eax->edi, custom convention - assembly_patterns.md "optimized LTCG COMDAT in an
	// /Od unit"), so an /Od anchor emission could not approach it anyway.
	// VERDICT: STRUCTURE UNVERIFIED (no base side) - body proven from the target line table;
	// access matches (target QBE = public const, ours public const), so the non-pairing is
	// genuine absence, not a mangled-join failure.
			void	serialize	( network_core::tcp_packet& packet ) const
			{
				packet.append( profile_id );
				packet.append( item_id );
				packet.append( item_dict_id );
				packet.append( source_slot_id );
				packet.append( target_slot_id );
				packet.append( amount );
			}
	inline	void	deserialize	( network_core::packet_reader& arg_0 ) { /* no source */ }

public:
	/* 0x0000 */	u32		profile_id;
	/* 0x0004 */	u32		item_id;
	/* 0x0008 */	u32		item_dict_id;
	/* 0x000c */	u32		source_slot_id;
	/* 0x0010 */	u32		target_slot_id;
	/* 0x0014 */	u16		amount;
}; // struct relocate_item_descr

STATIC_SIZE_ASSERT(relocate_item_descr, 0x18);


struct match_options {
			explicit	match_options	( ) {}

	inline	void		serialize		( network_core::udp_match_packet& arg_0 ) const { /* no source */ }
			void		deserialize		( network_core::packet_reader& reader ) {} // STATE[STUB]

public:
	/* 0x0000 */	player_profile		player_profiles[20];
	/* 0x2260 */	u32					match_id;
	/* 0x2264 */	u8					map_id;
	/* 0x2268 */	game_mode_type		match_mode_;
	/* 0x226c */	u8					players_count;
	/* 0x226d */	u8					respawn_time;
	/* 0x226e */	u16					match_time;
	/* 0x2270 */	float				wait_player_percent;
	/* 0x2274 */	u8					wait1_time;
	/* 0x2275 */	u8					wait2_time;
	/* 0x2276 */	u8					countdown_time;
	/* 0x2277 */	u8					victory_items_count;
	/* 0x2278 */	u8					received_players_count;
	/* 0x2279 */	char				map_name[32];
}; // struct match_options

STATIC_SIZE_ASSERT(match_options, 0x229C);


} // namespace survarium

#endif // #ifndef GAME_NET_DEFINES_H_INCLUDED

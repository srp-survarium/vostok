////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_NET_DEFINES_H_INCLUDED
#define GAME_NET_DEFINES_H_INCLUDED

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

enum game_mode_type {
	capture_enemy_base		= 0x0000,
	capture_neutral_base	= 0x0001,
	gather_victory_items	= 0x0002,
	invalid_game_mode		= 0x00ff,
};

/* sushi@TODO: Those are in their own corresponding headers (move?)
player_profile::player_profile( )
skill_booster::skill_booster( )
void player_profile::deserialize( network_core::packet_reader& reader )
*/

struct relocate_item_descr {
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

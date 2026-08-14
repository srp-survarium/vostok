////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_NET_DEFINES_H_INCLUDED
#define GAME_NET_DEFINES_H_INCLUDED

// the inline serialize bodies / value members below need the complete types
// (gained its first real includer with game's match_client.h)
#include <vostok/network_core/tcp_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/game_core/player_profile.h>
#include <vostok/c_array_functions.h>
// game_mode_type was extracted to its own header (the port's canonical home,
// also pulled by game's game_world_ui.h) - include it, don't redefine.
#include <vostok/game_core/game_mode_type.h>

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
}
*/

// per-slot wire encoding selector consulted by profile_slot::deserialize; file-static
// (one copy per includer, like player_templates_count's dynamic initializer).
// claude@NOTE: the per-slot mode VALUES below are a semantic guess (armor/weapon =
// condition, ammo = amount, quick = both); the table content is unrecoverable but is
// not part of the byte-match (deserialize only relocates against the symbol address).
static slot_serialize_mode_enum const slot_serialize_mode[ max_slots_count ] =
{
	serialize_just_condition_stack_values,	// helmet_slot
	serialize_just_condition_stack_values,	// mask_slot
	serialize_just_condition_stack_values,	// torso_slot
	serialize_just_condition_stack_values,	// back_slot
	serialize_just_condition_stack_values,	// pants_slot
	serialize_just_condition_stack_values,	// gloves_slot
	serialize_just_condition_stack_values,	// boots_slot
	serialize_just_condition_stack_values,	// weapon1_slot
	serialize_just_amount_values,			// ammo1_weapon1_slot
	serialize_just_amount_values,			// ammo2_weapon1_slot
	serialize_just_condition_stack_values,	// weapon2_slot
	serialize_just_amount_values,			// ammo1_weapon2_slot
	serialize_just_amount_values,			// ammo2_weapon2_slot
	serialize_both_values,					// quick_slot1
	serialize_both_values,					// quick_slot2
	serialize_both_values,					// quick_slot3
	serialize_both_values,					// quick_slot4
	serialize_both_values,					// quick_slot5
	serialize_both_values,					// quick_slot6
};

inline void player_profile::deserialize( network_core::packet_reader& reader )
{
	team		= (game_team_id)reader.r< u8 >( );
	is_local	= reader.r< u8 >( ) != 0;
	reader.r_string( profile_name );

	u16 const boosters_mask = reader.r< u16 >( );
	for ( u8 i = 0; i < array_size( boosters ); ++i )
	{
		if ( boosters_mask & ( 1 << i ) )
		{
			boosters[ i ].id	= reader.r< u8 >( );
			boosters[ i ].value	= reader.r< float >( );
		}
		else
		{
			boosters[ i ].id	= 0;
			boosters[ i ].value	= 0;
		}
	}

	while ( !reader.eof( ) )
	{
		profile_slot_enum const slot = (profile_slot_enum)reader.r< u8 >( );
		slots[ slot ].deserialize( reader, slot_serialize_mode[ slot ] );
	}
}

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
			explicit	match_options	( ) :
				match_id				( u32( -1 ) ),
				match_mode_			( invalid_game_mode ),
				players_count			( u8( -1 ) ),
				respawn_time			( 0 ),
				match_time				( 0 ),
				wait_player_percent		( 0.f ),
				wait1_time				( 0 ),
				wait2_time				( 0 ),
				countdown_time			( 0 ),
				victory_items_count		( 0 ),
				received_players_count	( u8( -1 ) )
			{
				map_name[ 0 ] = 0;
			}

	inline	void		serialize		( network_core::udp_match_packet& arg_0 ) const { /* no source */ }
			void		deserialize		( network_core::packet_reader& reader )
			{
				map_id				= reader.r< u8 >( );
				reader.r_string		( map_name );
				match_mode_			= (game_mode_type)reader.r< u8 >( );
				players_count		= reader.r< u8 >( );
				victory_items_count	= reader.r< u8 >( );
				respawn_time		= reader.r< u8 >( ); match_time = reader.r< u16 >( );

				match_id			= 0xFFFFFFFF;
				received_players_count	= 0xFF;
			}

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

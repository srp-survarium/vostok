// SPDX-License-Identifier: GPL-3.0-or-later

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

static u16 const player_templates[11][13] =
{
	{  0,  0, 29, 0, 28, 41, 36, 12, 51, 0, 13,  7, 0 },
	{  0,  0, 31, 0, 45, 25, 37, 16, 22, 0,  0,  0, 0 },
	{  0, 43, 34, 9, 46, 41, 38, 14, 52, 0,  0,  0, 0 },
	{  0,  0, 48, 0, 47, 42, 39, 19, 53, 0,  0,  0, 0 },
	{  0,  0, 29, 0, 28, 42, 24, 17, 22, 0,  0,  0, 0 },
	{ 27,  0, 32, 0, 28, 25, 35, 55, 53, 0,  0,  0, 0 },
	{  0, 43, 33, 9, 44, 40, 36, 13,  7, 0, 56, 20, 0 },
	{  0,  0, 31, 0, 47, 25, 37, 15, 22, 0,  0,  0, 0 },
	{  0,  0, 29, 0, 44, 42, 36, 56, 20, 0,  0,  0, 0 },
	{  0,  0, 48, 0, 28, 40, 24, 17, 22, 0,  0,  0, 0 },
	{  0,  0, 33, 0, 46, 41, 37, 64, 70, 0,  0,  0, 0 },
};

static u32 const player_templates_count = array_size( player_templates );

static slot_serialize_mode_enum slot_serialize_mode[ max_slots_count ] =
{
	serialize_just_condition_stack_values,
	serialize_just_condition_stack_values,
	serialize_just_condition_stack_values,
	serialize_just_condition_stack_values,
	serialize_just_condition_stack_values,
	serialize_just_condition_stack_values,
	serialize_just_condition_stack_values,
	serialize_just_condition_stack_values,
	serialize_both_values,
	serialize_both_values,
	serialize_just_condition_stack_values,
	serialize_both_values,
	serialize_both_values,
	serialize_both_values,
	serialize_both_values,
	serialize_both_values,
	serialize_both_values,
	serialize_both_values,
	serialize_both_values,
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
public:
	/* 0x0000 */	u32		profile_id;
	/* 0x0004 */	u32		item_id;
	/* 0x0008 */	u32		item_dict_id;
	/* 0x000c */	u32		source_slot_id;
	/* 0x0010 */	u32		target_slot_id;
	/* 0x0014 */	u16		amount;

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
}; // struct relocate_item_descr

STATIC_SIZE_ASSERT(relocate_item_descr, 0x18);


struct match_options {
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
}; // struct match_options

STATIC_SIZE_ASSERT(match_options, 0x229C);


} // namespace survarium

#endif // #ifndef GAME_NET_DEFINES_H_INCLUDED

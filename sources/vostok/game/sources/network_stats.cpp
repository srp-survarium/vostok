////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_stats.h"

#include "stats_graph.h"
#include "flash_text_manager.h"
#include "game_memory.h"		// NEW / DELETE ( survarium::g_allocator )
#include <vostok/network_core/udp_match_stats.h>
#include <vostok/math_constants.h>

namespace survarium {

// claude@NOTE: the network_stats HUD-row cluster (stats_stream/stats_row create +
// set_text + dtors) drives a column of flash_text labels via create_text and the
// flash_text accessors. Structure (statement count/order, create_text + set_*
// sequence, named-local set, allocator routing) is faithful; reachability is held by
// anchor_game_clients.cpp. The graph alloc/free now matches the target: NEW/DELETE
// route through doug_lea_allocator malloc_impl / vostok_mspace_free (was global
// operator new/delete - the dtors jumped 17/20% -> 37/56% on that fix alone).
// The DOMINANT remaining wall is CROSS-MODULE: three scaleform helpers are STUBs in
// vostok/scaleform/sources/movie.cpp - flash_text::set_visible, flash_text::set_color
// and flash_text_manager::destroy_text - so they inline to nothing on our base while
// the target inlines real GFx DrawText vtable dispatch. That empties the per-text
// set_visible/set_color line records in create (folds them into the next set_position
// statement, the SIZE +0x40 rows + TRGT_ONLY pairs) and drops all four destroy_text
// statements from each dtor (the TRGT_ONLY rows). Bodying those three movie.cpp stubs
// is the scaleform module's matching task; until then create/dtors are capped here.
// set_text (80/85%) has no allocator/destroy_text dependency: its residual is the
// inline scheduling of the (already-matched) flash_text::set_text + stats_graph
// add_value/cumulative_* helpers - STRUCTURE MATCH, capped.

stats_stream::~stats_stream( )
{
	if( text_manager )
	{
		DELETE( bytes_per_second_graph );
		DELETE( graph );

		text_manager->destroy_text( count_per_second );
		text_manager->destroy_text( bits_per_second );
		text_manager->destroy_text( bytes );
		text_manager->destroy_text( count );
	}
}

void stats_stream::create(
	flash_text_manager&		text_manager_in,
	const float				start_width,
	const float				start_height,
	const float				column0_width,
	const float				column1_width,
	const float				column2_width,
	math::color const&		color
)
{
	text_manager	= &text_manager_in;

	count			= text_manager_in.create_text( "" );
	count.set_visible	( true );
	count.set_color		( color.get_R( ), color.get_G( ), color.get_B( ), color.get_A( ) );
	count.set_position	( start_width, start_height );

	bytes			= text_manager_in.create_text( "" );
	bytes.set_visible	( true );
	bytes.set_color		( color.get_R( ), color.get_G( ), color.get_B( ), color.get_A( ) );
	bytes.set_position	( start_width + column0_width, start_height );

	bits_per_second	= text_manager_in.create_text( "" );
	bits_per_second.set_visible		( true );
	bits_per_second.set_color		( color.get_R( ), color.get_G( ), color.get_B( ), color.get_A( ) );
	bits_per_second.set_position	( start_width + column0_width + column1_width, start_height );

	count_per_second	= text_manager_in.create_text( "" );
	count_per_second.set_visible	( true );
	count_per_second.set_color		( color.get_R( ), color.get_G( ), color.get_B( ), color.get_A( ) );
	count_per_second.set_position	( start_width + column0_width + column1_width + column2_width, start_height );

	graph					= NEW( stats_graph )( 3.f, math::infinity, 256.f, 1024.f, 0xFF00FF00 );
	bytes_per_second_graph	= NEW( stats_graph )( 3.f, math::infinity, 2048.f, 8192.f, 0xFF00FF00 );
}

void stats_stream::set_text(
	const u32		current_time_in_ms,
	network_core::udp_match_items_stats const&	new_stats,
	network_core::udp_match_items_stats const&	previous_stats
)
{
	graph->add_value( current_time_in_ms * math::epsilon_3, (float)( new_stats.count - previous_stats.count ) );
	bytes_per_second_graph->add_value( current_time_in_ms * math::epsilon_3, (float)( new_stats.bytes - previous_stats.bytes ) );

	char	text[ 256 ];

	sprintf_big_number	( text, new_stats.count, 1000, "", "" );
	count.set_text		( text );

	sprintf_big_number	( text, new_stats.bytes, 1024, "b", "bytes" );
	bytes.set_text		( text );

	if( graph->cumulative_time( ) > math::epsilon_5 )
	{
		sprintf_big_number		( text, (u32)math::floor( graph->cumulative_value( ) / graph->cumulative_time( ) ), "" );
		count_per_second.set_text	( text );
	}

	if( bytes_per_second_graph->cumulative_time( ) > math::epsilon_5 )
	{
		vostok::sprintf< 256 >	( text, "%4.1f Kbits", bytes_per_second_graph->cumulative_value( ) / bytes_per_second_graph->cumulative_time( ) * ( 1.f / 128.f ) );
		bits_per_second.set_text	( text );
	}
}

stats_row::~stats_row( )
{
	if( text_manager )
	{
		DELETE( data_bytes_per_second_graph );

		text_manager->destroy_text( messages_per_second );
		text_manager->destroy_text( data_bits_per_message );
		text_manager->destroy_text( data_bits_per_second );
		text_manager->destroy_text( data_bytes );
		text_manager->destroy_text( caption );
	}
}

void stats_row::create(
	flash_text_manager&		text_manager_in,
	pcstr const				caption_string,
	const float				start_width,
	const float				start_height,
	const float				caption_width,
	const float				column0_width,
	const float				column1_width,
	const float				column2_width,
	const float				column3_width,
	math::color const&		color
)
{
	packets.create	( text_manager_in, 325.f, start_height, column0_width, column1_width, column2_width, color );
	messages.create	( text_manager_in, 665.f, start_height, column0_width, column1_width, column2_width, color );

	caption			= text_manager_in.create_text( caption_string );
	caption.set_visible		( true );
	caption.set_color		( color.get_R( ), color.get_G( ), color.get_B( ), color.get_A( ) );
	caption.set_position	( 150.f, start_height );

	data_bytes		= text_manager_in.create_text( "" );
	data_bytes.set_visible	( true );
	data_bytes.set_color	( color.get_R( ), color.get_G( ), color.get_B( ), color.get_A( ) );
	data_bytes.set_position	( 1005.f, start_height );

	data_bits_per_second	= text_manager_in.create_text( "" );
	data_bits_per_second.set_visible	( true );
	data_bits_per_second.set_color		( color.get_R( ), color.get_G( ), color.get_B( ), color.get_A( ) );
	data_bits_per_second.set_position	( 1105.f, start_height );

	data_bits_per_message	= text_manager_in.create_text( "" );
	data_bits_per_message.set_visible	( true );
	data_bits_per_message.set_color		( color.get_R( ), color.get_G( ), color.get_B( ), color.get_A( ) );
	data_bits_per_message.set_position	( 1205.f, start_height );

	messages_per_second		= text_manager_in.create_text( "" );
	messages_per_second.set_visible		( true );
	messages_per_second.set_color		( color.get_R( ), color.get_G( ), color.get_B( ), color.get_A( ) );
	messages_per_second.set_position	( 1305.f, start_height );

	data_bytes_per_second_graph	= NEW( stats_graph )( 3.f, math::infinity, 2048.f, 8192.f, 0xFF00FF00 );
}

void stats_row::set_text(
	const u32		current_time_in_ms,
	network_core::udp_match_stream_stats const&	new_stats,
	network_core::udp_match_stream_stats const&	previous_stats
)
{
	packets.set_text	( current_time_in_ms, new_stats.packets, previous_stats.packets );
	messages.set_text	( current_time_in_ms, new_stats.messages, previous_stats.messages );

	data_bytes_per_second_graph->add_value(
		current_time_in_ms * math::epsilon_3,
		(float)( new_stats.data_bytes - previous_stats.data_bytes ) );

	char	text[ 256 ];

	sprintf_big_number	( text, new_stats.data_bytes, 1024, "b", "bytes" );
	data_bytes.set_text	( text );

	if( data_bytes_per_second_graph->cumulative_time( ) > math::epsilon_5 )
	{
		vostok::sprintf< 256 >		( text, "%4.1f Kbits", data_bytes_per_second_graph->cumulative_value( ) / data_bytes_per_second_graph->cumulative_time( ) * ( 1.f / 128.f ) );
		data_bits_per_second.set_text	( text );
	}

	if( messages.graph->cumulative_count( ) != 0 )
	{
		vostok::sprintf< 256 >			( text, "%5.1f bytes", data_bytes_per_second_graph->cumulative_value( ) / (float)messages.graph->cumulative_count( ) );
		data_bits_per_message.set_text	( text );
	}

	if( packets.graph->cumulative_value( ) > math::epsilon_5 )
	{
		vostok::sprintf< 256 >			( text, "%5.2f mpp", messages.graph->cumulative_value( ) / packets.graph->cumulative_value( ) );
		messages_per_second.set_text	( text );
	}
}

} // namespace survarium

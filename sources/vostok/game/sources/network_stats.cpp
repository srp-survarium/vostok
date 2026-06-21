////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_stats.h"

#include "stats_graph.h"
#include "flash_text_manager.h"
#include <vostok/network_core/udp_match_stats.h>
#include <vostok/math_constants.h>

namespace survarium {

// claude@NOTE: the whole network_stats HUD-row cluster (stats_stream/stats_row
// create + set_text + dtors) is parked on the flash-glue /Od wall. Each builds /
// updates a column of flash_text labels through flash_text_manager::create_text
// and the flash_text accessors (set_visible / set_color / set_position /
// set_text). Those accessors are now DECLARED out-of-line in the game-side
// flash_text.h / flash_text_manager.h (their real GFx::DrawText bodies live in the
// scaleform glue, movie.cpp) so we resolve to that single definition - but at /Od
// the target INLINES the glue while we emit a `call`, so each statement is far
// smaller here (the inline-vs-call wall that holds scaleform/value.cpp glue at
// 14-34%). stats_graph::cumulative_time and the inline new+ctor of stats_graph are
// likewise inlined in the target but out-of-line here (and `delete` routes through
// global operator delete instead of the engine allocator's mspace_free). The
// column-width params also fold to the caller's literal 100 under LTCG. Structure
// (statement count/order, create_text + set_* sequence, named-local set) is
// recovered and faithful; the bytes are walled until the flash_text glue is matched
// in an optimized TU. Reachability is held by anchor_game_clients.cpp. The
// stats_row dtor reports `unpaired` in objdiff = ICF fold-rep churn on the inline
// COMDAT (its structure pairs cleanly via two-sided structure-diff), not a gap.

stats_stream::~stats_stream( )
{
	if( text_manager )
	{
		delete bytes_per_second_graph;
		delete graph;

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

	graph					= new stats_graph( 3.f, math::infinity, 256.f, 1024.f, 0xFF00FF00 );
	bytes_per_second_graph	= new stats_graph( 3.f, math::infinity, 2048.f, 8192.f, 0xFF00FF00 );
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
		delete data_bytes_per_second_graph;

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

	data_bytes_per_second_graph	= new stats_graph( 3.f, math::infinity, 2048.f, 8192.f, 0xFF00FF00 );
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

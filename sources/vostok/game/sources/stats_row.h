// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef STATS_ROW_H_INCLUDED
#define STATS_ROW_H_INCLUDED

#include "flash_text.h"
#include "stats_stream.h"

namespace vostok {
namespace network_core {
	struct udp_match_stream_stats;
} // namespace network_core
} // namespace vostok

namespace survarium {

class stats_graph;
struct flash_text_manager;

struct stats_row : private boost::noncopyable {
	inline			stats_row	( ) :
		data_bytes_per_second_graph	( NULL ),
		data_bytes_per_message_graph	( NULL ),
		text_manager					( NULL )
	{
	}
					~stats_row	( );

			void	create		(
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
					);

	inline	void	set_visible	( const bool value )
	{
		caption.set_visible					( value );
		data_bytes.set_visible				( value );
		data_bits_per_second.set_visible	( value );
		data_bits_per_message.set_visible	( value );
		messages_per_second.set_visible		( value );

		packets.set_visible					( value );
		messages.set_visible				( value );
	}
			void	set_text	(
						const u32		current_time_in_ms,
						network_core::udp_match_stream_stats const&	new_stats,
						network_core::udp_match_stream_stats const&	previous_stats
					);

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	flash_text				caption;
	/* 0x000c */	flash_text				data_bytes;
	/* 0x0018 */	flash_text				data_bits_per_second;
	/* 0x0024 */	flash_text				data_bits_per_message;
	/* 0x0030 */	flash_text				messages_per_second;
	/* 0x003c */	stats_stream			packets;
	/* 0x0078 */	stats_stream			messages;
	/* 0x00b4 */	stats_graph*			data_bytes_per_second_graph;
	/* 0x00b8 */	stats_graph*			data_bytes_per_message_graph;
	/* 0x00bc */	flash_text_manager*		text_manager;
}; // struct stats_row

STATIC_SIZE_ASSERT(stats_row, 0xC0);

} // namespace survarium

#endif // #ifndef STATS_ROW_H_INCLUDED

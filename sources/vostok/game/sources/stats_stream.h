// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef STATS_STREAM_H_INCLUDED
#define STATS_STREAM_H_INCLUDED

#include "flash_text.h"

namespace vostok {
namespace network_core {
	struct udp_match_items_stats;
} // namespace network_core
} // namespace vostok

namespace survarium {

class stats_graph;
struct flash_text_manager;

struct stats_stream : private boost::noncopyable {
	inline			stats_stream	( ) :
		graph					( NULL ),
		bytes_per_second_graph	( NULL ),
		text_manager			( NULL )
	{
	}
					~stats_stream	( );

			void	create			(
						flash_text_manager&		text_manager_in,
						const float				start_width,
						const float				start_height,
						const float				column0_width,
						const float				column1_width,
						const float				column2_width,
						math::color const&		color
					);

	inline	void	set_visible		( const bool value )
	{
		count.set_visible			( value );
		bytes.set_visible			( value );
		bits_per_second.set_visible	( value );
		count_per_second.set_visible( value );
	}
			void	set_text		(
						const u32		current_time_in_ms,
						network_core::udp_match_items_stats const&	new_stats,
						network_core::udp_match_items_stats const&	previous_stats
					);

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	flash_text				count;
	/* 0x000c */	flash_text				bytes;
	/* 0x0018 */	flash_text				bits_per_second;
	/* 0x0024 */	flash_text				count_per_second;
	/* 0x0030 */	stats_graph*			graph;
	/* 0x0034 */	stats_graph*			bytes_per_second_graph;
	/* 0x0038 */	flash_text_manager*		text_manager;
}; // struct stats_stream

STATIC_SIZE_ASSERT(stats_stream, 0x3C);

} // namespace survarium

#endif // #ifndef STATS_STREAM_H_INCLUDED

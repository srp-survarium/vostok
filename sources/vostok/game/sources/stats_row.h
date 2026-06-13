////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STATS_ROW_H_INCLUDED
#define STATS_ROW_H_INCLUDED

#include <vostok/scaleform/sources/flash_text.h>
#include "stats_stream.h"

namespace vostok {
namespace network_core {
	struct udp_match_stream_stats;
} // namespace network_core
} // namespace vostok

namespace survarium {

class stats_graph;
struct flash_text_manager;

struct stats_row : public boost::noncopyable {
	// STATE[STUB]
	// addressed carcass from the network_stats.h dump (this type's original home)
	inline			stats_row	( )
	{
		// FUNCTION BODY[0x91da0]: 0
		// <0x91da0>|0x000|+0x0c1:'69'	{
		// <0x91e61>|0x0c1|      :'70'	}
		// ******
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

	// STATE[STUB]
	// addressed carcass from the network_stats.h dump; PDB names the param via a
	// `const bool value` local
	inline	void	set_visible	( const bool arg_0 )
	{
		// LOCALS
		// const bool 						value
		// ******

		// FUNCTION BODY[0x91e70]: 7
		// <0x91e72>|0x002|+0x02f:'88'
		// <0x91ea1>|0x031|+0x021:'89'
		// <0x91ec2>|0x052|+0x01d:'90'
		// <0x91edf>|0x06f|+0x01d:'91'
		// <0x91efc>|0x08c|+0x01d:'92'
		// <0x91f19>|0x0a9|+0x009:'93'
		// <0x91f22>|0x0b2|+0x00c:'94'
		// ******
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

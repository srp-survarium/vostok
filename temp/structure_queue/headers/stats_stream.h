////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STATS_STREAM_H_INCLUDED
#define STATS_STREAM_H_INCLUDED

/* INCLUDES */
class survarium::stats_graph;
struct survarium::flash_text;
struct survarium::flash_text_manager;

/* FORWARD REFS */
class vostok::math::color;
class vostok::network_core::udp_match_items_stats;

namespace survarium {

struct stats_stream : public boost::noncopyable {
	inline			stats_stream	( ) { /* no source */ }
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

	inline	void	set_visible		( const bool value ) { /* no source */ }
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

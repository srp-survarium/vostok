////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_stats.h"

#include <vostok/network_core/udp_match_stats.h>

namespace survarium {

// claude@NOTE: the whole network_stats HUD-row cluster (stats_stream/stats_row
// ctors-via-create + set_text + dtors) is parked on the flash-glue /Od wall. Each
// builds/updates a column of flash_text labels through flash_text_manager and the
// flash_text accessors (set_position / set_font_size / set_color / set_alignment /
// set_text / set_visible). Those accessors are empty `{ /* no source */ }` inlines in
// our flash_text.h (the real out-of-line bodies live in the scaleform glue module,
// not in this tree), and the flash_text_manager.create_text calls inline in the
// whole-program target but emit out-of-line calls in our /Od build - the same wall
// that holds scaleform/value.cpp glue at 14-34%. The set_text bodies additionally
// format udp_match_*_stats deltas via sprintf_big_number (network_stats.h inlines).
// Structure is recoverable but bytes are walled until the flash_text glue is matched
// in an optimized TU; the stats_row/stats_stream carcass objects are kept reachable
// by anchor_game_clients.cpp.

// STATE[STUB]
 stats_stream::~stats_stream( )
{
}

// STATE[STUB]
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
}

// STATE[STUB]
void stats_stream::set_text(
	const u32		current_time_in_ms,
	network_core::udp_match_items_stats const&	new_stats,
	network_core::udp_match_items_stats const&	previous_stats
)
{
}

// STATE[STUB]
 stats_row::~stats_row( )
{
}

// STATE[STUB]
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
}

// STATE[STUB]
void stats_row::set_text(
	const u32		current_time_in_ms,
	network_core::udp_match_stream_stats const&	new_stats,
	network_core::udp_match_stream_stats const&	previous_stats
)
{
}

} // namespace survarium

////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_STATS_H_INCLUDED
#define NETWORK_STATS_H_INCLUDED

// the original network_stats.h defined stats_stream/stats_row themselves (the
// addressed inline carcasses carry its line numbers); batch 6 landed them as
// split headers - this header keeps the compiland's include surface plus the
// helpers below
#include "stats_row.h"
#include "stats_stream.h"

namespace survarium {

// PDB spells the first param char[256]&
inline void sprintf_big_number( char ( &text )[256], const u32 value, pcstr const postfix )
{
	if( value < 1000 )
		vostok::sprintf( text, "%3d %s", value, postfix );
	else if( value < 1000000 )
		vostok::sprintf( text, "%3d %3d %s", value / 1000, value % 1000, postfix );
	else if( value < 1000000000 )
		vostok::sprintf( text, "%3d %3d %3d %s", value / 1000000, ( value % 1000000 ) / 1000, value % 1000, postfix );
	else
		vostok::sprintf( text, "%3d %3d %3d %3d %s", value / 1000000000, ( value % 1000000000 ) / 1000000, ( value % 1000000 ) / 1000, value % 1000, postfix );
}

inline void sprintf_big_number(
	char ( &text )[256],
	const u32		value,
	const u32		divider,
	pcstr const		simple_postfix,
	pcstr const		complex_postfix
)
{
	if( value < divider )
		sprintf_big_number( text, value, simple_postfix );
	else if( value < divider * divider )
		vostok::sprintf( text, "%5.1f K%s", (float)value / divider, complex_postfix );
	else if( value < divider * divider * divider )
		vostok::sprintf( text, "%5.1f M%s", (float)value / ( divider * divider ), complex_postfix );
	else
		vostok::sprintf( text, "%5.1f G%s", (float)value / ( divider * divider * divider ), complex_postfix );
}

} // namespace survarium

#endif // #ifndef NETWORK_STATS_H_INCLUDED

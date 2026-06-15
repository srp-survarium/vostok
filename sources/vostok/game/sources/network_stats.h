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

// STATE[STUB]
// PDB spells the first param char[256]&
inline void sprintf_big_number( char ( &text )[256], const u32 value, pcstr const postfix )
{
	// FUNCTION BODY[0xa7440]: 8
	// <0xa7440>|0x000|+0x008:'116'	{
	// <0xa7448>|0x008|+0x008:'117'
	// <0xa7450>|0x010|+0x014:'118'
	// <0xa7464>|0x024|+0x008:'119'
	// <0xa746c>|0x02c|+0x02a:'120'
	// <0>
	// <0xa7496>|0x056|+0x053:'122'
	// <0>
	// <0xa74e9>|0x0a9|-0x086:'124'
	// <0xa7463>|0x023|+0x031:'125'
	// <0xa7494>|0x054|+0x054:'125'
	// <0xa74e8>|0x0a8|+0x06f:'125'
	// <0xa7557>|0x117|      :'125'	}
	// ******
}

// STATE[STUB]
inline void sprintf_big_number(
	char ( &text )[256],
	const u32		value,
	const u32		divider,
	pcstr const		simple_postfix,
	pcstr const		complex_postfix
)
{
	// FUNCTION BODY[0xa7560]: 8
	// <0xa7560>|0x000|+0x006:'128'	{
	// <0xa7566>|0x006|+0x004:'129'
	// <0xa756a>|0x00a|+0x010:'130'
	// <0xa757a>|0x01a|+0x009:'131'
	// <0xa7583>|0x023|+0x03d:'132'
	// <0xa75c0>|0x060|+0x007:'133'
	// <0xa75c7>|0x067|-0x012:'134'
	// <0>
	// <0xa75b5>|0x055|+0x048:'136'
	// <0xa75fd>|0x09d|+0x00b:'136'
	// <0xa7608>|0x0a8|-0x091:'136'
	// <0xa7577>|0x017|+0x046:'137'
	// <0xa75bd>|0x05d|+0x048:'137'
	// <0xa7605>|0x0a5|+0x043:'137'
	// <0xa7648>|0x0e8|      :'137'	}
	// ******
}

} // namespace survarium

#endif // #ifndef NETWORK_STATS_H_INCLUDED

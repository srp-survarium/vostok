////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STATS_GRAPH_STATS_VALUE_H_INCLUDED
#define STATS_GRAPH_STATS_VALUE_H_INCLUDED

/* INCLUDES */
struct survarium::stats_graph::stats_value;

namespace survarium {

struct stats_graph::stats_value {

public:
	/* 0x0000 */	stats_graph::stats_value*		next;
	/* 0x0004 */	stats_graph::stats_value*		previous;
	/* 0x0008 */	float							time;
	/* 0x000c */	float							value;
}; // struct stats_graph::stats_value

STATIC_SIZE_ASSERT(stats_graph::stats_value, 0x10);

} // namespace survarium

#endif // #ifndef STATS_GRAPH_STATS_VALUE_H_INCLUDED

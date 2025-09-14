////////////////////////////////////////////////////////////////////////////
//	Created		: 15.07.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_AI_SEARCH_PATH_HEURISTICS_PLANNER_BACKWARD_INLINE_H_INCLUDED
#define VOSTOK_AI_SEARCH_PATH_HEURISTICS_PLANNER_BACKWARD_INLINE_H_INCLUDED

namespace vostok {
namespace ai {
namespace path_heuristics {

#define HEURISTICS		path_heuristics::planner_backward::impl

inline HEURISTICS::impl	( graph_type& graph ) :
	super				( graph )
{
}

#undef HEURISTICS

} // namespace path_heuristics
} // namespace ai
} // namespace vostok

#endif // #ifndef VOSTOK_AI_SEARCH_PATH_HEURISTICS_PLANNER_BACKWARD_INLINE_H_INCLUDED
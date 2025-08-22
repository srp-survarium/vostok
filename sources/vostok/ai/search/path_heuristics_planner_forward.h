////////////////////////////////////////////////////////////////////////////
//	Created		: 15.07.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_AI_SEARCH_PATH_HEURISTICS_PLANNER_FORWARD_H_INCLUDED
#define VOSTOK_AI_SEARCH_PATH_HEURISTICS_PLANNER_FORWARD_H_INCLUDED

#include <vostok/ai/search/path_heuristrics_planner_base.h>

namespace vostok {
namespace ai {
namespace path_heuristics {

struct planner_forward : public propositional_planner_base
{
	typedef propositional_planner_base			super;

	class impl : public super::impl
	{
		typedef super::impl			super;

	public:
		inline						impl		( graph_type& graph );
				
				edge_weight_type	estimate	(
										vertex_id_type const* current_vertex_id_ptr,
										vertex_id_type const& neighbour_vertex_id
									) const;

	private:
				edge_weight_type	estimate_impl	(
										vertex_id_type const& vertex_id,
										planning::world_state::properties_type::const_iterator it_target_begin,
										planning::world_state::properties_type::const_iterator it_target_end
									) const;
	}; // class impl
}; // struct planner_forward

} // namespace path_heuristics
} // namespace ai
} // namespace vostok

#include <vostok/ai/search/path_heuristics_planner_forward_inline.h>

#endif // #ifndef VOSTOK_AI_SEARCH_PATH_HEURISTICS_PLANNER_FORWARD_H_INCLUDED

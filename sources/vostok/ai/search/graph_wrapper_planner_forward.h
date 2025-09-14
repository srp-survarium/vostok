////////////////////////////////////////////////////////////////////////////
//	Created		: 15.07.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_AI_SEARCH_GRAPH_WRAPPER_PLANNER_FORWARD_H_INCLUDED
#define VOSTOK_AI_SEARCH_GRAPH_WRAPPER_PLANNER_FORWARD_H_INCLUDED

#include <vostok/ai/search/graph_wrapper_planner_base.h>

namespace vostok {
namespace ai {
namespace graph_wrapper {

struct planner_forward : private propositional_planner_base
{
	typedef propositional_planner_base			super;

	class impl : public super::impl
	{
	public:
		inline						impl		( graph_type* graph );
		inline vertex_id_type const& vertex_id	( vertex_id_type const& current_vertex_id, const_edge_iterator const& iterator );
	
	private:
		typedef super::impl			super;

	private:
		typedef vertex_id_type::properties_type	properties_type;
		typedef properties_type::const_iterator	const_iterator;

	private:
				bool				applicable	(
										properties_type const& search_state,
										properties_type const& preconditions,
										properties_type const& start_state
									);
				void				apply		(
										properties_type const& search_state,
										properties_type const& effects,
										properties_type const& start_state
									);
	}; // class impl
}; // struct planner_forward

} // namespace graph_wrapper
} // namespace ai
} // namespace vostok

#include <vostok/ai/search/graph_wrapper_planner_forward_inline.h>

#endif // #ifndef VOSTOK_AI_SEARCH_GRAPH_WRAPPER_PLANNER_FORWARD_H_INCLUDED
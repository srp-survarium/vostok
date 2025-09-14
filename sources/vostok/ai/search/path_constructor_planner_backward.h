////////////////////////////////////////////////////////////////////////////
//	Created		: 15.07.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_AI_SEARCH_PATH_CONSTRUCTOR_PLANNER_BACKWARD_H_INCLUDED
#define VOSTOK_AI_SEARCH_PATH_CONSTRUCTOR_PLANNER_BACKWARD_H_INCLUDED

#include <vostok/ai/search/path_constructor_planner_base.h>

namespace vostok {
namespace ai {
namespace path_constructor {

struct planner_backward : public propositional_planner_base
{
	typedef propositional_planner_base	super;

	template < typename VertexType, typename PathType >
	class impl : public super::impl< VertexType, PathType >
	{
	public:
		inline		impl			( PathType* path = 0 );
		inline void	construct_path	( VertexType& best );

	private:
		typedef typename propositional_planner_base::impl< VertexType, PathType >	super;
	}; // class impl
}; // struct planner_backward

} // namespace path_constructor
} // namespace ai
} // namespace vostok

#include <vostok/ai/search/path_constructor_planner_backward_inline.h>

#endif // #ifndef VOSTOK_AI_SEARCH_PATH_CONSTRUCTOR_PLANNER_BACKWARD_H_INCLUDED
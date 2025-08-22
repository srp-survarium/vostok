////////////////////////////////////////////////////////////////////////////
//	Created		: 13.07.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_AI_SEARCH_OPERATOR_HOLDER_INLINE_H_INCLUDED
#define VOSTOK_AI_SEARCH_OPERATOR_HOLDER_INLINE_H_INCLUDED

namespace vostok {
namespace ai {
namespace planning {

#define HOLDER	planning::operator_holder

inline HOLDER::operator_holder	( propositional_planner& total_order_planner ) :
	m_planner					( total_order_planner )
{
}

inline HOLDER::objects_type const& HOLDER::objects	( ) const
{
	return						m_objects;
}

inline void HOLDER::remove		( operator_id_type const& operator_id )
{
	remove_impl					( operator_id, true );
}

#undef HOLDER

} // namespace planning
} // namespace ai
} // namespace vostok

#endif // #ifndef VOSTOK_AI_SEARCH_OPERATOR_HOLDER_INLINE_H_INCLUDED
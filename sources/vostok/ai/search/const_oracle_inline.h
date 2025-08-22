////////////////////////////////////////////////////////////////////////////
//	Created		: 18.07.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_AI_SEARCH_CONST_ORACLE_INLINE_H_INCLUDED
#define VOSTOK_AI_SEARCH_CONST_ORACLE_INLINE_H_INCLUDED

namespace vostok {
namespace ai {
namespace planning {

#define ORACLE				planning::const_oracle

inline ORACLE::const_oracle	( property_value_type const& value, pcstr id ) : 
	super					( id )
{
	m_value					= value;
}

#undef ORACLE

} // namespace planning
} // namespace ai
} // namespace vostok

#endif // #ifndef VOSTOK_AI_SEARCH_CONST_ORACLE_INLINE_H_INCLUDED
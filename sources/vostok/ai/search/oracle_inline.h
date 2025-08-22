////////////////////////////////////////////////////////////////////////////
//	Created		: 18.07.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_AI_SEARCH_ORACLE_INLINE_H_INCLUDED
#define VOSTOK_AI_SEARCH_ORACLE_INLINE_H_INCLUDED

namespace vostok {
namespace ai {
namespace planning {

#define PROPERTY_ORACLE			planning::oracle

inline PROPERTY_ORACLE::oracle	( pcstr id ) :
	m_id						( id )
{
}

inline pcstr PROPERTY_ORACLE::id( ) const
{
	return	m_id.c_str			( );
}

#undef PROPERTY_ORACLE

} // namespace planning
} // namespace ai
} // namespace vostok

#endif // #ifndef VOSTOK_AI_SEARCH_ORACLE_INLINE_H_INCLUDED
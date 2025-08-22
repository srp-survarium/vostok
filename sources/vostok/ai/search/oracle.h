////////////////////////////////////////////////////////////////////////////
//	Created		: 18.07.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_AI_SEARCH_ORACLE_H_INCLUDED
#define VOSTOK_AI_SEARCH_ORACLE_H_INCLUDED

#include <vostok/ai/search/base_types.h>

namespace vostok {
namespace ai {
namespace planning {

class oracle
{
protected:
	typedef property_value_type		property_value_type;

public:
	inline							oracle	( pcstr id );
	virtual							~oracle	( );
	
	virtual	property_value_type		value	( );
	inline	pcstr					id		( ) const;

public:
	fixed_string< 32 >				m_id;
};

} // namespace planning
} // namespace ai
} // namespace vostok

#include <vostok/ai/search/oracle_inline.h>

#endif // #ifndef VOSTOK_AI_SEARCH_ORACLE_H_INCLUDED
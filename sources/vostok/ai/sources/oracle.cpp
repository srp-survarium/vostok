////////////////////////////////////////////////////////////////////////////
//	Created		: 18.07.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include <vostok/ai/search/oracle.h>

namespace vostok {
namespace ai {
namespace planning {

oracle::~oracle	( )
{
}

oracle::property_value_type oracle::value	( )
{
	NODEFAULT								( );
#ifdef DEBUG
	return									property_value_type( 0 );
#endif // #ifdef DEBUG
}

} // namespace planning
} // namespace ai
} // namespace vostok
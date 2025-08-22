////////////////////////////////////////////////////////////////////////////
//	Created		: 21.12.2009
//	Author		: Alexander Maniluk
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#if !VOSTOK_PLATFORM_PS3
#include "boost_api.h"

namespace boost
{
#if 0//!VOSTOK_STATIC_LIBRARIES
	void throw_exception(std::exception const & e)
	{
		FATAL("boost exception caught: %s", e.what());
	}
#endif // #if !VOSTOK_STATIC_LIBRARIES
	void throw_exception(std::bad_alloc const & e)
	{
		VOSTOK_UNREFERENCED_PARAMETER	( e );
		FATAL("boost exception caught: %s", e.what());
	}

} // namespace boost



#endif // #if !VOSTOK_PLATFORM_PS3


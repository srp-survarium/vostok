////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/debug/debug.h>
#include <vostok/debug/macros.h>
#include <vostok/linkage_helper.h>
#include "utils.h"

#if VOSTOK_PLATFORM_WINDOWS || VOSTOK_PLATFORM_XBOX_360
#	pragma warning( push )
#	pragma warning( disable : 4074 )
#	pragma init_seg( compiler )
#	pragma warning( pop )
#elif VOSTOK_PLATFORM_PS3 // #if VOSTOK_PLATFORM_WINDOWS || VOSTOK_PLATFORM_XBOX_360
#else // #elseif VOSTOK_PLATFORM_PS3
#	error please define your platform
#endif // #if VOSTOK_PLATFORM_WINDOWS || VOSTOK_PLATFORM_XBOX_360

VOSTOK_DECLARE_LINKAGE_ID(debug_initialize);

namespace vostok {
namespace debug {

void			preinitialize		( );

struct debug_preinitializer
{
	debug_preinitializer	( )
	{
		preinitialize		( );
	}
};

static debug_preinitializer	s_preinitializer VOSTOK_INIT_PRIORITY(0);

} // namespace debug
} // namespace vostok
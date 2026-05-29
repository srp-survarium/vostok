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

#	pragma warning( push )
#	pragma warning( disable : 4074 )
#	pragma init_seg( compiler )
#	pragma warning( pop )

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
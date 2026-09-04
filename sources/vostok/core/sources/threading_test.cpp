////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/testing.h>
#include <vostok/core_test_suite.h>
#include <vostok/testing_macro.h>

VOSTOK_DECLARE_LINKAGE_ID(threading_test)

namespace vostok {

static threading::atomic32_type	s_threads_exited	=	0;

threading::atomic32_type	s_value			=	0; 

} // namespace vostok

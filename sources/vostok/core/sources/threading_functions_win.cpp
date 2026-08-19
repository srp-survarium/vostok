////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/os_include.h>
#include <intrin.h>

#pragma intrinsic(_InterlockedAnd, _InterlockedOr)

namespace vostok {
namespace threading {

// interlocked_and / interlocked_or are part of the VOSTOK_CORE_API surface
// (declared out-of-line in threading_functions.h) and are defined here rather
// than inline in threading_functions_win_inline.h. The intrinsic expands to a
// lock cmpxchg retry loop returning the previous value.

// sushi@TODO: remove - noinline+Oy-off hack to beat /GL inline-vs-call wall (interlocked_or/and out-of-line); revisit if a faithful cause is found
#pragma optimize( "y", off )

__declspec(noinline) DISABLE_LINKER_GL atomic32_value_type interlocked_and		( atomic32_type& target, atomic32_value_type mask )
{
	return				( _InterlockedAnd( &target, mask ) );
}

__declspec(noinline) DISABLE_LINKER_GL atomic32_value_type interlocked_or		( atomic32_type& target, atomic32_value_type mask )
{
	return				( _InterlockedOr( &target, mask ) );
}

#pragma optimize( "y", on )

void set_current_thread_affinity_impl	(u32 const hardware_thread)
{
#pragma message(VOSTOK_TODO("Lain 2 Lain: dont forget to uncomment back after debugging"))
	//SetThreadAffinityMask			(GetCurrentThread(), 1 << hardware_thread);	
}

u32 actual_core_count ( )
{
	SYSTEM_INFO						system_info;
	GetSystemInfo					( &system_info );
	return							system_info.dwNumberOfProcessors;
}

} // namespace threading
} // namespace vostok

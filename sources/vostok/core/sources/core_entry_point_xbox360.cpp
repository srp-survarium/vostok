////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/core_entry_point.h>
#include <vostok/os_include.h>
#include <xbdm.h>

namespace vostok {
namespace core {

void platform::preinitialize( )
{
#ifdef DEBUG
	DmMapDevkitDrive	( );
#endif // #ifdef DEBUG
}

} // namespace core
} // namespace vostok
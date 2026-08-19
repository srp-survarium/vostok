////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "platform_extensions_win.h"
#include <vostok/os_include.h>
#include "delayimp.h"

#pragma comment(lib, "delayimp")

pcstr vostok::g_delay_loading_libraries_reference_module = NULL;

bool vostok::platform::unload_delay_loaded_library	( pcstr dll_name )
{
	return	( ( __FUnloadDelayLoadedDLL2(dll_name) != FALSE ) || true );
}

void vostok::platform::log_error ( pcstr format, pcstr argument )
{
	LOG_ERROR ( format, argument );
}

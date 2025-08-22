////////////////////////////////////////////////////////////////////////////
//	Created		: 17.09.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define	VOSTOK_LOG_MODULE_INITIATOR	"engine_pc"

#include <vostok/macro_extensions.h>

#include <vostok/engine/library_dependencies.h>

#define VOSTOK_ENGINE_LIBRARY_BUILDING
#include <vostok/engine/library_linkage.h>

#ifndef VOSTOK_STATIC_LIBRARIES

#include <vostok/type_extensions.h>
#include <boost/noncopyable.hpp>
#include <vostok/detail_noncopyable.h>
#include <vostok/memory_base_allocator.h>
#include <stdio.h>
#include <vostok/os_extensions.h>
#include <vostok/os_include.h>
#include <vostok/engine/api.h>
#include <vostok/debug/extensions.h>

#include "linkage_resolver.h"


BOOL APIENTRY DllMain							( HANDLE hModule, unsigned int ul_reason_for_call, LPVOID lpReserved )
{
	VOSTOK_UNREFERENCED_PARAMETERS( hModule, lpReserved );

	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: {
			break;
		}
		case DLL_PROCESS_DETACH: {
			break;
		}
	}
	return						( TRUE );
}

#endif // #ifndef VOSTOK_STATIC_LIBRARIES
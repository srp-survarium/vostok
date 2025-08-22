////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_LIBRARY_LINKAGE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_LIBRARY_LINKAGE_H_INCLUDED

#include <vostok/macro_library_name.h>

#if VOSTOK_PLATFORM_WINDOWS
#	pragma comment(lib, VOSTOK_LIBRARY_NAME( render_engine_dx11, lib ) )
#	pragma comment(lib, VOSTOK_LIBRARY_NAME( render_core_dx11, lib ) )
#elif VOSTOK_PLATFORM_XBOX_360 // #if VOSTOK_PLATFORM_WINDOWS
#	pragma comment(lib, VOSTOK_LIBRARY_NAME( render_engine_xbox360, lib ) )
#elif VOSTOK_PLATFORM_PS3 // #if VOSTOK_PLATFORM_WINDOWS
#	pragma comment(lib, VOSTOK_LIBRARY_NAME( render_engine_ps3, lib ) )
#else // #elif VOSTOK_PLATFORM_PS3
#	error define your platform here
#endif // #if VOSTOK_PLATFORM_WINDOWS

#endif // #ifndef VOSTOK_RENDER_ENGINE_LIBRARY_LINKAGE_H_INCLUDED
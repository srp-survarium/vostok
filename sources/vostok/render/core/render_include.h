////////////////////////////////////////////////////////////////////////////
//	Created		: 01.06.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_CORE_RENDER_INCLUDE_H_INCLUDED
#define VOSTOK_RENDER_CORE_RENDER_INCLUDE_H_INCLUDED

#include <vostok/os_preinclude.h>
#undef NOUSER
#undef NOMSG
#undef NOGDI
#undef NOTEXTMETRIC
#include <vostok/os_include.h>

#if VOSTOK_PLATFORM_WINDOWS
#	if !USE_DX10
#		include <d3d11.h>
#	else // #if !USE_DX10 
#		include <d3d10.h>
#	endif // #if !USE_DX10 
#	include <dxdiag.h>
#	include <vostok/render/core/d3d_defines.h>
#elif VOSTOK_PLATFORM_XBOX_360 // #if VOSTOK_PLATFORM_WINDOWS
#elif VOSTOK_PLATFORM_PS3 // #elif VOSTOK_PLATFORM_XBOX_360
#else // #elif VOSTOK_PLATFORM_PS3
#endif // #if VOSTOK_PLATFORM_WINDOWS

#endif // #ifndef VOSTOK_RENDER_CORE_RENDER_INCLUDE_H_INCLUDED
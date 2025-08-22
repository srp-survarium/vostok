////////////////////////////////////////////////////////////////////////////
//	Created		: 29.12.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_PLATFORM_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_PLATFORM_H_INCLUDED

#if VOSTOK_PLATFORM_WINDOWS
#	include <vostok/render/engine/world_dx11.h>
#elif VOSTOK_PLATFORM_XBOX_360 // #if VOSTOK_PLATFORM_WINDOWS
#	include <vostok/render/engine/world_xbox360.h>
#elif VOSTOK_PLATFORM_PS3 // #elif VOSTOK_PLATFORM_XBOX360
#	include <vostok/render/engine/world_ps3.h>
#else // #elif VOSTOK_PLATFORM_PS3
#	error define your platform!
#endif // #if VOSTOK_PLATFORM_WINDOWS

#endif // #ifndef VOSTOK_RENDER_ENGINE_PLATFORM_H_INCLUDED
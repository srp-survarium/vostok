////////////////////////////////////////////////////////////////////////////
//	Created		: 14.12.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_CORE_PLATFORM_HELPER_H_INCLUDED
#define VOSTOK_RENDER_CORE_PLATFORM_HELPER_H_INCLUDED

#if VOSTOK_PLATFORM_WINDOWS
#	define VOSTOK_RENDER_CORE_INCLUDE_FOLDER	dx11
#elif VOSTOK_PLATFORM_XBOX_360	// #if VOSTOK_PLATFORM_WINDOWS
#	define VOSTOK_RENDER_CORE_INCLUDE_FOLDER	xbox360
#elif VOSTOK_PLATFORM_PS3 		// #elif VOSTOK_PLATFORM_WINDOWS
#	define VOSTOK_RENDER_CORE_INCLUDE_FOLDER	ps3
#else							// #elif VOSTOK_PLATFORM_PS3
#	error please define your platform!
#endif							// #if VOSTOK_PLATFORM_WINDOWS

#define VOSTOK_RENDER_CORE_FILE(...)	<vostok/render/core/VOSTOK_RENDER_CORE_INCLUDE_FOLDER/__VA_ARGS__>

#endif // #ifndef VOSTOK_RENDER_CORE_PLATFORM_HELPER_H_INCLUDED
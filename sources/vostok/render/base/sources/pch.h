////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_RENDER_BASE_BUILDING

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define VOSTOK_LOG_MODULE_INITIATOR	"render_base"
#include <vostok/extensions.h>

#if VOSTOK_PLATFORM_WINDOWS
#	include <vostok/render/dx9/render_include.h>
#endif // #if VOSTOK_PLATFORM_WINDOWS

//Macros delete is declared in windows headers, so this is small hack to avoid problems
#undef DELETE
#include <vostok/render/base/memory.h>

using vostok::math::float2;
using vostok::math::float3;
using vostok::math::color;

#endif // #ifndef PCH_H_INCLUDED
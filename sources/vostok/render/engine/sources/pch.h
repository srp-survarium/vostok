////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#include <vostok/macro_platform.h>

#	ifdef VOSTOK_OPENGL
#		define VOSTOK_LOG_MODULE_INITIATOR		"render_pc_opengl"
#	else // #ifdef VOSTOK_OPENGL
#		define USE_DX10	0
#		if USE_DX10
#			define VOSTOK_LOG_MODULE_INITIATOR	"render_pc_dx10"
#		else // #if USE_DX10
#			define VOSTOK_LOG_MODULE_INITIATOR	"render_pc_dx11"
#		endif // #if USE_DX10
#	endif // #ifdef VOSTOK_OPENGL

#include <vostok/extensions.h>

#define VOSTOK_RENDER_ENGINE_BUILDING

	// to disable warnings on sinf/cosf in speedtree SDK
#	pragma warning( push )
#	pragma warning( disable : 4995 )
#		include <speedtree/core/core.h>
#		include <speedtree/forest/forest.h>
#	pragma warning( pop )

#	include <speedtree/core/array.h>
#	include <speedtree/core/memory.h>
#	include <speedtree/forest/grass.h>

#include <vostok/render/core/render_include.h>
#include <vostok/render/core/memory.h>

#endif // #ifndef PCH_H_INCLUDED

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

#define VOSTOK_LOG_MODULE_INITIATOR	"render_dx9"
#include <vostok/extensions.h>

#define VOSTOK_RENDER_BUILDING
#include <vostok/render/api.h>

#include <vostok/render/dx9/render_include.h>

#include <vostok/render/base/memory.h>


using vostok::math::float2;
using vostok::math::float3;
using vostok::math::color;
using vostok::math::aabb;
using vostok::math::int2;
using vostok::math::uint2;

using namespace vostok;

using vostok::render::vector;
using vostok::render::map;
//using vostok::render::fixed_vector;

#include <vostok/render/common/sources/d3d_defs.h>
#include <vostok/render/dx9/dx9_include.h>

//#include "visual.h"
#include "render_queue.h"
//#include "model_manager.h"

#endif // #ifndef PCH_H_INCLUDED
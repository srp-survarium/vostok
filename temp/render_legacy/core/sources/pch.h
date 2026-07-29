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

#define VOSTOK_LOG_MODULE_INITIATOR	"render"

#include <vostok/extensions.h>

#define VOSTOK_RENDER_CORE_BUILDING

#include <vostok/render/core/render_include.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/macros_extensions.h>

#endif // #ifndef PCH_H_INCLUDED
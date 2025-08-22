////////////////////////////////////////////////////////////////////////////
//	Created 	: 16.01.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define VOSTOK_LOG_MODULE_INITIATOR	"maya_animation"
#include <vostok/extensions.h>

#define VOSTOK_DEFAULT_BUILDING
#include <vostok/maya_animation/world.h>

#include "maya_animation_memory.h"

#endif // #ifndef PCH_H_INCLUDED
////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_RENDER_BUILDING

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define VOSTOK_LOG_MODULE_INITIATOR	"render_facade"
#include <vostok/extensions.h>

#include "logic_allocator.h"
#include "editor_allocator.h"

#endif // #ifndef PCH_H_INCLUDED
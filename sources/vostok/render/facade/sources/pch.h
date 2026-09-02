// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_FACADE_SOURCES_PCH_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_PCH_H_INCLUDED
#define VOSTOK_RENDER_BUILDING

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define VOSTOK_LOG_MODULE_INITIATOR "render_facade"
#include <vostok/extensions.h>

#include "logic_allocator.h"
#include "editor_allocator.h"

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_PCH_H_INCLUDED

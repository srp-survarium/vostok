////////////////////////////////////////////////////////////////////////////
//	Created		: 21.09.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ENGINE_LIBRARY_DEPENDENCIES_H_INCLUDED
#define VOSTOK_ENGINE_LIBRARY_DEPENDENCIES_H_INCLUDED

#include <vostok/debug/library_linkage.h>
#include <vostok/logging/library_linkage.h>
#include <vostok/core/library_linkage.h>
#include <vostok/fs/library_linkage.h>
#include <vostok/vfs/library_linkage.h>
#include <vostok/network/library_linkage.h>
#include <vostok/sound/library_linkage.h>

#include <vostok/ai_navigation/library_linkage.h>
#include <vostok/ai/library_linkage.h>
#include <vostok/animation/library_linkage.h>
#include <vostok/collision/library_linkage.h>
#include <vostok/input/library_linkage.h>
#include <vostok/physics/library_linkage.h>
#include <vostok/rtp/library_linkage.h>
#include <vostok/ui/library_linkage.h>

#include <vostok/render/facade/library_linkage.h>
#ifndef VOSTOK_STATIC_LIBRARIES
#	include <vostok/render/library_linkage.h>
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#include <vostok/editor/world/library_linkage.h>

#endif // #ifndef VOSTOK_ENGINE_LIBRARY_DEPENDENCIES_H_INCLUDED
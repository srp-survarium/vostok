////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/engine/pc/library_linkage.h>
#include <vostok/game/library_linkage.h>

#ifdef VOSTOK_STATIC_LIBRARIES
#	include <vostok/engine/library_linkage.h>
#	include <vostok/engine/library_dependencies.h>

#	include <ogg/library_linkage.h>
#	include <vorbis/library_linkage.h>
#	pragma comment( lib, "x3daudio.lib" )
#	include <ode/library_linkage.h>
#	include <vostok/render/engine/library_linkage.h>
#	include <vostok/render/core/library_linkage.h>

#	include <vostok/linkage_helper.h>
	VOSTOK_INCLUDE_TO_LINKAGE(sound_library_linkage)
#endif // #ifdef VOSTOK_STATIC_LIBRARIES
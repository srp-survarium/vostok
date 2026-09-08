////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#ifdef OPENGL
#	include <vostok/engine/pc/opengl/library_linkage.h>
#else // #ifdef OPENGL
#	include <vostok/engine/pc/dx11/library_linkage.h>
#endif // #ifdef OPENGL

#include <vostok/game/library_linkage.h>
#include <vostok/game_core/library_linkage.h>

#if !defined(VOSTOK_GAME_DLL)
#include <vostok/network_core/library_linkage.h>

#include <vostok/physics/library_linkage.h>
#endif

#ifdef VOSTOK_STATIC_LIBRARIES
#	include <vostok/engine/library_linkage.h>
#	include <vostok/engine/library_dependencies.h>

#	include <ogg/library_linkage.h>
#	include <vorbis/library_linkage.h>
#	pragma comment( lib, "x3daudio.lib" )
#	include <ode/library_linkage.h>
#	include <vostok/render/engine/library_linkage.h>

#endif // #ifdef VOSTOK_STATIC_LIBRARIES

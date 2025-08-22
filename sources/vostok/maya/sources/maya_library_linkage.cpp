////////////////////////////////////////////////////////////////////////////
//	Created		: 07.05.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/debug/library_linkage.h>
#include <vostok/logging/library_linkage.h>
#include <vostok/core/library_linkage.h>
#include <vostok/fs/library_linkage.h>
#include <vostok/vfs/library_linkage.h>
#include <vostok/maya_animation/library_linkage.h>
#include <vostok/physics/library_linkage.h>
#include <vostok/animation/library_linkage.h>
#include <vostok/render/facade/library_linkage.h>
#include <vostok/render/engine/library_linkage.h>
#include <vostok/render/core/library_linkage.h>
#include <vostok/particle/library_linkage.h>
#include <vostok/collision/library_linkage.h>

#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "OpenMayaUI.lib" )
#pragma comment( lib, "Foundation.lib" )
#pragma comment( lib, "OpenMaya.lib" )
#pragma comment( lib, "OpenMayaAnim.lib" )
#pragma comment( lib, "OpenMayaRender.lib" )

#if defined WIN64
	#pragma comment( lib, "TootleSoftwareOnlyStatic_2k8_MTDLL64.lib" )
#else
	#pragma comment( lib, "TootleSoftwareOnlyStatic_2k8_MTDLL.lib" )
#endif

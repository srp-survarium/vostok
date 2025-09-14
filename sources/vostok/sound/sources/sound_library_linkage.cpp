////////////////////////////////////////////////////////////////////////////
//	Created 	: 22.12.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/core/library_linkage.h>
#include <ogg/library_linkage.h>

#include <vostok/linkage_helper.h>

VOSTOK_DECLARE_LINKAGE_ID(sound_library_linkage)

#if !VOSTOK_PLATFORM_PS3
#	pragma comment( lib, "x3daudio.lib" )
#	pragma comment( lib, "xapofx.lib" )
#	pragma comment( lib, "xapobase.lib" )
#endif // #if !VOSTOK_PLATFORM_PS3

#if VOSTOK_PLATFORM_XBOX_360
#	pragma comment( lib, "xaudio2.lib" )
#	pragma comment( lib, "xmcore.lib" )
#endif // #if VOSTOK_PLATFORM_XBOX_360
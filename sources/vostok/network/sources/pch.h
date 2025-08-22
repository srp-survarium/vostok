////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_NETWORK_BUILDING

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define VOSTOK_USE_STDIOSTREAM
#define VOSTOK_NO_SIGNALS_H

#include "stlport_config.h"

#include <vostok/os_preinclude.h>
#undef NONET

#define VOSTOK_LOG_MODULE_INITIATOR	"network"
#include <vostok/extensions.h>

#include "network_memory.h"

#if !VOSTOK_PLATFORM_PS3
#	define _WIN32_WINNT 0x0501
#	include "boost_api.h"
#endif // #if !VOSTOK_PLATFORM_PS3

#endif // #ifndef PCH_H_INCLUDED
////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_NETWORK_BUILDING

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define VOSTOK_LOG_MODULE_INITIATOR	"network"
#include <vostok/extensions.h>

#include "network_memory.h"

#ifdef WIN32
#	define _WIN32_WINNT		0x0600
#endif // #ifdef WIN32

#include <vostok/os_include.h>

#include <boost/asio.hpp>

using namespace vostok;

#endif // #ifndef PCH_H_INCLUDED

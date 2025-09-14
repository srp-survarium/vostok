////////////////////////////////////////////////////////////////////////////
//	Created		: 15.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_LOGGING_BUILDING

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define	VOSTOK_LOG_MODULE_INITIATOR	"logging"

#include <vostok/macro_platform.h>
#include <vostok/type_extensions.h>
#include <vostok/macro_extensions.h>
#include <vostok/debug/extensions.h>
#include <vostok/memory_override_operators.h>
#include <vostok/memory_extensions.h>
#include <boost/noncopyable.hpp>
#include <vostok/command_line_extensions.h>
#include <vostok/threading_extensions.h>
#include <vostok/containers_extensions.h>
#include <vostok/uninitialized_reference.h>
#include <vostok/type_enum_flags.h>
#include <vostok/math_extensions.h>
#include <vostok/stdlib_extensions.h>
#include <vostok/strings_extensions.h>

#include "logging_memory.h"

#endif // #ifndef PCH_H_INCLUDED
////////////////////////////////////////////////////////////////////////////
//	Created		: 15.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_FS_BUILDING

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define VOSTOK_LOG_MODULE_INITIATOR	"fs"
#include <vostok/macro_extensions.h>
#include <vostok/optimization_extensions.h>
#include <vostok/type_extensions.h>
#include <vostok/platform_extensions.h>
#include <vostok/detail_noncopyable.h>
#include <vostok/debug/extensions.h>
#include <vostok/stdlib_extensions.h>
#include <vostok/debug/static_cast_checked.h>
#include <vostok/debug/pointer_cast.h>
#include <vostok/memory_extensions.h>
#include <vostok/command_line_extensions.h>
#include <vostok/os_extensions.h>
#include <vostok/timing_extensions.h>
#include <vostok/threading_extensions.h>
#include <vostok/uninitialized_reference.h>
#include <vostok/type_enum_flags.h>
#include <vostok/memory_debug_allocator.h>
#include <vostok/strings_extensions.h>
#include <vostok/debug/check_no_multithread.h>
#include <vostok/logging/extensions.h>
#include <vostok/pointer_extensions.h>
#include <vostok/debug/debug_pointers.h>
#include <vostok/containers_extensions.h>
#include <vostok/math_extensions.h>
#include <vostok/memory_chunk_reader.h>
#include <vostok/memory_writer.h>

#include "memory.h"

#endif // #ifndef PCH_H_INCLUDED
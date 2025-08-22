////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_EXTENSIONS_H_INCLUDED
#define VOSTOK_EXTENSIONS_H_INCLUDED

#ifdef SN_TARGET_PS3
	namespace scestd = std;
	#include <assert.h>
	namespace std {
		void _SCE_Assert(const char *first, const char *second);
		void _Assert(const char *first, const char *second);
	} // namespace std
#endif // #ifdef SN_TARGET_PS3

#define BOOST_NO_EXCEPTIONS   
#include <boost/noncopyable.hpp>

#include <vostok/macro_extensions.h>
#include <vostok/optimization_extensions.h>
#include <vostok/type_extensions.h>
#include <vostok/platform_extensions.h>
#include <vostok/detail_noncopyable.h>
#include <vostok/debug/extensions.h>
#include <vostok/memory_override_operators.h>
#include <vostok/memory_extensions.h>
#include <vostok/stdlib_extensions.h>
#include <vostok/debug/static_cast_checked.h>
#include <vostok/debug/pointer_cast.h>
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
#include <vostok/tasks_system.h>
#include <vostok/configs.h>
#include <vostok/resources.h>

#if !VOSTOK_CORE_BUILDING
	namespace vostok {
		using math::float2;
		using math::float3;
		using math::float4;
		using math::float4x4;
	} // namespace vostok
#endif // #if !VOSTOK_CORE_BUILDING

#endif // #ifndef VOSTOK_EXTENSIONS_H_INCLUDED
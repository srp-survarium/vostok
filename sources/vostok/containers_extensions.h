////////////////////////////////////////////////////////////////////////////
//	Created 	: 01.09.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_CONTAINERS_EXTENSIONS_H_INCLUDED
#define VOSTOK_CONTAINERS_EXTENSIONS_H_INCLUDED

#include <vostok/c_array_functions.h>

#include <vostok/buffer_vector.h>
#include <vostok/fixed_vector.h>
#include <vostok/associative_vector.h>
#include <vostok/hash_multiset.h>

#include <map>
#include <vector>
#include <list>
#include <set>
#include <deque>

#include <vostok/vectora.h>

namespace vostok {
namespace mt {
#	define USER_ALLOCATOR		::vostok::memory::g_mt_allocator
#	include <vostok/std_containers.h>
#	include <vostok/unique_ptr.h>
#	undef USER_ALLOCATOR
} // namespace mt

#if VOSTOK_DEBUG_ALLOCATOR
namespace debug {
#	define USER_ALLOCATOR		::vostok::debug::g_mt_allocator
#	include <vostok/std_containers.h>
#	include <vostok/unique_ptr.h>
#	undef USER_ALLOCATOR
} // namespace debug
#endif // #if VOSTOK_DEBUG_ALLOCATOR

} // namespace vostok

#endif // #ifndef VOSTOK_CONTAINERS_EXTENSIONS_H_INCLUDED
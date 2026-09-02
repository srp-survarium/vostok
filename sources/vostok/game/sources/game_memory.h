////////////////////////////////////////////////////////////////////////////
//	Created 	: 11.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include <vostok/buffer_vector.h>
#include <vostok/fixed_vector.h>
#include <vostok/associative_vector.h>
#include <vostok/hash_multiset.h>

#include <vostok/os_preinclude.h>
#define _WIN32_WINNT				0x0500
#include <vostok/os_include.h>

namespace survarium {

extern vostok::memory::doug_lea_allocator_type*	g_allocator;

#define USER_ALLOCATOR							*::survarium::g_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

} // namespace survarium

#endif // #ifndef MEMORY_H_INCLUDED

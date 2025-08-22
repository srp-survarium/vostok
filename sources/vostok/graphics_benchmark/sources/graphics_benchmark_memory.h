////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef GRAPHICS_BENCHMARK_MEMORY_H_INCLUDED
#define GRAPHICS_BENCHMARK_MEMORY_H_INCLUDED

#include <vostok/buffer_vector.h>
#include <vostok/fixed_vector.h>
#include <vostok/associative_vector.h>
#include <vostok/hash_multiset.h>

#include <map>
#include <vector>
#include <set>

namespace vostok {
namespace graphics_benchmark {

extern	vostok::memory::doug_lea_allocator_type	g_allocator;

} // namespace graphics_benchmark
} // namespace vostok

#include <vostok/render/facade/render_allocator.h>

#endif // #ifndef GRAPHICS_BENCHMARK_MEMORY_H_INCLUDED
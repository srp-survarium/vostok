////////////////////////////////////////////////////////////////////////////
//	Created		: 29.03.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_ANIMATION_CONTAINERS_H_INCLUDED
#define VOSTOK_ANIMATION_ANIMATION_CONTAINERS_H_INCLUDED

#include <vostok/animation/api.h>

namespace vostok {
namespace animation {

extern allocator_type*							g_allocator;

#define USER_ALLOCATOR							*::vostok::animation::g_allocator
#include <vostok/std_containers.h>
#undef USER_ALLOCATOR

} // namespace animation
} // namespace vostok

#endif // #ifndef VOSTOK_ANIMATION_ANIMATION_CONTAINERS_H_INCLUDED
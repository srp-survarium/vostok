#ifndef UI_MEMORY_H_INCLUDED
#define UI_MEMORY_H_INCLUDED

#include <vostok/ui/api.h>

namespace vostok {
namespace ui {

extern allocator_type*						g_allocator;

#define USER_ALLOCATOR						*::vostok::ui::g_allocator
#include <vostok/std_containers.h>
#include <vostok/unique_ptr.h>
#undef USER_ALLOCATOR

} // namespace ui
} // namespace vostok

#define NEW(type)							VOSTOK_NEW_IMPL		(	*::vostok::ui::g_allocator, type)
#define DELETE(pointer)						VOSTOK_DELETE_IMPL	(	*::vostok::ui::g_allocator, pointer)
#define MALLOC(size, description)			VOSTOK_MALLOC_IMPL	(	*::vostok::ui::g_allocator, size, description)
#define REALLOC(pointer, size, description)	VOSTOK_REALLOC_IMPL	(	*::vostok::ui::g_allocator, pointer, size, description)
#define FREE(pointer)						VOSTOK_FREE_IMPL		(	*::vostok::ui::g_allocator, pointer)
#define ALLOC(type, count)					VOSTOK_ALLOC_IMPL		(	*::vostok::ui::g_allocator, type, count)

#endif // #ifndef UI_MEMORY_H_INCLUDED
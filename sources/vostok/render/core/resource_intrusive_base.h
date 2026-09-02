// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_RESOURCE_INTRUSIVE_BASE_H_INCLUDED
#define VOSTOK_RENDER_CORE_RESOURCE_INTRUSIVE_BASE_H_INCLUDED
namespace vostok {
namespace render {

class resource_intrusive_base {
public:
	resource_intrusive_base( );

	template < typename T >
	static void destroy( T const* instance );

	mutable u32 m_reference_count;
};

STATIC_SIZE_ASSERT( resource_intrusive_base, 0x4 );

} // namespace render
} // namespace vostok

#include <vostok/render/core/resource_intrusive_base_inline.h>

#endif // #ifndef VOSTOK_RENDER_CORE_RESOURCE_INTRUSIVE_BASE_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RESOURCE_INTRUSIVE_BASE_H_INCLUDED
#define RENDER_RESOURCE_INTRUSIVE_BASE_H_INCLUDED

namespace vostok {
namespace render {

class resource_intrusive_base {
public:
	inline		resource_intrusive_base	( ) { /* no source */ }

	/* 0x0000 */	u32		m_reference_count;
}; // class resource_intrusive_base

STATIC_SIZE_ASSERT(resource_intrusive_base, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RESOURCE_INTRUSIVE_BASE_H_INCLUDED

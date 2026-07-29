////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MODEL_LOCATOR_ITEM_H_INCLUDED
#define RENDER_MODEL_LOCATOR_ITEM_H_INCLUDED

namespace vostok {
namespace render {

struct model_locator_item {
	inline		model_locator_item	( ) { /* no source */ }

	/* 0x0000 */	char		m_name[32];
	/* 0x0020 */	float4x4	m_offset;
	/* 0x0060 */	u16			m_bone;
}; // struct model_locator_item

STATIC_SIZE_ASSERT(model_locator_item, 0x64);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MODEL_LOCATOR_ITEM_H_INCLUDED

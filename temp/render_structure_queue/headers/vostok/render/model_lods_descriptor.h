////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MODEL_LODS_DESCRIPTOR_H_INCLUDED
#define RENDER_MODEL_LODS_DESCRIPTOR_H_INCLUDED

namespace vostok {
namespace render {

struct model_lods_descriptor {
	inline		model_lods_descriptor	( ) { /* no source */ }

	/* 0x0000 */	u8			m_lod_surfaces_count[3];
	/* 0x0004 */	pbyte		m_lod_surfaces[3];
	/* 0x0010 */	u8			m_lod_calc_type;
	/* 0x0014 */	float		m_lod_custom_params[3];
	/* 0x0020 */	bool		m_lod_params_default;
}; // struct model_lods_descriptor

STATIC_SIZE_ASSERT(model_lods_descriptor, 0x24);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MODEL_LODS_DESCRIPTOR_H_INCLUDED

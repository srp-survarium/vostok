////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_COMPILER_TEXTURE_QUERY_DESC_H_INCLUDED
#define RENDER_EFFECT_COMPILER_TEXTURE_QUERY_DESC_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<260>;

namespace vostok {
namespace render {

struct effect_compiler::texture_query_desc {
	inline		texture_query_desc	( ) { /* no source */ }

	/* 0x0000 */	fixed_string< 260 >		m_query_physicaly_path;
	/* 0x0110 */	u32						m_mip_level_cut;
	/* 0x0114 */	u32						m_num_last_mips_used;
}; // struct effect_compiler::texture_query_desc

STATIC_SIZE_ASSERT(effect_compiler::texture_query_desc, 0x118);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_COMPILER_TEXTURE_QUERY_DESC_H_INCLUDED

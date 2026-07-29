////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_UI_VERTEX_H_INCLUDED
#define RENDER_UI_VERTEX_H_INCLUDED

namespace vostok {
namespace render {

struct ui::vertex {
	inline		vertex	( ) { /* no source */ }
	inline		vertex	(
					const float		arg_0,
					const float		arg_1,
					const float		arg_2,
					const u32		arg_3,
					const float		arg_4,
					const float		arg_5
				) { /* no source */ }

	/* 0x0000 */	float4		m_position;
	/* 0x0010 */	u32			m_color;
	/* 0x0014 */	float2		m_uv;
}; // struct ui::vertex

STATIC_SIZE_ASSERT(ui::vertex, 0x1C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_UI_VERTEX_H_INCLUDED

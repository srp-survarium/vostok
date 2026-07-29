////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERTEX_FORMATS_TL0UV_H_INCLUDED
#define RENDER_VERTEX_FORMATS_TL0UV_H_INCLUDED

namespace vostok {
namespace render {

struct vertex_formats::TL0uv {
	inline	void	set			( vertex_formats::TL0uv const& arg_0 ) { /* no source */ }
	inline	void	set			( float arg_0, float arg_1, u32 arg_2 ) { /* no source */ }
	inline	void	set			( s32 arg_0, s32 arg_1, u32 arg_2 ) { /* no source */ }
	inline	void	set			(
						float		arg_0,
						float		arg_1,
						float		arg_2,
						float		arg_3,
						u32			arg_4
					) { /* no source */ }

	inline	void	transform	( float3 const& arg_0, float4x4 const& arg_1 ) { /* no source */ }

	inline			TL0uv		( ) { /* no source */ }

	/* 0x0000 */	float4		p;
	/* 0x0010 */	u32			color;
}; // struct vertex_formats::TL0uv

STATIC_SIZE_ASSERT(vertex_formats::TL0uv, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERTEX_FORMATS_TL0UV_H_INCLUDED

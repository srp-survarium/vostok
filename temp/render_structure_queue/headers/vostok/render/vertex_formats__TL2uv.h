////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERTEX_FORMATS_TL2UV_H_INCLUDED
#define RENDER_VERTEX_FORMATS_TL2UV_H_INCLUDED

namespace vostok {
namespace render {

struct vertex_formats::TL2uv {
	inline	void	set			( vertex_formats::TL2uv const& arg_0 ) { /* no source */ }
	inline	void	set			(
						float		arg_0,
						float		arg_1,
						u32			arg_2,
						float2&		arg_3,
						float2&		arg_4
					) { /* no source */ }
	inline	void	set			(
						float		arg_0,
						float		arg_1,
						float		arg_2,
						float		arg_3,
						u32			arg_4,
						float2&		arg_5,
						float2&		arg_6
					) { /* no source */ }
	inline	void	set			(
						float		arg_0,
						float		arg_1,
						u32			arg_2,
						float		arg_3,
						float		arg_4,
						float		arg_5,
						float		arg_6
					) { /* no source */ }
	inline	void	set			(
						s32			arg_0,
						s32			arg_1,
						u32			arg_2,
						float		arg_3,
						float		arg_4,
						float		arg_5,
						float		arg_6
					) { /* no source */ }
	inline	void	set			(
						float		arg_0,
						float		arg_1,
						float		arg_2,
						float		arg_3,
						u32			arg_4,
						float		arg_5,
						float		arg_6,
						float		arg_7,
						float		arg_8
					) { /* no source */ }

	inline	void	transform	( float3 const& arg_0, float4x4 const& arg_1 ) { /* no source */ }

	inline			TL2uv		( ) { /* no source */ }

	/* 0x0000 */	float4		p;
	/* 0x0010 */	u32			color;
	/* 0x0014 */	float2		uv[2];
}; // struct vertex_formats::TL2uv

STATIC_SIZE_ASSERT(vertex_formats::TL2uv, 0x24);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERTEX_FORMATS_TL2UV_H_INCLUDED

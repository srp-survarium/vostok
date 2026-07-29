////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERTEX_FORMATS_LIT_H_INCLUDED
#define RENDER_VERTEX_FORMATS_LIT_H_INCLUDED

namespace vostok {
namespace render {

struct vertex_formats::LIT {
	inline	void	set	( vertex_formats::LIT const& arg_0 ) { /* no source */ }
	inline	void	set	(
						float		arg_0,
						float		arg_1,
						float		arg_2,
						u32			arg_3,
						float		arg_4,
						float		arg_5
					) { /* no source */ }
	inline	void	set	(
						float3 const&		arg_0,
						u32					arg_1,
						float				arg_2,
						float				arg_3
					) { /* no source */ }

	inline			LIT	( ) { /* no source */ }

	/* 0x0000 */	float3		p;
	/* 0x000c */	u32			color;
	/* 0x0010 */	float2		t;
}; // struct vertex_formats::LIT

STATIC_SIZE_ASSERT(vertex_formats::LIT, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERTEX_FORMATS_LIT_H_INCLUDED

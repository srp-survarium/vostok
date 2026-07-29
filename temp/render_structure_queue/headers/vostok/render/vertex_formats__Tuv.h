////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERTEX_FORMATS_TUV_H_INCLUDED
#define RENDER_VERTEX_FORMATS_TUV_H_INCLUDED

namespace vostok {
namespace render {

struct vertex_formats::Tuv {
	inline	void	set	( vertex_formats::Tuv const& arg_0 ) { /* no source */ }
	inline	void	set	(
						float		arg_0,
						float		arg_1,
						float		arg_2,
						float		arg_3,
						float		arg_4,
						float		arg_5
					) { /* no source */ }
	inline	void	set	( float4 const& arg_0, float arg_1, float arg_2 ) { /* no source */ }

	inline			Tuv	( ) { /* no source */ }

	/* 0x0000 */	float4		p;
	/* 0x0010 */	float2		t;
}; // struct vertex_formats::Tuv

STATIC_SIZE_ASSERT(vertex_formats::Tuv, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERTEX_FORMATS_TUV_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERTEX_FORMATS_V_H_INCLUDED
#define RENDER_VERTEX_FORMATS_V_H_INCLUDED

namespace vostok {
namespace render {

struct vertex_formats::V {
	inline	void	set	( vertex_formats::V const& arg_0 ) { /* no source */ }
	inline	void	set	(
						float		arg_0,
						float		arg_1,
						float		arg_2,
						float		arg_3,
						float		arg_4
					) { /* no source */ }
	inline	void	set	( float3 const& arg_0, float arg_1, float arg_2 ) { /* no source */ }

	inline			V	( ) { /* no source */ }

	/* 0x0000 */	float3		p;
	/* 0x000c */	float2		t;
}; // struct vertex_formats::V

STATIC_SIZE_ASSERT(vertex_formats::V, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERTEX_FORMATS_V_H_INCLUDED

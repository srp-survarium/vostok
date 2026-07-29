////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERTEX_FORMATS_L_H_INCLUDED
#define RENDER_VERTEX_FORMATS_L_H_INCLUDED

namespace vostok {
namespace render {

struct vertex_formats::L {
	inline	void	set	( vertex_formats::L const& arg_0 ) { /* no source */ }
	inline	void	set	(
						float		arg_0,
						float		arg_1,
						float		arg_2,
						u32			arg_3
					) { /* no source */ }
	inline	void	set	( float3 const& arg_0, u32 arg_1 ) { /* no source */ }

	inline			L	( ) { /* no source */ }

	/* 0x0000 */	float3		p;
	/* 0x000c */	u32			color;
}; // struct vertex_formats::L

STATIC_SIZE_ASSERT(vertex_formats::L, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERTEX_FORMATS_L_H_INCLUDED

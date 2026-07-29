////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCREEN_VERTEX_H_INCLUDED
#define RENDER_SCREEN_VERTEX_H_INCLUDED

namespace vostok {
namespace render {

struct screen_vertex {
	inline	void	set				( float4 const& arg_0, float2 const& arg_1 ) { /* no source */ }

	inline			screen_vertex	( ) { /* no source */ }

	/* 0x0000 */	float4		position;
	/* 0x0010 */	float2		tc;
}; // struct screen_vertex

STATIC_SIZE_ASSERT(screen_vertex, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCREEN_VERTEX_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_PARTICLE_BEAMTRAIL_VERTEX_H_INCLUDED
#define RENDER_PARTICLE_BEAMTRAIL_VERTEX_H_INCLUDED

namespace vostok {
namespace render {

struct particle_beamtrail_vertex {
	inline	void	set	( float3 arg_0, float4 arg_1, float2 arg_2 ) { /* no source */ }

	/* 0x0000 */	float		position[3];
	/* 0x000c */	float		color[4];
	/* 0x001c */	float		uv[2];
}; // struct particle_beamtrail_vertex

STATIC_SIZE_ASSERT(particle_beamtrail_vertex, 0x24);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_PARTICLE_BEAMTRAIL_VERTEX_H_INCLUDED

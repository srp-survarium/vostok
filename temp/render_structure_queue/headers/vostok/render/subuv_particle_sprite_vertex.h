////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SUBUV_PARTICLE_SPRITE_VERTEX_H_INCLUDED
#define RENDER_SUBUV_PARTICLE_SPRITE_VERTEX_H_INCLUDED

/* INCLUDES */
struct vostok::render::particle_sprite_vertex;

namespace vostok {
namespace render {

struct subuv_particle_sprite_vertex : public particle_sprite_vertex {
	inline	void	set							(
						float3		arg_0,
						float4		arg_1,
						float2		arg_2,
						float2		arg_3,
						float		arg_4,
						float		arg_5,
						float3		arg_6,
						float2		arg_7,
						float4		arg_8
					) { /* no source */ }

	inline			subuv_particle_sprite_vertex( ) { /* no source */ }

	/* 0x0000 */	/* particle_sprite_vertex */
	/* 0x0040 */	float2		size_uv;
	/* 0x0048 */	float4		blend_uv;
}; // struct subuv_particle_sprite_vertex

STATIC_SIZE_ASSERT(subuv_particle_sprite_vertex, 0x58);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SUBUV_PARTICLE_SPRITE_VERTEX_H_INCLUDED

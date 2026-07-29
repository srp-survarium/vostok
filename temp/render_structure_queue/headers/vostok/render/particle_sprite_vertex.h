////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_PARTICLE_SPRITE_VERTEX_H_INCLUDED
#define RENDER_PARTICLE_SPRITE_VERTEX_H_INCLUDED

namespace vostok {
namespace render {

struct particle_sprite_vertex {
			void	set						(
						float3		in_position,
						float4		in_color,
						float2		in_uv,
						float2		in_size,
						float		in_rotation,
						float		in_gravity,
						float3		in_old_position
					);

	inline			particle_sprite_vertex	( ) { /* no source */ }

	/* 0x0000 */	float3		position;
	/* 0x000c */	float4		color;
	/* 0x001c */	float2		uv;
	/* 0x0024 */	float2		size;
	/* 0x002c */	float		rotation;
	/* 0x0030 */	float		gravity;
	/* 0x0034 */	float3		old_position;
}; // struct particle_sprite_vertex

STATIC_SIZE_ASSERT(particle_sprite_vertex, 0x40);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_PARTICLE_SPRITE_VERTEX_H_INCLUDED

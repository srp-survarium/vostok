////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_HARDWARE_2WEIGHTS_SKINNING_VERTEX_H_INCLUDED
#define RENDER_HARDWARE_2WEIGHTS_SKINNING_VERTEX_H_INCLUDED

namespace vostok {
namespace render {

struct hardware_2weights_skinning_vertex {
	inline		hardware_2weights_skinning_vertex	( ) { /* no source */ }

	/* 0x0000 */	float3		P;
	/* 0x000c */	u8			indices[4];
	/* 0x0010 */	float		weights;
	/* 0x0014 */	u32			N;
	/* 0x0018 */	u32			T;
	/* 0x001c */	u32			B;
	/* 0x0020 */	float2		uv;
}; // struct hardware_2weights_skinning_vertex

STATIC_SIZE_ASSERT(hardware_2weights_skinning_vertex, 0x28);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_HARDWARE_2WEIGHTS_SKINNING_VERTEX_H_INCLUDED

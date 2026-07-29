////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_LIGHTS_INSTANCE_DATA_H_INCLUDED
#define RENDER_STAGE_LIGHTS_INSTANCE_DATA_H_INCLUDED

namespace vostok {
namespace render {

struct stage_lights::instance_data {
	inline		instance_data	( ) { /* no source */ }

	/* 0x0000 */	float3		color;
	/* 0x000c */	float3		position;
	/* 0x0018 */	float		range;
}; // struct stage_lights::instance_data

STATIC_SIZE_ASSERT(stage_lights::instance_data, 0x1C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_LIGHTS_INSTANCE_DATA_H_INCLUDED

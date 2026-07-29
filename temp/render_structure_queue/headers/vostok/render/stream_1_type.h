////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STREAM_1_TYPE_H_INCLUDED
#define RENDER_STREAM_1_TYPE_H_INCLUDED

namespace vostok {
namespace render {

struct stream_1_type {
	inline		stream_1_type	( ) { /* no source */ }

	/* 0x0000 */	float3		object_position;
	/* 0x000c */	u32			color_and_wind;
}; // struct stream_1_type

STATIC_SIZE_ASSERT(stream_1_type, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STREAM_1_TYPE_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCENE_PROCESS_STREAMING___L16_READY_TEXTURE_COMPARER_H_INCLUDED
#define RENDER_SCENE_PROCESS_STREAMING___L16_READY_TEXTURE_COMPARER_H_INCLUDED

/* FORWARD REFS */
class vostok::render::streaming_ready_texture;

namespace vostok {
namespace render {

struct scene::process_streaming::__l16::ready_texture_comparer {
	inline	bool	operator()	( streaming_ready_texture const& arg_0, streaming_ready_texture const& arg_1 ) { /* no source */ }
}; // struct scene::process_streaming::__l16::ready_texture_comparer

STATIC_SIZE_ASSERT(scene::process_streaming::__l16::ready_texture_comparer, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCENE_PROCESS_STREAMING___L16_READY_TEXTURE_COMPARER_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCENE_PROCESS_STREAMING___L3_REMOVE_REQUESTED_TEXTURE_PREDICATE_H_INCLUDED
#define RENDER_SCENE_PROCESS_STREAMING___L3_REMOVE_REQUESTED_TEXTURE_PREDICATE_H_INCLUDED

/* FORWARD REFS */
class vostok::render::requested_streamable_texture;

namespace vostok {
namespace render {

struct scene::process_streaming::__l3::remove_requested_texture_predicate {
	inline	bool	operator()	( requested_streamable_texture const& arg_0 ) { /* no source */ }
}; // struct scene::process_streaming::__l3::remove_requested_texture_predicate

STATIC_SIZE_ASSERT(scene::process_streaming::__l3::remove_requested_texture_predicate, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCENE_PROCESS_STREAMING___L3_REMOVE_REQUESTED_TEXTURE_PREDICATE_H_INCLUDED

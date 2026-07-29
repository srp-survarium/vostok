////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCENE_PROCESS_STREAMING___L2_REMOVE_TEXTURE_PREDICATE_H_INCLUDED
#define RENDER_SCENE_PROCESS_STREAMING___L2_REMOVE_TEXTURE_PREDICATE_H_INCLUDED

/* FORWARD REFS */
class vostok::render::streamable_texture_info;

namespace vostok {
namespace render {

struct scene::process_streaming::__l2::remove_texture_predicate {
	inline	bool	operator()	( streamable_texture_info const& arg_0 ) { /* no source */ }
}; // struct scene::process_streaming::__l2::remove_texture_predicate

STATIC_SIZE_ASSERT(scene::process_streaming::__l2::remove_texture_predicate, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCENE_PROCESS_STREAMING___L2_REMOVE_TEXTURE_PREDICATE_H_INCLUDED

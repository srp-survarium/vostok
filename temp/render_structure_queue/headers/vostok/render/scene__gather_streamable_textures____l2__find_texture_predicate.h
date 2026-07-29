////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCENE_GATHER_STREAMABLE_TEXTURES___L2_FIND_TEXTURE_PREDICATE_H_INCLUDED
#define RENDER_SCENE_GATHER_STREAMABLE_TEXTURES___L2_FIND_TEXTURE_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::res_texture;

/* FORWARD REFS */
class vostok::render::streamable_texture_info;

namespace vostok {
namespace render {

struct scene::gather_streamable_textures::__l2::find_texture_predicate {
	inline	explicit	find_texture_predicate	( res_texture* arg_0 ) { /* no source */ }

	inline	bool		operator()				( streamable_texture_info const& arg_0 ) const { /* no source */ }

	/* 0x0000 */	res_texture*	m_texture;
}; // struct scene::gather_streamable_textures::__l2::find_texture_predicate

STATIC_SIZE_ASSERT(scene::gather_streamable_textures::__l2::find_texture_predicate, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCENE_GATHER_STREAMABLE_TEXTURES___L2_FIND_TEXTURE_PREDICATE_H_INCLUDED

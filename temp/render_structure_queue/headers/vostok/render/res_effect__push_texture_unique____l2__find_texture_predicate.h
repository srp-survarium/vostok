////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_EFFECT_PUSH_TEXTURE_UNIQUE___L2_FIND_TEXTURE_PREDICATE_H_INCLUDED
#define RENDER_RES_EFFECT_PUSH_TEXTURE_UNIQUE___L2_FIND_TEXTURE_PREDICATE_H_INCLUDED

/* INCLUDES */
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
class vostok::render::res_texture;

/* FORWARD REFS */
class vostok::render::texture_named_instance;

namespace vostok {
namespace render {

struct res_effect::push_texture_unique::__l2::find_texture_predicate {
	inline	explicit	find_texture_predicate	( res_texture_ptr const& arg_0 ) { /* no source */ }

	inline	bool		operator()				( texture_named_instance const& arg_0 ) const { /* no source */ }

	inline				~find_texture_predicate	( ) { /* no source */ }

	/* 0x0000 */	res_texture_ptr		m_texture;
}; // struct res_effect::push_texture_unique::__l2::find_texture_predicate

STATIC_SIZE_ASSERT(res_effect::push_texture_unique::__l2::find_texture_predicate, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_EFFECT_PUSH_TEXTURE_UNIQUE___L2_FIND_TEXTURE_PREDICATE_H_INCLUDED

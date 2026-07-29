////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_REMOVE_MODEL_IF_NOT_LOD_PREDICATE_H_INCLUDED
#define RENDER_REMOVE_MODEL_IF_NOT_LOD_PREDICATE_H_INCLUDED

/* FORWARD REFS */
class vostok::render::render_surface_instance;

namespace vostok {
namespace render {

struct remove_model_if_not_lod_predicate {
	inline				remove_model_if_not_lod_predicate	( ) { /* no source */ }
	inline	explicit	remove_model_if_not_lod_predicate	( u32 arg_0 ) { /* no source */ }

			bool		operator()							( render_surface_instance* in_model );

	/* 0x0000 */	u32		m_shader_lod_index;
}; // struct remove_model_if_not_lod_predicate

STATIC_SIZE_ASSERT(remove_model_if_not_lod_predicate, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_REMOVE_MODEL_IF_NOT_LOD_PREDICATE_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_REMOVE_MODEL_IF_IN_FRUSTUM_PREDICATE_H_INCLUDED
#define RENDER_REMOVE_MODEL_IF_IN_FRUSTUM_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::math::frustum;

/* FORWARD REFS */
class vostok::render::render_surface_instance;

namespace vostok {
namespace render {

struct remove_model_if_in_frustum_predicate {
	inline	explicit	remove_model_if_in_frustum_predicate( math::frustum& arg_0 ) { /* no source */ }

			bool		operator()	( render_surface_instance* in_model );

	/* 0x0000 */	math::frustum*		m_frustum;
}; // struct remove_model_if_in_frustum_predicate

STATIC_SIZE_ASSERT(remove_model_if_in_frustum_predicate, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_REMOVE_MODEL_IF_IN_FRUSTUM_PREDICATE_H_INCLUDED

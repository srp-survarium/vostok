////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_REMOVE_MODEL_SKELETAL_FILTER_PREDICATE_H_INCLUDED
#define RENDER_REMOVE_MODEL_SKELETAL_FILTER_PREDICATE_H_INCLUDED

/* FORWARD REFS */
class vostok::render::render_surface_instance;

namespace vostok {
namespace render {

struct remove_model_skeletal_filter_predicate {
	inline	explicit	remove_model_skeletal_filter_predicate( bool arg_0 ) { /* no source */ }

			bool		operator()	( render_surface_instance* in_model );

	/* 0x0000 */	bool	m_inverse_flag;
}; // struct remove_model_skeletal_filter_predicate

STATIC_SIZE_ASSERT(remove_model_skeletal_filter_predicate, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_REMOVE_MODEL_SKELETAL_FILTER_PREDICATE_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_REMOVE_MODEL_FILTER_PREDICATE_H_INCLUDED
#define RENDER_REMOVE_MODEL_FILTER_PREDICATE_H_INCLUDED

/* FORWARD REFS */
class vostok::render::render_surface_instance;

namespace vostok {
namespace render {

struct remove_model_filter_predicate {
	inline			remove_model_filter_predicate	( ) { /* no source */ }
	inline			remove_model_filter_predicate	( bool arg_0, bool arg_1, bool arg_2 ) { /* no source */ }

	inline	bool	operator()						( render_surface_instance* arg_0 ) { /* no source */ }

	/* 0x0000 */	bool	m_need_static;
	/* 0x0001 */	bool	m_need_skeletal;
	/* 0x0002 */	bool	m_need_translucency;
}; // struct remove_model_filter_predicate

STATIC_SIZE_ASSERT(remove_model_filter_predicate, 0x3);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_REMOVE_MODEL_FILTER_PREDICATE_H_INCLUDED

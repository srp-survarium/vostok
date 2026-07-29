////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_REMOVE_MODEL_IF_OLT_PREDICATE_H_INCLUDED
#define RENDER_REMOVE_MODEL_IF_OLT_PREDICATE_H_INCLUDED

/* FORWARD REFS */
class vostok::render::render_surface_instance;

namespace vostok {
namespace render {

struct remove_model_if_olt_predicate {
	inline				remove_model_if_olt_predicate	( ) { /* no source */ }
	inline	explicit	remove_model_if_olt_predicate	( u32 arg_0 ) { /* no source */ }

	inline	bool		operator()						( render_surface_instance* arg_0 ) { /* no source */ }

	/* 0x0000 */	bool	m_use_olt;
}; // struct remove_model_if_olt_predicate

STATIC_SIZE_ASSERT(remove_model_if_olt_predicate, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_REMOVE_MODEL_IF_OLT_PREDICATE_H_INCLUDED

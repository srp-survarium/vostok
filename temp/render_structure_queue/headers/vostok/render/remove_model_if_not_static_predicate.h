////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_REMOVE_MODEL_IF_NOT_STATIC_PREDICATE_H_INCLUDED
#define RENDER_REMOVE_MODEL_IF_NOT_STATIC_PREDICATE_H_INCLUDED

/* FORWARD REFS */
class vostok::render::render_surface_instance;

namespace vostok {
namespace render {

struct remove_model_if_not_static_predicate {
	bool	operator()	( render_surface_instance* in_model );
}; // struct remove_model_if_not_static_predicate

STATIC_SIZE_ASSERT(remove_model_if_not_static_predicate, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_REMOVE_MODEL_IF_NOT_STATIC_PREDICATE_H_INCLUDED

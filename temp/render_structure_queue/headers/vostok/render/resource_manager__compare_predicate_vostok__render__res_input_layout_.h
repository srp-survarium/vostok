////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RESOURCE_MANAGER_COMPARE_PREDICATE_H_INCLUDED
#define RENDER_RESOURCE_MANAGER_COMPARE_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::res_input_layout const* const;

namespace vostok {
namespace render {

struct resource_manager::compare_predicate< res_input_layout > {
	inline	bool	operator()	( res_input_layout const* const arg_0, res_input_layout const* const arg_1 ) const { /* no source */ }
}; // struct resource_manager::compare_predicate< res_input_layout >

STATIC_SIZE_ASSERT(resource_manager::compare_predicate< res_input_layout >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RESOURCE_MANAGER_COMPARE_PREDICATE_H_INCLUDED

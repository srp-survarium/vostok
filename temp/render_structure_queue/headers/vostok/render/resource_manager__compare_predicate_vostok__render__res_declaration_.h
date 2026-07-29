////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RESOURCE_MANAGER_COMPARE_PREDICATE_H_INCLUDED
#define RENDER_RESOURCE_MANAGER_COMPARE_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::res_declaration const* const;

namespace vostok {
namespace render {

struct resource_manager::compare_predicate< res_declaration > {
	inline	bool	operator()	( res_declaration const* const arg_0, res_declaration const* const arg_1 ) const { /* no source */ }
}; // struct resource_manager::compare_predicate< res_declaration >

STATIC_SIZE_ASSERT(resource_manager::compare_predicate< res_declaration >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RESOURCE_MANAGER_COMPARE_PREDICATE_H_INCLUDED

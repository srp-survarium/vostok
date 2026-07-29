////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RESOURCE_MANAGER_COMPARE_PREDICATE_H_INCLUDED
#define RENDER_RESOURCE_MANAGER_COMPARE_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::res_signature const* const;

namespace vostok {
namespace render {

struct resource_manager::compare_predicate< res_signature > {
	inline	bool	operator()	( res_signature const* const left, res_signature const* const right ) const { /* no source */ }
}; // struct resource_manager::compare_predicate< res_signature >

STATIC_SIZE_ASSERT(resource_manager::compare_predicate< res_signature >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RESOURCE_MANAGER_COMPARE_PREDICATE_H_INCLUDED

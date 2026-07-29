////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RESOURCE_MANAGER_COMPARE_MEMBER_PREDICATE_H_INCLUDED
#define RENDER_RESOURCE_MANAGER_COMPARE_MEMBER_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::res_geometry const* const;

namespace vostok {
namespace render {

struct resource_manager::compare_member_predicate< res_geometry > {
	inline	bool	operator()	( res_geometry const* const left, res_geometry const* const right ) const { /* no source */ }
}; // struct resource_manager::compare_member_predicate< res_geometry >

STATIC_SIZE_ASSERT(resource_manager::compare_member_predicate< res_geometry >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RESOURCE_MANAGER_COMPARE_MEMBER_PREDICATE_H_INCLUDED

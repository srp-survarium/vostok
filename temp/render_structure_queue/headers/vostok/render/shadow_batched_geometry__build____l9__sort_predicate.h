////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADOW_BATCHED_GEOMETRY_BUILD___L9_SORT_PREDICATE_H_INCLUDED
#define RENDER_SHADOW_BATCHED_GEOMETRY_BUILD___L9_SORT_PREDICATE_H_INCLUDED

/* FORWARD REFS */
class vostok::render::shadow_batched_geometry::build::__l2::surface_set;

namespace vostok {
namespace render {

struct shadow_batched_geometry::build::__l9::sort_predicate {
	inline	bool	operator()	(
						shadow_batched_geometry::build::__l2::surface_set const&	arg_0,
						shadow_batched_geometry::build::__l2::surface_set const&	arg_1
					) const { /* no source */ }
}; // struct shadow_batched_geometry::build::__l9::sort_predicate

STATIC_SIZE_ASSERT(shadow_batched_geometry::build::__l9::sort_predicate, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADOW_BATCHED_GEOMETRY_BUILD___L9_SORT_PREDICATE_H_INCLUDED

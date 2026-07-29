////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_PREDICATE_RESULT_H_INCLUDED
#define RENDER_PREDICATE_RESULT_H_INCLUDED

/* INCLUDES */
struct vostok::collision::ray_triangle_result;

namespace vostok {
namespace render {

struct predicate_result {
	inline			predicate_result( ) { /* no source */ }

	inline	bool	predicate		( collision::ray_triangle_result const& arg_0 ) { /* no source */ }

	/* 0x0000 */	collision::ray_triangle_result		stored_result;
}; // struct predicate_result

STATIC_SIZE_ASSERT(predicate_result, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_PREDICATE_RESULT_H_INCLUDED

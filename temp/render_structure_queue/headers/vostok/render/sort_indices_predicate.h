////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SORT_INDICES_PREDICATE_H_INCLUDED
#define RENDER_SORT_INDICES_PREDICATE_H_INCLUDED

/* INCLUDES */
struct vostok::render::grass_patch;

/* FORWARD REFS */
class vostok::render::grass_patch::sort_info;

namespace vostok {
namespace render {

struct sort_indices_predicate {
	inline			sort_indices_predicate	( grass_patch* arg_0, float3 const& arg_1 ) { /* no source */ }

	inline	bool	operator()				( grass_patch::sort_info const& arg_0, grass_patch::sort_info const& arg_1 ) const { /* no source */ }

private:
	/* 0x0000 */	grass_patch*	m_patch;
	/* 0x0004 */	float3			m_view_pos;
}; // struct sort_indices_predicate

STATIC_SIZE_ASSERT(sort_indices_predicate, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SORT_INDICES_PREDICATE_H_INCLUDED

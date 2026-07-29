////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SORT_BY_DISTANCE_PREDICATE_H_INCLUDED
#define RENDER_SORT_BY_DISTANCE_PREDICATE_H_INCLUDED

/* FORWARD REFS */
class vostok::render::render_surface_instance;

namespace vostok {
namespace render {

struct sort_by_distance_predicate {
	inline			sort_by_distance_predicate	( float3 const& arg_0, bool arg_1 ) { /* no source */ }

			bool	operator()					( render_surface_instance const* left, render_surface_instance const* right ) const;

	/* 0x0000 */	float3		m_eye_position;
	/* 0x000c */	bool		m_from_near_to_far;
}; // struct sort_by_distance_predicate

STATIC_SIZE_ASSERT(sort_by_distance_predicate, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SORT_BY_DISTANCE_PREDICATE_H_INCLUDED

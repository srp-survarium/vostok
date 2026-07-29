////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SORT_GRASS_PATCH_PREDICATE_H_INCLUDED
#define RENDER_SORT_GRASS_PATCH_PREDICATE_H_INCLUDED

/* FORWARD REFS */
class vostok::render::grass_patch;

namespace vostok {
namespace render {

struct sort_grass_patch_predicate {
	inline	explicit	sort_grass_patch_predicate	( float3 const& arg_0 ) { /* no source */ }

			bool		operator()					( grass_patch const* left, grass_patch const* right ) const;

private:
	/* 0x0000 */	float3		m_view_pos;
}; // struct sort_grass_patch_predicate

STATIC_SIZE_ASSERT(sort_grass_patch_predicate, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SORT_GRASS_PATCH_PREDICATE_H_INCLUDED

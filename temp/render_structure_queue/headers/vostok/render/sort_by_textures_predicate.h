////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SORT_BY_TEXTURES_PREDICATE_H_INCLUDED
#define RENDER_SORT_BY_TEXTURES_PREDICATE_H_INCLUDED

/* FORWARD REFS */
class vostok::render::render_surface_instance;

namespace vostok {
namespace render {

struct sort_by_textures_predicate {
	inline	bool	operator()	( render_surface_instance const* arg_0, render_surface_instance const* arg_1 ) const { /* no source */ }
}; // struct sort_by_textures_predicate

STATIC_SIZE_ASSERT(sort_by_textures_predicate, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SORT_BY_TEXTURES_PREDICATE_H_INCLUDED

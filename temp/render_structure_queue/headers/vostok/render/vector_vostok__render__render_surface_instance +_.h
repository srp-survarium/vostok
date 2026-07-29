////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::render_surface_instance *,vostok::render::std_allocator<vostok::render::render_surface_instance *> >
	stlp_std::vector<vostok::render::render_surface_instance * >;
class vostok::render::render_surface_instance *;

/* FORWARD REFS */
class vostok::render::render_surface_instance;
class vostok::render::render_surface_instance*;

namespace vostok {
namespace render {

class vector< render_surface_instance* > : public std::vector< render_surface_instance* > {
public:
	inline										vector<vostok::render::render_surface_instance *>( ) { /* no source */ }
	inline										vector<vostok::render::render_surface_instance *>( u32 arg_0, render_surface_instance* const& arg_1 ) { /* no source */ }
	inline	explicit							vector<vostok::render::render_surface_instance *>( u32 arg_0 ) { /* no source */ }

	inline	render_surface_instance*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	render_surface_instance* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32									size		( ) const { /* no source */ }

	inline										~vector<vostok::render::render_surface_instance *>( ) { /* no source */ }
}; // class vector< render_surface_instance* >

STATIC_SIZE_ASSERT(vector< render_surface_instance* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED

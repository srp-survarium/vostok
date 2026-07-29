////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::scene_view *,vostok::render::std_allocator<vostok::render::scene_view *> >
	stlp_std::vector<vostok::render::scene_view * >;
class vostok::render::scene_view *;

/* FORWARD REFS */
class vostok::render::scene_view;
class vostok::render::scene_view*;

namespace vostok {
namespace render {

class vector< scene_view* > : public std::vector< scene_view* > {
public:
	inline							vector<vostok::render::scene_view *>( ) { /* no source */ }
	inline							vector<vostok::render::scene_view *>( u32 arg_0, scene_view* const& arg_1 ) { /* no source */ }
	inline	explicit				vector<vostok::render::scene_view *>( u32 arg_0 ) { /* no source */ }

	inline	scene_view*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	scene_view* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32						size		( ) const { /* no source */ }

	inline							~vector<vostok::render::scene_view *>( ) { /* no source */ }
}; // class vector< scene_view* >

STATIC_SIZE_ASSERT(vector< scene_view* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::scene *,vostok::render::std_allocator<vostok::render::scene *> >
	stlp_std::vector<vostok::render::scene * >;
class vostok::render::scene *;

/* FORWARD REFS */
class vostok::render::scene;
class vostok::render::scene*;

namespace vostok {
namespace render {

class vector< scene* > : public std::vector< scene* > {
public:
	inline						vector<vostok::render::scene *>	( ) { /* no source */ }
	inline						vector<vostok::render::scene *>	( u32 arg_0, scene* const& arg_1 ) { /* no source */ }
	inline	explicit			vector<vostok::render::scene *>	( u32 arg_0 ) { /* no source */ }

	inline	scene*&				operator[]						( u32 arg_0 ) { /* no source */ }
	inline	scene* const&		operator[]						( u32 arg_0 ) const { /* no source */ }

	inline	u32					size							( ) const { /* no source */ }

	inline						~vector<vostok::render::scene *>( ) { /* no source */ }
}; // class vector< scene* >

STATIC_SIZE_ASSERT(vector< scene* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED

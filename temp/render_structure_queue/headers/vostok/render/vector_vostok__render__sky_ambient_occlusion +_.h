////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::sky_ambient_occlusion *,vostok::render::std_allocator<vostok::render::sky_ambient_occlusion *> >
	stlp_std::vector<vostok::render::sky_ambient_occlusion * >;
class vostok::render::sky_ambient_occlusion *;

/* FORWARD REFS */
class vostok::render::sky_ambient_occlusion;
class vostok::render::sky_ambient_occlusion*;

namespace vostok {
namespace render {

class vector< sky_ambient_occlusion* > : public std::vector< sky_ambient_occlusion* > {
public:
	inline										vector<vostok::render::sky_ambient_occlusion *>( ) { /* no source */ }
	inline										vector<vostok::render::sky_ambient_occlusion *>( u32 arg_0, sky_ambient_occlusion* const& arg_1 ) { /* no source */ }
	inline	explicit							vector<vostok::render::sky_ambient_occlusion *>( u32 arg_0 ) { /* no source */ }

	inline	sky_ambient_occlusion*&				operator[]	( u32 arg_0 ) { /* no source */ }
	inline	sky_ambient_occlusion* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32									size		( ) const { /* no source */ }

	inline										~vector<vostok::render::sky_ambient_occlusion *>( ) { /* no source */ }
}; // class vector< sky_ambient_occlusion* >

STATIC_SIZE_ASSERT(vector< sky_ambient_occlusion* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED

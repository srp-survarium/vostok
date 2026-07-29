////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::culling::sector_double_query_preventer::frustum_image,vostok::render::std_allocator<vostok::render::culling::sector_double_query_preventer::frustum_image> >
	stlp_std::vector<vostok::render::culling::sector_double_query_preventer::frustum_image >;
class vostok::render::culling::sector_double_query_preventer::frustum_image;

namespace vostok {
namespace render {

class vector< culling::sector_double_query_preventer::frustum_image > : public std::vector< culling::sector_double_query_preventer::frustum_image > {
public:
	inline				vector<vostok::render::culling::sector_double_query_preventer::frustum_image>( ) { /* no source */ }
	inline				vector<vostok::render::culling::sector_double_query_preventer::frustum_image>( u32 arg_0, culling::sector_double_query_preventer::frustum_image const& arg_1 ) { /* no source */ }
	inline	explicit	vector<vostok::render::culling::sector_double_query_preventer::frustum_image>( u32 arg_0 ) { /* no source */ }

	inline	culling::sector_double_query_preventer::frustum_image&	operator[]	( u32 arg_0 ) { /* no source */ }
	inline	culling::sector_double_query_preventer::frustum_image const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32			size		( ) const { /* no source */ }

	inline				~vector<vostok::render::culling::sector_double_query_preventer::frustum_image>( ) { /* no source */ }
}; // class vector< culling::sector_double_query_preventer::frustum_image >

STATIC_SIZE_ASSERT(vector< culling::sector_double_query_preventer::frustum_image >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::math::frustum,vostok::render::std_allocator<vostok::math::frustum> >
	stlp_std::vector<vostok::math::frustum >;
class vostok::math::frustum;

namespace vostok {
namespace render {

class vector< math::frustum > : public std::vector< math::frustum > {
public:
	inline							vector<vostok::math::frustum>	( ) { /* no source */ }
	inline							vector<vostok::math::frustum>	( u32 arg_0, math::frustum const& arg_1 ) { /* no source */ }
	inline	explicit				vector<vostok::math::frustum>	( u32 arg_0 ) { /* no source */ }

	inline	math::frustum&			operator[]						( u32 arg_0 ) { /* no source */ }
	inline	math::frustum const&	operator[]						( u32 arg_0 ) const { /* no source */ }

	inline	u32						size							( ) const { /* no source */ }

	inline							~vector<vostok::math::frustum>	( ) { /* no source */ }
}; // class vector< math::frustum >

STATIC_SIZE_ASSERT(vector< math::frustum >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED

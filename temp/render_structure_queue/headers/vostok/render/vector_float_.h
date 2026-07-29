////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<float,vostok::render::std_allocator<float> >
	stlp_std::vector<float >;
class float;

namespace vostok {
namespace render {

class vector< float > : public std::vector< float > {
public:
	inline					vector<float>	( ) { /* no source */ }
	inline					vector<float>	( u32 arg_0, float const& arg_1 ) { /* no source */ }
	inline	explicit		vector<float>	( u32 arg_0 ) { /* no source */ }

	inline	float&			operator[]		( u32 arg_0 ) { /* no source */ }
	inline	float const&	operator[]		( u32 arg_0 ) const { /* no source */ }

	inline	u32				size			( ) const { /* no source */ }

	inline					~vector<float>	( ) { /* no source */ }
}; // class vector< float >

STATIC_SIZE_ASSERT(vector< float >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED

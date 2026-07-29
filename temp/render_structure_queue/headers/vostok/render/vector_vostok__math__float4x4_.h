////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::math::float4x4,vostok::render::std_allocator<vostok::math::float4x4> >
	stlp_std::vector<vostok::math::float4x4 >;
class vostok::math::float4x4;

namespace vostok {
namespace render {

class vector< float4x4 > : public std::vector< float4x4 > {
public:
	inline						vector<vostok::math::float4x4>	( ) { /* no source */ }
	inline						vector<vostok::math::float4x4>	( u32 arg_0, float4x4 const& arg_1 ) { /* no source */ }
	inline	explicit			vector<vostok::math::float4x4>	( u32 arg_0 ) { /* no source */ }

	inline	float4x4&			operator[]						( u32 arg_0 ) { /* no source */ }
	inline	float4x4 const&		operator[]						( u32 arg_0 ) const { /* no source */ }

	inline	u32					size							( ) const { /* no source */ }

	inline						~vector<vostok::math::float4x4>	( ) { /* no source */ }
}; // class vector< float4x4 >

STATIC_SIZE_ASSERT(vector< float4x4 >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED

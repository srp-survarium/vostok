////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::math::float3,survarium::std_allocator<vostok::math::float3> >
	stlp_std::vector<vostok::math::float3 >;
class vostok::math::float3;

namespace survarium {

class vector< float3 > : public std::vector< float3 > {
public:
	inline						vector<vostok::math::float3>	( ) { /* no source */ }
	inline						vector<vostok::math::float3>	( u32 arg_0, float3 const& arg_1 ) { /* no source */ }
	inline	explicit			vector<vostok::math::float3>	( u32 arg_0 ) { /* no source */ }

	inline	float3&				operator[]						( u32 arg_0 ) { /* no source */ }
	inline	float3 const&		operator[]						( u32 arg_0 ) const { /* no source */ }

	inline	u32					size							( ) const { /* no source */ }

	inline						~vector<vostok::math::float3>	( ) { /* no source */ }
}; // class vector< float3 >

STATIC_SIZE_ASSERT(vector< float3 >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

namespace survarium {

class std_allocator< float3 > {
public:
	inline						std_allocator<vostok::math::float3>	( ) { /* no source */ }

	inline	float3*				address								( float3& arg_0 ) const { /* no source */ }
	inline	float3 const*		address								( float3 const& arg_0 ) const { /* no source */ }

	inline	float3*				allocate							( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	float3*				allocate							( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	float3*				_M_allocate							( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	float3*				_M_allocate							( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*				__charalloc							( u32 arg_0 ) { /* no source */ }

	inline	void				deallocate							( float3* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void				deallocate							( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void				construct							( float3* arg_0, float3 const& arg_1 ) { /* no source */ }

	inline	void				destroy								( float3* arg_0 ) { /* no source */ }

	inline	u32					max_size							( ) const { /* no source */ }
}; // class std_allocator< float3 >

STATIC_SIZE_ASSERT(std_allocator< float3 >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED

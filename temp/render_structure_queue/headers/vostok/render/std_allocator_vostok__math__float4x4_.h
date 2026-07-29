////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

namespace vostok {
namespace render {

class std_allocator< float4x4 > {
public:
	inline						std_allocator<vostok::math::float4x4>( ) { /* no source */ }

	inline	float4x4*			address		( float4x4& arg_0 ) const { /* no source */ }
	inline	float4x4 const*		address		( float4x4 const& arg_0 ) const { /* no source */ }

	inline	float4x4*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	float4x4*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	float4x4*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	float4x4*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*				__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void				deallocate	( float4x4* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void				deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void				construct	( float4x4* arg_0, float4x4 const& arg_1 ) { /* no source */ }

	inline	void				destroy		( float4x4* arg_0 ) { /* no source */ }

	inline	u32					max_size	( ) const { /* no source */ }
}; // class std_allocator< float4x4 >

STATIC_SIZE_ASSERT(std_allocator< float4x4 >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED

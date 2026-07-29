////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

namespace vostok {
namespace render {

class std_allocator< _D3DVERTEXELEMENT9 > {
public:
	inline									std_allocator<_D3DVERTEXELEMENT9>	( ) { /* no source */ }

	inline	_D3DVERTEXELEMENT9*				address								( _D3DVERTEXELEMENT9& arg_0 ) const { /* no source */ }
	inline	_D3DVERTEXELEMENT9 const*		address								( _D3DVERTEXELEMENT9 const& arg_0 ) const { /* no source */ }

	inline	_D3DVERTEXELEMENT9*				allocate							( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	_D3DVERTEXELEMENT9*				allocate							( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	_D3DVERTEXELEMENT9*				_M_allocate							( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	_D3DVERTEXELEMENT9*				_M_allocate							( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*							__charalloc							( u32 arg_0 ) { /* no source */ }

	inline	void							deallocate							( _D3DVERTEXELEMENT9* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void							deallocate							( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void							construct							( _D3DVERTEXELEMENT9* arg_0, _D3DVERTEXELEMENT9 const& arg_1 ) { /* no source */ }

	inline	void							destroy								( _D3DVERTEXELEMENT9* arg_0 ) { /* no source */ }

	inline	u32								max_size							( ) const { /* no source */ }
}; // class std_allocator< _D3DVERTEXELEMENT9 >

STATIC_SIZE_ASSERT(std_allocator< _D3DVERTEXELEMENT9 >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED

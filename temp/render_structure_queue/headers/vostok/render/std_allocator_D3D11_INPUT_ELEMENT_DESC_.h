////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

namespace vostok {
namespace render {

class std_allocator< D3D11_INPUT_ELEMENT_DESC > {
public:
	inline										std_allocator<D3D11_INPUT_ELEMENT_DESC>( ) { /* no source */ }

	inline	D3D11_INPUT_ELEMENT_DESC*			address		( D3D11_INPUT_ELEMENT_DESC& arg_0 ) const { /* no source */ }
	inline	D3D11_INPUT_ELEMENT_DESC const*		address		( D3D11_INPUT_ELEMENT_DESC const& arg_0 ) const { /* no source */ }

	inline	D3D11_INPUT_ELEMENT_DESC*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	D3D11_INPUT_ELEMENT_DESC*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	D3D11_INPUT_ELEMENT_DESC*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	D3D11_INPUT_ELEMENT_DESC*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*								__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void								deallocate	( D3D11_INPUT_ELEMENT_DESC* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void								deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void								construct	( D3D11_INPUT_ELEMENT_DESC* arg_0, D3D11_INPUT_ELEMENT_DESC const& arg_1 ) { /* no source */ }

	inline	void								destroy		( D3D11_INPUT_ELEMENT_DESC* arg_0 ) { /* no source */ }

	inline	u32									max_size	( ) const { /* no source */ }
}; // class std_allocator< D3D11_INPUT_ELEMENT_DESC >

STATIC_SIZE_ASSERT(std_allocator< D3D11_INPUT_ELEMENT_DESC >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED

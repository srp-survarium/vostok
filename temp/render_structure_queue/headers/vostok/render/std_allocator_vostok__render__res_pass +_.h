////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::res_pass;
class vostok::render::res_pass*;

namespace vostok {
namespace render {

class std_allocator< res_pass* > {
public:
	inline						std_allocator<vostok::render::res_pass *>( ) { /* no source */ }

	inline	res_pass**			address		( res_pass*& arg_0 ) const { /* no source */ }
	inline	res_pass* const*	address		( res_pass* const& arg_0 ) const { /* no source */ }

	inline	res_pass**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	res_pass**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	res_pass**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	res_pass**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*				__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void				deallocate	( res_pass** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void				deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void				construct	( res_pass** arg_0, res_pass* const& arg_1 ) { /* no source */ }

	inline	void				destroy		( res_pass** arg_0 ) { /* no source */ }

	inline	u32					max_size	( ) const { /* no source */ }
}; // class std_allocator< res_pass* >

STATIC_SIZE_ASSERT(std_allocator< res_pass* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED

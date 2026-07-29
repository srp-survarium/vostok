////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::res_signature;
class vostok::render::res_signature*;

namespace vostok {
namespace render {

class std_allocator< res_signature* > {
public:
	inline								std_allocator<vostok::render::res_signature *>( ) { /* no source */ }

	inline	res_signature**				address		( res_signature*& arg_0 ) const { /* no source */ }
	inline	res_signature* const*		address		( res_signature* const& arg_0 ) const { /* no source */ }

	inline	res_signature**				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	res_signature**				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	res_signature**				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	res_signature**				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( res_signature** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( res_signature** arg_0, res_signature* const& arg_1 ) { /* no source */ }

	inline	void						destroy		( res_signature** arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< res_signature* >

STATIC_SIZE_ASSERT(std_allocator< res_signature* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED

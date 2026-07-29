////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::fixed_string<32>;

namespace vostok {
namespace render {

class std_allocator< fixed_string< 32 > > {
public:
	inline									std_allocator<vostok::fixed_string<32> >( ) { /* no source */ }

	inline	fixed_string< 32 >*				address		( fixed_string< 32 >& arg_0 ) const { /* no source */ }
	inline	fixed_string< 32 > const*		address		( fixed_string< 32 > const& arg_0 ) const { /* no source */ }

	inline	fixed_string< 32 >*				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	fixed_string< 32 >*				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	fixed_string< 32 >*				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	fixed_string< 32 >*				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*							__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void							deallocate	( fixed_string< 32 >* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void							deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void							construct	( fixed_string< 32 >* arg_0, fixed_string< 32 > const& arg_1 ) { /* no source */ }

	inline	void							destroy		( fixed_string< 32 >* arg_0 ) { /* no source */ }

	inline	u32								max_size	( ) const { /* no source */ }
}; // class std_allocator< fixed_string< 32 > >

STATIC_SIZE_ASSERT(std_allocator< fixed_string< 32 > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::fixed_string<16>;

namespace survarium {

class std_allocator< fixed_string< 16 > > {
public:
	inline									std_allocator<vostok::fixed_string<16> >( ) { /* no source */ }

	inline	fixed_string< 16 >*				address		( fixed_string< 16 >& arg_0 ) const { /* no source */ }
	inline	fixed_string< 16 > const*		address		( fixed_string< 16 > const& arg_0 ) const { /* no source */ }

	inline	fixed_string< 16 >*				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	fixed_string< 16 >*				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	fixed_string< 16 >*				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	fixed_string< 16 >*				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*							__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void							deallocate	( fixed_string< 16 >* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void							deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void							construct	( fixed_string< 16 >* arg_0, fixed_string< 16 > const& arg_1 ) { /* no source */ }

	inline	void							destroy		( fixed_string< 16 >* arg_0 ) { /* no source */ }

	inline	u32								max_size	( ) const { /* no source */ }
}; // class std_allocator< fixed_string< 16 > >

STATIC_SIZE_ASSERT(std_allocator< fixed_string< 16 > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED

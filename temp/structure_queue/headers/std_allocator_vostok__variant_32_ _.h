////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::variant<32>;

namespace survarium {

class std_allocator< variant< 32 > > {
public:
	inline							std_allocator<vostok::variant<32> >	( ) { /* no source */ }

	inline	variant< 32 >*			address								( variant< 32 >& arg_0 ) const { /* no source */ }
	inline	variant< 32 > const*	address								( variant< 32 > const& arg_0 ) const { /* no source */ }

	inline	variant< 32 >*			allocate							( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	variant< 32 >*			allocate							( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	variant< 32 >*			_M_allocate							( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	variant< 32 >*			_M_allocate							( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*					__charalloc							( u32 arg_0 ) { /* no source */ }

	inline	void					deallocate							( variant< 32 >* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void					deallocate							( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void					construct							( variant< 32 >* arg_0, variant< 32 > const& arg_1 ) { /* no source */ }

	inline	void					destroy								( variant< 32 >* arg_0 ) { /* no source */ }

	inline	u32						max_size							( ) const { /* no source */ }
}; // class std_allocator< variant< 32 > >

STATIC_SIZE_ASSERT(std_allocator< variant< 32 > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* INCLUDES */
class survarium::ladder;

namespace survarium {

class std_allocator< ladder_ptr > {
public:
	inline							std_allocator<vostok::resources::resource_ptr<survarium::ladder,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }

	inline	ladder_ptr*				address		( ladder_ptr& arg_0 ) const { /* no source */ }
	inline	ladder_ptr const*		address		( ladder_ptr const& arg_0 ) const { /* no source */ }

	inline	ladder_ptr*				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	ladder_ptr*				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	ladder_ptr*				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	ladder_ptr*				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*					__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void					deallocate	( ladder_ptr* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void					deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void					construct	( ladder_ptr* arg_0, ladder_ptr const& arg_1 ) { /* no source */ }

	inline	void					destroy		( ladder_ptr* arg_0 ) { /* no source */ }

	inline	u32						max_size	( ) const { /* no source */ }
}; // class std_allocator< ladder_ptr >

STATIC_SIZE_ASSERT(std_allocator< ladder_ptr >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED

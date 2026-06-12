////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class survarium::zone_group;
class survarium::zone_group*;

namespace survarium {

class std_allocator< zone_group* > {
public:
	inline							std_allocator<survarium::zone_group *>( ) { /* no source */ }

	inline	zone_group**			address		( zone_group*& arg_0 ) const { /* no source */ }
	inline	zone_group* const*		address		( zone_group* const& arg_0 ) const { /* no source */ }

	inline	zone_group**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	zone_group**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	zone_group**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	zone_group**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*					__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void					deallocate	( zone_group** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void					deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void					construct	( zone_group** arg_0, zone_group* const& arg_1 ) { /* no source */ }

	inline	void					destroy		( zone_group** arg_0 ) { /* no source */ }

	inline	u32						max_size	( ) const { /* no source */ }
}; // class std_allocator< zone_group* >

STATIC_SIZE_ASSERT(std_allocator< zone_group* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED

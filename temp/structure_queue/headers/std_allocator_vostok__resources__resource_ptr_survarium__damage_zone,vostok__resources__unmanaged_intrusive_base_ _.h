////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* INCLUDES */
class survarium::damage_zone;

namespace survarium {

class std_allocator< damage_zone_ptr > {
public:
	inline								std_allocator<vostok::resources::resource_ptr<survarium::damage_zone,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }

	inline	damage_zone_ptr*			address		( damage_zone_ptr& arg_0 ) const { /* no source */ }
	inline	damage_zone_ptr const*		address		( damage_zone_ptr const& arg_0 ) const { /* no source */ }

	inline	damage_zone_ptr*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	damage_zone_ptr*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	damage_zone_ptr*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	damage_zone_ptr*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( damage_zone_ptr* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( damage_zone_ptr* arg_0, damage_zone_ptr const& arg_1 ) { /* no source */ }

	inline	void						destroy		( damage_zone_ptr* arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< damage_zone_ptr >

STATIC_SIZE_ASSERT(std_allocator< damage_zone_ptr >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED

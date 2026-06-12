////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
enum survarium::profile_slot_enum;

namespace survarium {

class std_allocator< enum profile_slot_enum > {
public:
	inline								std_allocator<enum survarium::profile_slot_enum>( ) { /* no source */ }

	inline	profile_slot_enum*			address		( profile_slot_enum& arg_0 ) const { /* no source */ }
	inline	profile_slot_enum const*	address		( profile_slot_enum const& arg_0 ) const { /* no source */ }

	inline	profile_slot_enum*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	profile_slot_enum*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	profile_slot_enum*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	profile_slot_enum*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( profile_slot_enum* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( profile_slot_enum* arg_0, profile_slot_enum const& arg_1 ) { /* no source */ }

	inline	void						destroy		( profile_slot_enum* arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< enum profile_slot_enum >

STATIC_SIZE_ASSERT(std_allocator< enum profile_slot_enum >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED

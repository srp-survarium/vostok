////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class survarium::collision_geometry;
class survarium::collision_geometry*;

namespace survarium {

class std_allocator< collision_geometry* > {
public:
	inline									std_allocator<survarium::collision_geometry *>( ) { /* no source */ }

	inline	collision_geometry**			address		( collision_geometry*& arg_0 ) const { /* no source */ }
	inline	collision_geometry* const*		address		( collision_geometry* const& arg_0 ) const { /* no source */ }

	inline	collision_geometry**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	collision_geometry**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	collision_geometry**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	collision_geometry**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*							__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void							deallocate	( collision_geometry** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void							deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void							construct	( collision_geometry** arg_0, collision_geometry* const& arg_1 ) { /* no source */ }

	inline	void							destroy		( collision_geometry** arg_0 ) { /* no source */ }

	inline	u32								max_size	( ) const { /* no source */ }
}; // class std_allocator< collision_geometry* >

STATIC_SIZE_ASSERT(std_allocator< collision_geometry* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED

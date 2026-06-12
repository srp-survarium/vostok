////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::physics::base_physics_object;
class vostok::physics::base_physics_object*;

namespace survarium {

class std_allocator< physics::base_physics_object* > {
public:
	inline										std_allocator<vostok::physics::base_physics_object *>( ) { /* no source */ }

	inline	physics::base_physics_object**		address		( physics::base_physics_object*& arg_0 ) const { /* no source */ }
	inline	physics::base_physics_object* const*	address		( physics::base_physics_object* const& arg_0 ) const { /* no source */ }

	inline	physics::base_physics_object**		allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	physics::base_physics_object**		allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	physics::base_physics_object**		_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	physics::base_physics_object**		_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*								__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void								deallocate	( physics::base_physics_object** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void								deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void								construct	( physics::base_physics_object** arg_0, physics::base_physics_object* const& arg_1 ) { /* no source */ }

	inline	void								destroy		( physics::base_physics_object** arg_0 ) { /* no source */ }

	inline	u32									max_size	( ) const { /* no source */ }
}; // class std_allocator< physics::base_physics_object* >

STATIC_SIZE_ASSERT(std_allocator< physics::base_physics_object* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class survarium::game_action_id;

namespace survarium {

class std_allocator< enum game_action_id > {
public:
	inline								std_allocator<enum survarium::game_action_id>( ) { /* no source */ }

	inline	game_action_id*				address		( game_action_id& arg_0 ) const { /* no source */ }
	inline	game_action_id const*		address		( game_action_id const& arg_0 ) const { /* no source */ }

	inline	game_action_id*				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	game_action_id*				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	game_action_id*				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	game_action_id*				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( game_action_id* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( game_action_id* arg_0, game_action_id const& arg_1 ) { /* no source */ }

	inline	void						destroy		( game_action_id* arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< enum game_action_id >

STATIC_SIZE_ASSERT(std_allocator< enum game_action_id >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED

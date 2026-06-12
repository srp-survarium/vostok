////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class survarium::player_actions_subscriber;
class survarium::player_actions_subscriber*;

namespace survarium {

class std_allocator< player_actions_subscriber* > {
public:
	inline											std_allocator<survarium::player_actions_subscriber *>( ) { /* no source */ }

	inline	player_actions_subscriber**				address		( player_actions_subscriber*& arg_0 ) const { /* no source */ }
	inline	player_actions_subscriber* const*		address		( player_actions_subscriber* const& arg_0 ) const { /* no source */ }

	inline	player_actions_subscriber**				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	player_actions_subscriber**				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	player_actions_subscriber**				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	player_actions_subscriber**				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*									__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void									deallocate	( player_actions_subscriber** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void									deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void									construct	( player_actions_subscriber** arg_0, player_actions_subscriber* const& arg_1 ) { /* no source */ }

	inline	void									destroy		( player_actions_subscriber** arg_0 ) { /* no source */ }

	inline	u32										max_size	( ) const { /* no source */ }
}; // class std_allocator< player_actions_subscriber* >

STATIC_SIZE_ASSERT(std_allocator< player_actions_subscriber* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED

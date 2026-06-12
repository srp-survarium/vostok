////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class survarium::game_world::bullet_tracer;

namespace survarium {

class std_allocator< game_world::bullet_tracer > {
public:
	inline										std_allocator<survarium::game_world::bullet_tracer>( ) { /* no source */ }

	inline	game_world::bullet_tracer*			address		( game_world::bullet_tracer& arg_0 ) const { /* no source */ }
	inline	game_world::bullet_tracer const*	address		( game_world::bullet_tracer const& arg_0 ) const { /* no source */ }

	inline	game_world::bullet_tracer*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	game_world::bullet_tracer*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	game_world::bullet_tracer*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	game_world::bullet_tracer*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*								__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void								deallocate	( game_world::bullet_tracer* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void								deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void								construct	( game_world::bullet_tracer* arg_0, game_world::bullet_tracer const& arg_1 ) { /* no source */ }

	inline	void								destroy		( game_world::bullet_tracer* arg_0 ) { /* no source */ }

	inline	u32									max_size	( ) const { /* no source */ }
}; // class std_allocator< game_world::bullet_tracer >

STATIC_SIZE_ASSERT(std_allocator< game_world::bullet_tracer >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED

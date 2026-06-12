////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::game_world::bullet_tracer,survarium::std_allocator<survarium::game_world::bullet_tracer> >
	stlp_std::vector<survarium::game_world::bullet_tracer >;
class survarium::game_world::bullet_tracer;

namespace survarium {

class vector< game_world::bullet_tracer > : public std::vector< game_world::bullet_tracer > {
public:
	inline										vector<survarium::game_world::bullet_tracer>( ) { /* no source */ }
	inline										vector<survarium::game_world::bullet_tracer>( u32 arg_0, game_world::bullet_tracer const& arg_1 ) { /* no source */ }
	inline	explicit							vector<survarium::game_world::bullet_tracer>( u32 arg_0 ) { /* no source */ }

	inline	game_world::bullet_tracer&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	game_world::bullet_tracer const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32									size		( ) const { /* no source */ }

	inline										~vector<survarium::game_world::bullet_tracer>( ) { /* no source */ }
}; // class vector< game_world::bullet_tracer >

STATIC_SIZE_ASSERT(vector< game_world::bullet_tracer >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<enum survarium::game_action_id,survarium::std_allocator<enum survarium::game_action_id> >
	stlp_std::vector<enum survarium::game_action_id >;
class enum survarium::game_action_id;

/* FORWARD REFS */
class survarium::game_action_id;

namespace survarium {

class vector< enum game_action_id > : public std::vector< enum game_action_id > {
public:
	inline								vector<enum survarium::game_action_id>( ) { /* no source */ }
	inline								vector<enum survarium::game_action_id>( u32 arg_0, game_action_id const& arg_1 ) { /* no source */ }
	inline	explicit					vector<enum survarium::game_action_id>( u32 arg_0 ) { /* no source */ }

	inline	game_action_id&				operator[]	( u32 arg_0 ) { /* no source */ }
	inline	game_action_id const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<enum survarium::game_action_id>( ) { /* no source */ }
}; // class vector< enum game_action_id >

STATIC_SIZE_ASSERT(vector< enum game_action_id >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED

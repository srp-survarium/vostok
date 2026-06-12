////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::player_actions_subscriber *,survarium::std_allocator<survarium::player_actions_subscriber *> >
	stlp_std::vector<survarium::player_actions_subscriber * >;
class survarium::player_actions_subscriber *;

/* FORWARD REFS */
class survarium::player_actions_subscriber;
class survarium::player_actions_subscriber*;

namespace survarium {

class vector< player_actions_subscriber* > : public std::vector< player_actions_subscriber* > {
public:
	inline											vector<survarium::player_actions_subscriber *>( ) { /* no source */ }
	inline											vector<survarium::player_actions_subscriber *>( u32 arg_0, player_actions_subscriber* const& arg_1 ) { /* no source */ }
	inline	explicit								vector<survarium::player_actions_subscriber *>( u32 arg_0 ) { /* no source */ }

	inline	player_actions_subscriber*&				operator[]	( u32 arg_0 ) { /* no source */ }
	inline	player_actions_subscriber* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32										size		( ) const { /* no source */ }

	inline											~vector<survarium::player_actions_subscriber *>( ) { /* no source */ }
}; // class vector< player_actions_subscriber* >

STATIC_SIZE_ASSERT(vector< player_actions_subscriber* >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED

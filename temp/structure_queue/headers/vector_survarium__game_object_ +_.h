////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::game_object_ *,survarium::std_allocator<survarium::game_object_ *> >
	stlp_std::vector<survarium::game_object_ * >;
class survarium::game_object_ *;

/* FORWARD REFS */
class survarium::game_object_;
class survarium::game_object_*;

namespace survarium {

class vector< game_object_* > : public std::vector< game_object_* > {
public:
	inline							vector<survarium::game_object_ *>	( ) { /* no source */ }
	inline							vector<survarium::game_object_ *>	( u32 arg_0, game_object_* const& arg_1 ) { /* no source */ }
	inline	explicit				vector<survarium::game_object_ *>	( u32 arg_0 ) { /* no source */ }

	inline	game_object_*&			operator[]							( u32 arg_0 ) { /* no source */ }
	inline	game_object_* const&	operator[]							( u32 arg_0 ) const { /* no source */ }

	inline	u32						size								( ) const { /* no source */ }

	inline							~vector<survarium::game_object_ *>	( ) { /* no source */ }
}; // class vector< game_object_* >

STATIC_SIZE_ASSERT(vector< game_object_* >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED

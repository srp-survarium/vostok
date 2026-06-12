////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::victory_items_container *,survarium::std_allocator<survarium::victory_items_container *> >
	stlp_std::vector<survarium::victory_items_container * >;
class survarium::victory_items_container *;

/* FORWARD REFS */
class survarium::victory_items_container;
class survarium::victory_items_container*;

namespace survarium {

class vector< victory_items_container* > : public std::vector< victory_items_container* > {
public:
	inline										vector<survarium::victory_items_container *>( ) { /* no source */ }
	inline										vector<survarium::victory_items_container *>( u32 arg_0, victory_items_container* const& arg_1 ) { /* no source */ }
	inline	explicit							vector<survarium::victory_items_container *>( u32 arg_0 ) { /* no source */ }

	inline	victory_items_container*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	victory_items_container* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32									size		( ) const { /* no source */ }

	inline										~vector<survarium::victory_items_container *>( ) { /* no source */ }
}; // class vector< victory_items_container* >

STATIC_SIZE_ASSERT(vector< victory_items_container* >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::collision_geometry *,survarium::std_allocator<survarium::collision_geometry *> >
	stlp_std::vector<survarium::collision_geometry * >;
class survarium::collision_geometry *;

/* FORWARD REFS */
class survarium::collision_geometry;
class survarium::collision_geometry*;

namespace survarium {

class vector< collision_geometry* > : public std::vector< collision_geometry* > {
public:
	inline									vector<survarium::collision_geometry *>( ) { /* no source */ }
	inline									vector<survarium::collision_geometry *>( u32 arg_0, collision_geometry* const& arg_1 ) { /* no source */ }
	inline	explicit						vector<survarium::collision_geometry *>( u32 arg_0 ) { /* no source */ }

	inline	collision_geometry*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	collision_geometry* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32								size		( ) const { /* no source */ }

	inline									~vector<survarium::collision_geometry *>( ) { /* no source */ }
}; // class vector< collision_geometry* >

STATIC_SIZE_ASSERT(vector< collision_geometry* >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED

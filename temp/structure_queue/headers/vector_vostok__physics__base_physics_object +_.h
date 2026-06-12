////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::physics::base_physics_object *,survarium::std_allocator<vostok::physics::base_physics_object *> >
	stlp_std::vector<vostok::physics::base_physics_object * >;
class vostok::physics::base_physics_object *;

/* FORWARD REFS */
class vostok::physics::base_physics_object;
class vostok::physics::base_physics_object*;

namespace survarium {

class vector< physics::base_physics_object* > : public std::vector< physics::base_physics_object* > {
public:
	inline										vector<vostok::physics::base_physics_object *>( ) { /* no source */ }
	inline										vector<vostok::physics::base_physics_object *>( u32 arg_0, physics::base_physics_object* const& arg_1 ) { /* no source */ }
	inline	explicit							vector<vostok::physics::base_physics_object *>( u32 arg_0 ) { /* no source */ }

	inline	physics::base_physics_object*&		operator[]	( u32 arg_0 ) { /* no source */ }
	inline	physics::base_physics_object* const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32									size		( ) const { /* no source */ }

	inline										~vector<vostok::physics::base_physics_object *>( ) { /* no source */ }
}; // class vector< physics::base_physics_object* >

STATIC_SIZE_ASSERT(vector< physics::base_physics_object* >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED

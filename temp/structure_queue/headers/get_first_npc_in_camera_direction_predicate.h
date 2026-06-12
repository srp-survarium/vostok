////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GET_FIRST_NPC_IN_CAMERA_DIRECTION_PREDICATE_H_INCLUDED
#define GET_FIRST_NPC_IN_CAMERA_DIRECTION_PREDICATE_H_INCLUDED

/* INCLUDES */
class survarium::human_npc;

/* FORWARD REFS */
class vostok::physics::closest_ray_result;

namespace survarium {

struct get_first_npc_in_camera_direction_predicate : public boost::noncopyable {
	inline			get_first_npc_in_camera_direction_predicate( ) { /* no source */ }

	inline	bool	operator()	( physics::closest_ray_result const& arg_0 ) { /* no source */ }

	inline			~get_first_npc_in_camera_direction_predicate( ) { /* no source */ }

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	human_npc*		first_npc;
}; // struct get_first_npc_in_camera_direction_predicate

STATIC_SIZE_ASSERT(get_first_npc_in_camera_direction_predicate, 0x4);

} // namespace survarium

#endif // #ifndef GET_FIRST_NPC_IN_CAMERA_DIRECTION_PREDICATE_H_INCLUDED

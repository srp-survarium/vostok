////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\object_solid_visual.h"

namespace survarium {

// STATE[STUB]
explicit object_particle_visual::object_particle_visual( base_game_scene& w )
{
	// FUNCTION BODY[0x78e270]
	// <0x78e270>|0x000|      :'455'	{
	// ******
}

// STATE[STUB]
void object_particle_visual::load(
	configs::binary_config_value const&		t,
	pcstr									project_resources_path,
	boost::function< void( game_object_& ) >&	cb
)
{
	// LOCALS
	// variant< 32 > 					ud
	// ******

	// FUNCTION BODY[0x78e310]: 15
	// <0x78e31a>|0x00a|+0x015:'459'
	// <0>
	// <1>
	// <0x78e32f>|0x01f|+0x04b:'462'
	// <0x78e37a>|0x06a|+0x029:'463'
	// <0>
	// <0x78e3a3>|0x093|+0x01e:'465'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x78e3c1>|0x0b1|+0x098:'473'
	// ******
}

// STATE[STUB]
void object_particle_visual::on_visual_ready( resources::queries_result& data, boost::function< void( game_object_& ) >& cb )
{
	// FUNCTION BODY[0x78e1b0]: 4
	// <0>
	// <1>
	// <0x78e1b1>|0x001|+0x0ab:'480'
	// <0x78e25c>|0x0ac|+0x00a:'481'
	// ******
}

// STATE[STUB]
void object_particle_visual::insert( )
{
	// FUNCTION BODY[0x78e2d0]: 1
	// <0x78e2d3>|0x003|+0x03a:'486'
	// ******
}

// STATE[STUB]
void object_particle_visual::remove( )
{
	// FUNCTION BODY[0x78e2a0]: 1
	// <0x78e2a0>|0x000|+0x026:'491'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	survarium::base_project::resolve_link_object*
	// 	iterator_type;

	// typedef
	// 	survarium::scheduler::record*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data const*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data*
	// 	iterator_type;

	// ******

} // namespace survarium

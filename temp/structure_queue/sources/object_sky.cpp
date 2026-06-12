////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\object_sky.h"

namespace survarium {

// STATE[STUB]
explicit object_sky::object_sky( base_game_scene& w )
{
	// FUNCTION BODY[0x78f860]
	// <0x78f860>|0x000|      :'22'	{
	// ******
}

// STATE[STUB]
 object_sky::~object_sky( )
{
	// FUNCTION BODY[0x78f820]
	// <0x78f820>|0x000|      :'25'	{
	// ******
}

// STATE[STUB]
void object_sky::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	// LOCALS
	// variant< 32 > 					user_data
	// pcstr 							sky_material_name
	// ******

	// FUNCTION BODY[0x78f8f0]: 21
	// <0x78f900>|0x010|+0x00f:'29'
	// <0>
	// <1>
	// <0x78f90f>|0x01f|+0x02f:'32'
	// <0>
	// <0x78f93e>|0x04e|+0x008:'34'
	// <0x78f946>|0x056|+0x005:'35'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <0x78f94b>|0x05b|+0x0c5:'49'
	// ******
}

// STATE[STUB]
void object_sky::material_ready(
	resources::queries_result&		data,
	render::material_effects_instance_cook_data*	cook_data,
	boost::function< void( game_object_& ) >&	cb
)
{
	// FUNCTION BODY[0x78f770]: 4
	// <0x78f773>|0x003|+0x012:'54'
	// <0>
	// <0x78f785>|0x015|+0x07e:'56'
	// <0x78f803>|0x093|+0x00a:'57'
	// ******
}

// STATE[STUB]
void object_sky::insert( )
{
	// FUNCTION BODY[0x78f8c0]: 1
	// <0x78f8c0>|0x000|+0x026:'62'
	// ******
}

// STATE[STUB]
void object_sky::remove( )
{
	// FUNCTION BODY[0x78f890]: 1
	// <0x78f891>|0x001|+0x02b:'67'
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

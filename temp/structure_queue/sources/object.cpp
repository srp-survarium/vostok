////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\object.h"

namespace survarium {

// STATE[STUB]
explicit game_object_::game_object_( base_game_scene& s )
{
	// FUNCTION BODY[0x777c00]
	// <0x777c00>|0x000|      :'15'	{
	// ******
}

// STATE[STUB]
explicit game_object_static::game_object_static( base_game_scene& s )
{
	// FUNCTION BODY[0x777c20]
	// <0x777c20>|0x000|      :'19'	{
	// ******
}

// STATE[STUB]
void game_object_static::load(
	configs::binary_config_value const&		t,
	pcstr									project_resources_path,
	boost::function< void( game_object_& ) >&	cb
)
{
	// FUNCTION BODY[0x777be0]: 2
	// <0>
	// <0x777be0>|0x000|+0x011:'24'
	// ******
}

// STATE[STUB]
void load_transform( configs::binary_config_value const& t, float4x4& dest )
{
	// FUNCTION BODY[0x777b10]: 4
	// <0x777b21>|0x011|+0x00e:'29'
	// <0x777b2f>|0x01f|+0x00e:'30'
	// <0x777b3d>|0x02d|+0x00c:'31'
	// <0x777b49>|0x039|+0x08d:'32'
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

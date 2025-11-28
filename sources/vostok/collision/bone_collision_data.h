////////////////////////////////////////////////////////////////////////////
//	Created		: 14.04.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_COLLISION_BONE_COLLISION_DATA_H_INCLUDED
#define VOSTOK_COLLISION_BONE_COLLISION_DATA_H_INCLUDED

#include <vostok/animation/type_definitions.h>

namespace vostok {
namespace collision {

class geometry_instance;
class composite_geometry_instance;

class bone_collision_data
{
public:
	inline	bone_collision_data	( pcstr name, geometry_instance* instance, pcstr part_name ) :
		bone_name				( name ),
		body_part_name			( part_name ),
		bone_geometry_instance	( instance ),
		skeleton_bone_index		( u32( -1 ) )
	{
	}

private:
	friend struct memory::detail::call_constructor_helper< bone_collision_data, false >;

	inline	bone_collision_data	( )
	{
		UNREACHABLE_CODE( );
	}

public:
	fixed_string<64>				bone_name;
	fixed_string<16>				body_part_name;
	animation::bone_index_type		skeleton_bone_index;
	geometry_instance*				bone_geometry_instance;
};

STATIC_SIZE_ASSERT(bone_collision_data, 0x70);

} // namespace collision
} // namespace vostok

#endif // #ifndef VOSTOK_COLLISION_BONE_COLLISION_DATA_H_INCLUDED

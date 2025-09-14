////////////////////////////////////////////////////////////////////////////
//	Created		: 28.01.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_EDITOR_BASE_COLLISION_OBJECT_TYPES_H_INCLUDED
#define VOSTOK_EDITOR_BASE_COLLISION_OBJECT_TYPES_H_INCLUDED

namespace vostok {
namespace editor_base {

enum collision_object_types
{
	collision_object_type_dynamic	= ( 1 << 0 ),
	collision_object_type_control	= ( 1 << 1 ),
	collision_object_type_touch		= ( 1 << 2 ),
	collision_object_type_terrain	= ( 1 << 3 ),
}; // enum collision_object_types

} // namespace editor_base
} // namespace vostok

#endif // #ifndef VOSTOK_EDITOR_BASE_COLLISION_OBJECT_TYPES_H_INCLUDED
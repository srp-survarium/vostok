////////////////////////////////////////////////////////////////////////////
//	Created		: 29.03.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RESOURCES_TYPES_H_INCLUDED
#define VOSTOK_RESOURCES_TYPES_H_INCLUDED

namespace vostok {
namespace resources {

enum	
{
	normal_priority			=	0,
	high_priority			=	3,
	low_priority			=	-3
};

typedef u32					query_flag_enum;
enum
{
	query_flag_recursive	=	1,
};

enum autoselect_quality_bool
{
	autoselect_quality_false	=	0,
	autoselect_quality_true		=	1,
};

} // namespace resources
} // namespace vostok

#endif // #ifndef VOSTOK_RESOURCES_TYPES_H_INCLUDED
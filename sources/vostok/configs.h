////////////////////////////////////////////////////////////////////////////
//	Created		: 09.02.2009
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_CONFIGS_H_INCLUDED
#define VOSTOK_CONFIGS_H_INCLUDED

namespace vostok {
namespace configs {

enum enum_types {
	t_boolean,
	t_integer,
	t_float,
	t_table_named,
	t_table_indexed,
	t_embedded_types		= t_table_indexed,
	t_string,
	t_float2,
	t_float3,
	t_float4,
	t_int2,
	t_max					= u16(-1)
}; // enum enum_types

} // namespace configs
} // namespace vostok

#include <vostok/configs_binary_config.h>

#ifndef MASTER_GOLD
	#include <vostok/configs_lua_config.h>
#endif

#endif // #ifndef VOSTOK_CONFIGS_H_INCLUDED
////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\texture_options.h"

namespace vostok {
namespace render {

// STATE[STUB]
 texture_options_cooker::texture_options_cooker(
	resources::class_id_enum			resource_class,
	resources::cook_base::reuse_enum	reuse_type,
	u32									translate_query_thread,
	enum_flags< enum resources::cook_base::flags_enum >	flags
)
{
	// FUNCTION BODY[0x5591b0]
	// ******
}

// STATE[STUB]
void texture_options_cooker::delete_resource( resources::resource_base* __formal )
{
	// FUNCTION BODY[0x5591a0]: 0
	// ******
}

// STATE[STUB]
 texture_options_binary_cooker::texture_options_binary_cooker( )
{
	// FUNCTION BODY[0x5591d0]: 0
	// ******
}

// STATE[STUB]
void texture_options_binary_cooker::translate_query( resources::query_result_for_cook& parent )
{
	// FUNCTION BODY[0x5593c0]: 9
	// <0x5593cc>|0x00c|+0x013:'133'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5593df>|0x01f|+0x06f:'141'
	// ******
}

// STATE[STUB]
void texture_options_binary_cooker::on_binary_config_loaded( resources::queries_result& result )
{
	// LOCALS
	// resources::unmanaged_resource_ptr resource
	// fs_new::virtual_path_string 		path
	// ******

	// FUNCTION BODY[0x559240]: 23
	// <0x55924c>|0x00c|+0x011:'146'
	// <0>
	// <0x55925d>|0x01d|+0x011:'148'
	// <0>
	// <1>
	// <0x55926e>|0x02e|+0x013:'151'
	// <0>
	// <0x559281>|0x041|+0x022:'153'
	// <0x5592a3>|0x063|+0x00d:'154'
	// <0x5592b0>|0x070|+0x02c:'155'
	// <0>
	// <0x5592dc>|0x09c|+0x008:'157'
	// <0x5592e4>|0x0a4|+0x066:'158'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x55934a>|0x10a|-0x077:'166'
	// <0>
	// <1>
	// <0x5592d3>|0x093|+0x0e4:'169'
	// ******
}

// STATE[STUB]
void texture_options_binary_cooker::on_lua_options_loaded( resources::queries_result& result )
{
	// FUNCTION BODY[0x559210]: 4
	// <0>
	// <1>
	// <2>
	// <0x559211>|0x001|+0x01b:'176'
	// ******
}

} // namespace render
} // namespace vostok

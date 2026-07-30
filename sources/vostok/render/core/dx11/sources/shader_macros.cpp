#include "pch.h"
#include <vostok/render/core/shader_macros.h>

namespace vostok {
namespace render {

namespace {

void add_bool_macro(
	shader_defines_list&,
	pcstr,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x1297a0]
}

void add_u8_macro(
	shader_defines_list&,
	pcstr,
	u8
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x129880]
}

} // namespace

shader_macros::shader_macros( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x73a910]
}

void shader_macros::register_available_macros( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x73a520]
}

void shader_macros::fill_shader_macro_list(
	shader_defines_list&,
	shader_configuration
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x73af60]
}

void shader_macros::fill_shader_configuration_macros(
	shader_defines_list&,
	shader_configuration
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x73a960]
}

void shader_macros::fill_global_macros( shader_defines_list& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x73a950]
}

} // namespace render
} // namespace vostok

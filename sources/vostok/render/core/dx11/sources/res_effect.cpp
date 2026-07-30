#include "pch.h"
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/res_state.h>
#include <vostok/render/core/res_xs.h>
#include <vostok/render/core/dx11/res_input_layout.h>

namespace vostok {
namespace render {

res_pass::~res_pass( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e7e50]
}

void res_pass::destroy_impl( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e7ee0]
}

s32 compare( res_pass const&, res_pass const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e7b10]
	return 0;
}

void res_pass::apply( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e7e20]
}

res_shader_technique::~res_shader_technique( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e7ef0]
}

void res_shader_technique::destroy_impl( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e7f30]
}

s32 compare( res_shader_technique const&, res_shader_technique const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e7b80]
	return 0;
}

res_effect::~res_effect( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e8010]
}

bool res_effect::apply_pass( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e7f70]
	return false;
}

void res_effect::push_texture_unique( res_texture*, pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6e7ce0]
}

} // namespace render
} // namespace vostok

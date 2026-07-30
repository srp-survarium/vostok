#include "pch.h"
#include <vostok/render/core/shader_constant_table.h>

namespace vostok {
namespace render {

namespace res_const_table_predicates {

bool sort( shader_constant const&, shader_constant const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12ea70]
	return false;
}

} // namespace res_const_table_predicates

void shader_constant_table::destroy_impl( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x739360]
}

shader_constant* shader_constant_table::get( shared_string const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7390d0]
	return 0;
}

shader_constant* shader_constant_table::get( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7392e0]
	return 0;
}

bool shader_constant_table::parse_constant_buffer(
	ID3D11ShaderReflectionConstantBuffer*,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x739370]
	return false;
}

bool shader_constant_table::parse(
	ID3D11ShaderReflection*,
	enum_shader_type
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x739600]
	return false;
}

void shader_constant_table::clear( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7392a0]
}

s32 shader_constant_table::compare( shader_constant_table const& ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x739190]
	return 0;
}

void shader_constant_table::apply_bindings( shader_constant_bindings const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x739100]
}

} // namespace render
} // namespace vostok

#include "pch.h"
#include "material.h"

namespace vostok {
namespace render {

// STATE[STUB]
bool material::is_nomaterial_material_ready( )
{
	// FUNCTION BODY[0x70cb60]
	return false;
}

// STATE[STUB]
material_effects& material::nomaterial_material( enum_vertex_input_type )
{
	// FUNCTION BODY[0x70cb50]
	return *static_cast< material_effects* >( 0 );
}

// STATE[STUB]
void material::finalize_nomaterial_material( )
{
	// FUNCTION BODY[0x70cb90]
}

} // namespace render
} // namespace vostok

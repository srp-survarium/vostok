#include "pch.h"
#include "material_effects.h"

namespace vostok {
namespace render {

material_effects::material_effects( ) :
	m_vertex_input_type	( null_vertex_input_type ),
	m_cull_mode			( D3D11_CULL_NONE )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62ff80]
}

enum_vertex_input_type material_effects::get_vertex_input_type( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x62feb0]
	return m_vertex_input_type;
}

void material_effects::get_used_textures(
	vector< texture_named_instance >&
) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x62fec0]
}

} // namespace render
} // namespace vostok

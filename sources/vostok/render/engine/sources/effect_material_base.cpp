#include "pch.h"
#include "blend_mode.h"
#include "effect_material_base.h"
#include <vostok/render/core/shader_include_getter.h>
#include <vostok/render/facade/vertex_input_type.h>

namespace vostok {
namespace render {

class material_shader_include_getter : public shader_include_getter {
public:
	explicit material_shader_include_getter( u32 vertex_type ) :
		m_vertex_type( vertex_type )
	{
	}

	virtual pcstr get_shader_include( pcstr include_file_name ) const override
	{
		if ( strings::compare( include_file_name, "vertex_input.h" ) != 0 )
			return include_file_name;

		switch ( m_vertex_type ) {
		case null_vertex_input_type:
			return "null_mesh_vertex_input.h";
		case static_mesh_vertex_input_type:
		case static_mesh_vertex_colored_input_type:
			return "static_mesh_vertex_input.h";
		case skeletal_4_bones_mesh_vertex_input_type:
		case skeletal_3_bones_mesh_vertex_input_type:
		case skeletal_2_bones_mesh_vertex_input_type:
		case skeletal_1_bones_mesh_vertex_input_type:
			return "skeletal_mesh_vertex_input.h";
		case particle_vertex_input_type:
			return "particle_vertex_input.h";
		case particle_subuv_vertex_input_type:
			return "particle_subuv_vertex_input.h";
		case particle_beamtrail_vertex_input_type:
			return "particle_beamtrail_vertex_input.h";
		case decal_vertex_input_type:
			return "decal_vertex_input.h";
		case grassmesh_vertex_input_type:
			return "grass_vertex_input.h";
		default:
			return include_file_name;
		}
	}

private:
	u32 m_vertex_type;
};

void effect_material_base::compile_begin(
	pcstr,
	pcstr,
	effect_compiler&,
	shader_configuration*,
	custom_config_value const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x7b3d40]
}

void effect_material_base::compile_begin(
	pcstr,
	pcstr,
	pcstr,
	effect_compiler&,
	shader_configuration*,
	custom_config_value const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x7b3bb0]
}

void effect_material_base::compile_end( effect_compiler& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7b3d60]
}

} // namespace render
} // namespace vostok

#include "pch.h"
#include "material.h"

#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include <vostok/render/facade/render_stage_types.h>

#include "depth_accumulate_material_effect.h"
#include "effect_gbuffer_nomaterial_materials.h"
#include "material_effects.h"

namespace vostok {
namespace render {

static material_effects* s_nomaterial_material_effects[num_vertex_input_types];

void material::initialize_nomaterial_material( )
{
	// FUNCTION BODY[0x6fcbf0]
	// claude@NOTE: legacy geometry_render_stage adapted to the canonical gbuffer_render_stage
	// (same adaptation as is_nomaterial_material_ready below).
	for (u32 i=0; i<num_vertex_input_types; i++)
	{
		s_nomaterial_material_effects[i]									  = NEW(material_effects);
		s_nomaterial_material_effects[i]->m_vertex_input_type				  = (enum_vertex_input_type)i;

		byte data[Kb];
		vostok::render::effect_options_descriptor								  additional_parameters(data, sizeof(data) );
		additional_parameters["vertex_input_type"]							  = (enum_vertex_input_type)i;
		additional_parameters["cull_mode"]									  = D3D_CULL_NONE;

		effect_manager::ref().create_effect<effect_gbuffer_nomaterial_materials>(
			&s_nomaterial_material_effects[i]->m_effects[gbuffer_render_stage],
			additional_parameters
		);
		s_nomaterial_material_effects[i]->stage_enable[gbuffer_render_stage] = true;

		effect_manager::ref().create_effect<render::depth_accumulate_material_effect>(
			&s_nomaterial_material_effects[i]->m_effects[shadow_render_stage],
			additional_parameters
		);
		s_nomaterial_material_effects[i]->stage_enable[shadow_render_stage] = true;
	}
}

bool material::is_nomaterial_material_ready( )
{
	// FUNCTION BODY[0x70cb60]
	// claude@NOTE: legacy geometry_render_stage adapted to the canonical gbuffer_render_stage.
	for (u32 i=0; i<num_vertex_input_types; i++)
	{
		if (!s_nomaterial_material_effects[i]->m_effects[gbuffer_render_stage].c_ptr())
			return false;
	}
	return true;
}

material_effects& material::nomaterial_material( enum_vertex_input_type vertex_input_type )
{
	// FUNCTION BODY[0x70cb50]
//	if (!s_nomaterial_material_effects[vertex_input_type])
//		material::initialize_nomaterial_material();

	ASSERT_CMP(s_nomaterial_material_effects[vertex_input_type], !=, 0);
	return *s_nomaterial_material_effects[vertex_input_type];
}

void material::finalize_nomaterial_material( )
{
	// FUNCTION BODY[0x70cb90]
	for (u32 i=0; i<num_vertex_input_types; i++)
	{
		DELETE(s_nomaterial_material_effects[i]);
		s_nomaterial_material_effects[i] = 0;
	}
}

} // namespace render
} // namespace vostok

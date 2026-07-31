////////////////////////////////////////////////////////////////////////////
//	Created		: 16.09.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

// HARVEST REMAINDER: is_nomaterial_material_ready / nomaterial_material /
// finalize_nomaterial_material ported to engine/sources/material.cpp; the
// material ctor ported to engine/sources/material.h (legacy material.h deleted).
// Remaining: initialize_nomaterial_material - the canonical header keeps it as
// an empty inline, so this real body stays here as a reading reference.

#include "pch.h"
#include "material.h"
#include "material_effects.h"
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/custom_config_value.h>
#include <vostok/render/core/custom_config.h>
#include <vostok/render/core/res_texture.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/facade/vertex_input_type.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include "effect_gbuffer_nomaterial_materials.h"
#include "depth_accumulate_material_effect.h"
#include <vostok/render/core/destroy_data_helper.h>

namespace vostok {
namespace render {

static material_effects* s_nomaterial_material_effects[num_vertex_input_types];

void material::initialize_nomaterial_material()
{
	for (u32 i=0; i<num_vertex_input_types; i++)
	{
		s_nomaterial_material_effects[i]									  = NEW(material_effects);
		s_nomaterial_material_effects[i]->m_vertex_input_type				  = (enum_vertex_input_type)i;

		byte data[Kb];
		vostok::render::effect_options_descriptor								  additional_parameters(data, sizeof(data) );
		additional_parameters["vertex_input_type"]							  = (enum_vertex_input_type)i;
		additional_parameters["cull_mode"]									  = D3D_CULL_NONE;

		effect_manager::ref().create_effect<effect_gbuffer_nomaterial_materials>(
			&s_nomaterial_material_effects[i]->m_effects[geometry_render_stage],
			additional_parameters
		);
		s_nomaterial_material_effects[i]->stage_enable[geometry_render_stage] = true;

		effect_manager::ref().create_effect<render::depth_accumulate_material_effect>(
			&s_nomaterial_material_effects[i]->m_effects[shadow_render_stage],
			additional_parameters
		);
		s_nomaterial_material_effects[i]->stage_enable[shadow_render_stage] = true;
	}
}

} // namespace render
} // namespace vostok



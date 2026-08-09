////////////////////////////////////////////////////////////////////////////
//	Created		: 14.09.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "material_effects.h"
#include <vostok/render/core/custom_config.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/res_texture.h>
#include <vostok/render/facade/vertex_input_type.h>
#include "material.h"

namespace vostok {
namespace render {

material_effects::material_effects()
{


	m_cull_mode				= D3D11_CULL_NONE;
	m_vertex_input_type		= null_vertex_input_type;
	vostok::memory::zero		(stage_enable);
	organic_clear_color		= math::float4(1.0f, 1.0f, 1.0f, 1.0f);
	is_cast_shadow			= true;
	is_organic				= false;
	organic_clear_color		= vostok::math::float4(1.0f, 1.0f, 1.0f, 1.0f);
	is_use_bound_normals		= false;
	is_wind_swings			= false;
	has_translucency			= false;
	is_background_sky		= false;
	is_use_alpha_test		= false;
	is_emissive				= false;
	has_local_reflections	= false;
	has_transparency		= false;
	is_forward_after_fog	= false;
	use_subsurface_scattering = false;
	blend_mode				= 0;
}

u32 material_effects::get_render_complexity() const
{
	u32 result = 0;
	for (u32 effect_index = 0; effect_index < num_render_stages; effect_index++)
	{
		if (m_effects[effect_index])
		{
			result += m_effects[effect_index]->get_max_pixel_shader_instruction_count();
		}
	}
	return result;
}

void material_effects::get_max_used_texture_dimension(u32& out_size_x, u32& out_size_y) const
{
	out_size_x = out_size_y = 1;

	for (u32 effect_index = 0; effect_index < num_render_stages; effect_index++)
	{
		if (m_effects[effect_index])
		{
			u32 current_size_x, current_size_y;
			m_effects[effect_index]->get_max_used_texture_dimension(current_size_x, current_size_y);
			out_size_x = vostok::math::max(current_size_x, out_size_x);
			out_size_y = vostok::math::max(current_size_y, out_size_y);
		}
	}
}

enum_vertex_input_type material_effects::get_vertex_input_type() const
{
	return m_vertex_input_type;
}

void material_effects::get_used_textures(vector< texture_named_instance >& out_array) const
{
	for (u32 i = 0; i < num_render_stages; ++i)
	{
		if (!m_effects[i])
		{
			continue;
		}

		texture_named_instance const* tex_it = m_effects[i]->m_used_textures.begin();
		texture_named_instance const* tex_end = m_effects[i]->m_used_textures.end();

		for (; tex_it != tex_end; ++tex_it)
			out_array.push_back(*tex_it);
	}
}

} // namespace render
} // namespace vostok

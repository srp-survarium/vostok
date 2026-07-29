#ifndef VOSTOK_RENDER_ENGINE_MATERIAL_EFFECTS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_MATERIAL_EFFECTS_H_INCLUDED

#include <d3d11.h>

#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/facade/vertex_input_type.h>
#include <vostok/resources_unmanaged_resource.h>

#include "post_process_parameters.h"

namespace vostok {
namespace render {

struct texture_named_instance;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

struct material_effects {
	enum { stage_count = 29 };

	material_effects( );

	u32 get_render_complexity( ) const
	{
		return 0;
	}

	void get_max_used_texture_dimension(
		u32& size_x,
		u32& size_y
	) const
	{
		size_x = 1;
		size_y = 1;
	}

	enum_vertex_input_type get_vertex_input_type( ) const;
	void get_used_textures( vector< texture_named_instance >& out_array ) const;

	post_process_parameters	m_post_process_stage_parameters;
	bool					stage_enable[stage_count];
	bool					is_emissive;
	bool					is_organic;
	bool					use_subsurface_scattering;
	bool					is_cast_shadow;
	bool					is_use_bound_normals;
	bool					is_use_alpha_test;
	bool					is_wind_swings;
	bool					is_background_sky;
	bool					has_local_reflections;
	bool					has_translucency;
	bool					has_transparency;
	bool					is_forward_after_fog;
	u32						blend_mode;
	float4					organic_clear_color;
	enum_vertex_input_type	m_vertex_input_type;
	D3D11_CULL_MODE			m_cull_mode;
	res_effect_ptr			m_effects[stage_count];
};

STATIC_SIZE_ASSERT( material_effects, 0x390 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_MATERIAL_EFFECTS_H_INCLUDED

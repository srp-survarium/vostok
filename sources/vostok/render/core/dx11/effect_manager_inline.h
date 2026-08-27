#ifndef VOSTOK_RENDER_CORE_DX11_EFFECT_MANAGER_INLINE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_EFFECT_MANAGER_INLINE_H_INCLUDED

#include <vostok/render/core/effect_loader.h>
#include <vostok/resources.h>

namespace vostok {
namespace render {

inline void effect_manager::create_new_effect(
	res_effect_ptr* out_effect,
	effect_descriptor* descriptor,
	custom_config_ptr const& config,
	u32 crc,
	creation_callback_type callback
)
{
	resources::user_data_variant user_data_variant;

	effect_compile_data* cook_data = NEW( effect_compile_data )(
		descriptor,
		config,
		crc
	);
	user_data_variant.set( cook_data );

	resources::query_create_resource(
		"",
		vostok::const_buffer( "", 1 ),
		resources::render_effect_class,
		boost::bind(
			&effect_manager::on_async_effect_created_callback,
			this,
			_1,
			out_effect,
			descriptor,
			callback
		),
		g_allocator,
		&user_data_variant
	);
}

inline void effect_manager::create_new_effect(
	effect_loader* loader,
	effect_descriptor* descriptor,
	custom_config_ptr const& config,
	u32 crc
)
{
	resources::user_data_variant user_data_variant;

	effect_compile_data* cook_data = NEW( effect_compile_data )(
		descriptor,
		config,
		crc
	);
	user_data_variant.set( cook_data );

	resources::query_create_resource(
		"",
		vostok::const_buffer( "", 1 ),
		resources::render_effect_class,
		boost::bind( &effect_loader::on_effect_ready, loader, _1 ),
		g_allocator,
		&user_data_variant
	);
}

template < typename effect_descriptor_type >
inline res_effect_ptr effect_manager::create_effect( )
{
	res_effect_ptr result;
	create_effect<effect_descriptor_type>( &result );
	return result;
}

template < typename effect_descriptor_type >
inline res_effect_ptr effect_manager::create_effect(
	effect_options_descriptor const& options
)
{
	res_effect_ptr result;
	create_effect<effect_descriptor_type>( &result, options );
	return result;
}

template < typename effect_descriptor_type >
inline void effect_manager::create_effect( res_effect_ptr* out_effect )
{
	// Observed instantiations:
	// effect_downsample_reflective_shadow_map [0x7d4b0]
	// effect_translucency [0x7e3d0]
	// effect_resolve_lighting [0x7e4e0]
	// effect_clouds [0x7e610]
	// effect_atmospheric_scattering [0x7eb50]
	// effect_apply_distortion [0x7f180]
	// effect_rain [0x7f2b0]
	// effect_exponential_volume_fog [0x7f550]
	// effect_gather_bloom [0x7fe00]
	// effect_debug_environment_probe_preview [0x855d0]
	// effect_editor_apply_wireframe [0x86090]
	// scr_quad_effect [0x864c0]
	// effect_hiz_occlusion [0x8cb80]
	// effect_pick_light_luminance [0x651d0]
	// effect_system_colored [0x66a30]
	// effect_resolve_particles [0x68a60]
	// effect_light_propagation_volumes [0x8c9c0]
	// effect_copy_depth_rt [0x726c0]
	// effect_decal_mask [0x73270]
	// effect_wet_surface [0x73950]
	// effect_ssao_accumulation [0x73d70]
	// effect_light_mask [0x74470]
	// effect_sun [0x788b0]
	// point_light_effect<0, 0> [0x78d10]
	static effect_descriptor_type	descriptor_object;

	pcvoid					  mem	= ALLOCA(Kb);
	effect_options_descriptor empty_desc(mem, Kb);

	u32 crc							= 0;

	if (force_sync)
		*out_effect					= create_new_effect(
			descriptor_object,
			render::create_custom_config(empty_desc, crc, false),
			crc
		);
	else
		create_new_effect				(out_effect, &descriptor_object, render::create_custom_config(empty_desc, crc, false), crc);
}

template < typename effect_descriptor_type >
inline void effect_manager::create_effect(
	res_effect_ptr* out_effect,
	effect_options_descriptor const& desc
)
{
	// Observed instantiations:
	// effect_debug_tracer [0x7ed60]
	// effect_motion_vectors_accumulation [0x81ef0]
	// effect_editor_wireframe_accumulation [0x85910]
	// effect_system_ui [0x66c50]
	// effect_gbuffer_nomaterial_materials [0x68dc0]
	// effect_fill_reflective_shadow_map [0x727d0]
	static effect_descriptor_type	descriptor_object;
	u32 crc							= desc.get_crc();
	if (force_sync)
	{
		*out_effect					= create_new_effect(
			descriptor_object,
			render::create_custom_config(desc, crc, false),
			crc
		);
	}else
	{
		create_new_effect	( out_effect,
							&descriptor_object,
							render::create_custom_config(desc, crc, false),
							crc );
	}
}

template < typename effect_descriptor_type >
inline void effect_manager::create_effect(
	effect_loader* loader,
	effect_options_descriptor const& desc
)
{
	static effect_descriptor_type	descriptor_object;
	u32 crc							= desc.get_crc();
	create_new_effect	( loader,
						&descriptor_object,
						render::create_custom_config(desc, crc, false),
						crc );
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_EFFECT_MANAGER_INLINE_H_INCLUDED

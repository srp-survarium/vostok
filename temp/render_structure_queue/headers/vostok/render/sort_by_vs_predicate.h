////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SORT_BY_VS_PREDICATE_H_INCLUDED
#define RENDER_SORT_BY_VS_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::enum_render_stage_type;

/* FORWARD REFS */
class vostok::render::render_surface_instance;

namespace vostok {
namespace render {

enum vostok::render::enum_render_stage_type
{
	gbuffer_render_stage					= 0x00,
	decals_accumulate_render_stage			= 0x01,
	accumulate_distortion_render_stage		= 0x02,
	pre_rain_normal_modify_render_stage		= 0x03,
	pre_lighting_render_stage				= 0x04,
	ambient_occlusion_render_stage			= 0x05,
	ambient_lighting_render_stage			= 0x06,
	sun_shadows_accumulate_render_stage		= 0x07,
	sun_render_stage						= 0x08,
	deferred_lighting_render_stage			= 0x09,
	light_propagation_volumes_render_stage	= 0x0a,
	translucency_render_stage				= 0x0b,
	resolve_lighting_render_stage			= 0x0c,
	skybox_render_stage						= 0x0d,
	skysphere_render_stage					= 0x0e,
	clouds_render_stage						= 0x0f,
	atmosphere_render_stage					= 0x10,
	forward_render_stage					= 0x11,
	atmosphere_on_geometry_render_stage		= 0x12,
	apply_distortion_render_stage			= 0x13,
	forward_sky_render_stage				= 0x14,
	rain_render_stage						= 0x15,
	lighting_render_stage					= 0x16,
	particles_render_stage					= 0x17,
	volume_fog_render_stage					= 0x18,
	post_process_render_stage				= 0x19,
	debug_post_process_render_stage			= 0x1a,
	debug_render_stage						= 0x1b,
	shadow_render_stage						= 0x1c,
	num_render_stages						= 0x1d,
	unknown_render_stage					= 0x1e,
};

struct sort_by_vs_predicate {
	inline			sort_by_vs_predicate( enum_render_stage_type arg_0, const u32 arg_1 ) { /* no source */ }

			bool	operator()			( render_surface_instance const* left, render_surface_instance const* right ) const;

	/* 0x0000 */	enum_render_stage_type		m_stage_type;
	/* 0x0004 */	u32							m_tech_index;
}; // struct sort_by_vs_predicate

STATIC_SIZE_ASSERT(sort_by_vs_predicate, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SORT_BY_VS_PREDICATE_H_INCLUDED

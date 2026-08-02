#ifndef VOSTOK_RENDER_ENGINE_STAGE_ATMOSPHERE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_ATMOSPHERE_H_INCLUDED

#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/untyped_buffer.h>

#include "render_target.h"
#include "res_geometry.h"
#include "sky_dome_geometry.h"
#include "sphere_geometry.h"
#include "stage.h"

namespace vostok {
namespace render {

class renderer;
class renderer_context;
class shader_constant_host;

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

class stage_atmosphere : public stage {
public:
	enum stage_type {
		atmosphere_on_sky		= 0,
		atmosphere_on_geometry	= 1,
	};

	stage_atmosphere(
		renderer*			in_renderer,
		renderer_context*	context,
		stage_type			type
	);

	virtual ~stage_atmosphere( );

	virtual void execute( ) override;

	bool is_effects_ready( ) const;

	void fill_surfaces(
		render_target_ptr	surf0,
		render_target_ptr	surf1,
		bool				use_base_depth_stencil
	);

private:
	sky_dome_geometry	m_sky_dome_geometry;
	sphere_geometry		m_clouds_geometry;
	res_effect_ptr		m_atmospheric_scattering_effect;
	untyped_buffer_ptr	m_screen_vertex_ib;
	res_geometry_ptr	m_screen_vertex_geometry;
	shader_constant_host*	m_to_sun_direction_parameter;
	shader_constant_host*	m_c_inverted_view_projection_matrix;
	shader_constant_host*	m_c_atmosphere_parameters;
	shader_constant_host*	m_sky_clouds_parameters0;
	shader_constant_host*	m_sky_clouds_parameters1;
	shader_constant_host*	m_sky_clouds_parameters2;
	shader_constant_host*	m_sun_moon_parameters;
	shader_constant_host*	m_c_inscatter_parameters;
	shader_constant_host*	m_c_eye_ray_corner;
	stage_type				m_type;
};

STATIC_SIZE_ASSERT( stage_atmosphere, 0x6C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_ATMOSPHERE_H_INCLUDED

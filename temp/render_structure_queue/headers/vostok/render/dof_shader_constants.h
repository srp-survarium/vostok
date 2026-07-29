////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DOF_SHADER_CONSTANTS_H_INCLUDED
#define RENDER_DOF_SHADER_CONSTANTS_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;

namespace vostok {
namespace render {

struct dof_shader_constants {
			dof_shader_constants( );

	void	set					(
				float3 const&		blurriness_height_lights,
				float				distance,
				float				region,
				float				power,
				float				near_blur_amout,
				float				far_blur_amout,
				float				bokeh_dof_radius,
				float				bokeh_dof_density
			);

private:
	/* 0x0000 */	shader_constant_host*		m_dof_parameters;
	/* 0x0004 */	shader_constant_host*		m_dof_height_lights;
	/* 0x0008 */	shader_constant_host*		m_blurriness_amount;
	/* 0x000c */	shader_constant_host*		m_bokeh_dof_parameters;
}; // struct dof_shader_constants

STATIC_SIZE_ASSERT(dof_shader_constants, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DOF_SHADER_CONSTANTS_H_INCLUDED

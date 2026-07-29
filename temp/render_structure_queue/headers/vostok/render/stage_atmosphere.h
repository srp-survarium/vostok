////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_ATMOSPHERE_H_INCLUDED
#define RENDER_STAGE_ATMOSPHERE_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
class vostok::render::stage;
struct vostok::render::sky_dome_geometry;
struct vostok::render::sphere_geometry;
class vostok::render::render_target;
class vostok::render::stage_atmosphere::stage_type;
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;
class vostok::render::res_geometry;
class vostok::render::untyped_buffer;

/* FORWARD REFS */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

enum vostok::render::stage_atmosphere::stage_type
{
	atmosphere_on_sky			= 0x0,
	atmosphere_on_geometry		= 0x1,
};

class stage_atmosphere : public stage {
public:
	inline			stage_atmosphere	( renderer* arg_0, renderer_context* arg_1, stage_atmosphere::stage_type arg_2 ) { /* no source */ }
	virtual			~stage_atmosphere	( );

	virtual	void	execute				( ) override;

			bool	is_effects_ready	( ) const;

			void	fill_surfaces		( render_target_ptr surf0, render_target_ptr surf1, bool use_base_depth_stencil );

	/* 0x0000 */	/* stage */
private:
	/* 0x0010 */	sky_dome_geometry				m_sky_dome_geometry;
	/* 0x0024 */	sphere_geometry					m_clouds_geometry;
	/* 0x0038 */	res_effect_ptr					m_atmospheric_scattering_effect;
	/* 0x003c */	untyped_buffer_ptr				m_screen_vertex_ib;
	/* 0x0040 */	res_geometry_ptr				m_screen_vertex_geometry;
	/* 0x0044 */	shader_constant_host*			m_to_sun_direction_parameter;
	/* 0x0048 */	shader_constant_host*			m_c_inverted_view_projection_matrix;
	/* 0x004c */	shader_constant_host*			m_c_atmosphere_parameters;
	/* 0x0050 */	shader_constant_host*			m_sky_clouds_parameters0;
	/* 0x0054 */	shader_constant_host*			m_sky_clouds_parameters1;
	/* 0x0058 */	shader_constant_host*			m_sky_clouds_parameters2;
	/* 0x005c */	shader_constant_host*			m_sun_moon_parameters;
	/* 0x0060 */	shader_constant_host*			m_c_inscatter_parameters;
	/* 0x0064 */	shader_constant_host*			m_c_eye_ray_corner;
	/* 0x0068 */	stage_atmosphere::stage_type	m_type;
}; // class stage_atmosphere

STATIC_SIZE_ASSERT(stage_atmosphere, 0x6C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_ATMOSPHERE_H_INCLUDED

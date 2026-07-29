////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_BILLBOARD_PARAMETERS_H_INCLUDED
#define RENDER_SPEEDTREE_BILLBOARD_PARAMETERS_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;

/* FORWARD REFS */
class vostok::render::renderer_context;
class vostok::render::speedtree_tree_component;

namespace vostok {
namespace render {

struct speedtree_billboard_parameters {
			speedtree_billboard_parameters	( );

	void	set								( renderer_context* context, speedtree_tree_component* tree_component );

private:
	void	set_billboard_tangents			( float camera_azimuth );

public:
	/* 0x0000 */	shader_constant_host*		m_billboard_tangents_parameter;
	/* 0x0004 */	shader_constant_host*		m_billboard_dimensions_parameter;
	/* 0x0008 */	shader_constant_host*		m_camera_azimuth_trig_parameter;
	/* 0x000c */	shader_constant_host*		m_camera_angles_parameter;
	/* 0x0010 */	shader_constant_host*		m_num360images_parameter;
	/* 0x0014 */	shader_constant_host*		m_tex_coords_360_parameter;
	/* 0x0018 */	shader_constant_host*		m_view_location_parameter;
}; // struct speedtree_billboard_parameters

STATIC_SIZE_ASSERT(speedtree_billboard_parameters, 0x1C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_BILLBOARD_PARAMETERS_H_INCLUDED

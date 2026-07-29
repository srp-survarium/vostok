////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_COMMON_PARAMETERS_H_INCLUDED
#define RENDER_SPEEDTREE_COMMON_PARAMETERS_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;

/* FORWARD REFS */
class vostok::render::renderer_context;
class vostok::render::speedtree_tree_component;

namespace vostok {
namespace render {

struct speedtree_common_parameters {
			speedtree_common_parameters	( );

	void	set							(
				renderer_context*				context,
				speedtree_tree_component*		tree_component,
				float3 const&					lod_reference_position
			);

	/* 0x0000 */	shader_constant_host*		m_camera_facing_matrix_parameter;
	/* 0x0004 */	shader_constant_host*		m_lod_profile_parameter;
	/* 0x0008 */	shader_constant_host*		m_lod_reference_position_parameter;
}; // struct speedtree_common_parameters

STATIC_SIZE_ASSERT(speedtree_common_parameters, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_COMMON_PARAMETERS_H_INCLUDED

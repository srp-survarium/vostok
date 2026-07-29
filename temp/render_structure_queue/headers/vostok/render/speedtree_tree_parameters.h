////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_TREE_PARAMETERS_H_INCLUDED
#define RENDER_SPEEDTREE_TREE_PARAMETERS_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;

/* FORWARD REFS */
class vostok::render::speedtree_tree_component;

namespace vostok {
namespace render {

struct speedtree_tree_parameters {
			speedtree_tree_parameters	( );

	void	set							(
				speedtree_tree_component*			tree_component,
				SpeedTree::CInstance const*			instance,
				SpeedTree::SInstanceLod const*		instance_lod
			);

	/* 0x0000 */	shader_constant_host*		m_tree_position_and_scale_parameter;
	/* 0x0004 */	shader_constant_host*		m_tree_rotation_parameter;
}; // struct speedtree_tree_parameters

STATIC_SIZE_ASSERT(speedtree_tree_parameters, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_TREE_PARAMETERS_H_INCLUDED

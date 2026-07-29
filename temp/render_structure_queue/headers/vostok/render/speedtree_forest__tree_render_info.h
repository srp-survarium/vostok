////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_FOREST_TREE_RENDER_INFO_H_INCLUDED
#define RENDER_SPEEDTREE_FOREST_TREE_RENDER_INFO_H_INCLUDED

/* INCLUDES */
class SpeedTree::CInstance;
struct SpeedTree::SInstanceLod;
class vostok::render::speedtree_tree_component;
struct vostok::render::lod_entry;

namespace vostok {
namespace render {

struct speedtree_forest::tree_render_info {
	inline		tree_render_info(
					lod_entry const*					arg_0,
					SpeedTree::CInstance const*			arg_1,
					SpeedTree::SInstanceLod const*		arg_2,
					speedtree_tree_component*			arg_3
				) { /* no source */ }

	/* 0x0000 */	lod_entry const*					lod;
	/* 0x0004 */	SpeedTree::CInstance const*			instance;
	/* 0x0008 */	SpeedTree::SInstanceLod const*		instance_lod;
	/* 0x000c */	speedtree_tree_component*			tree_component;
}; // struct speedtree_forest::tree_render_info

STATIC_SIZE_ASSERT(speedtree_forest::tree_render_info, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_FOREST_TREE_RENDER_INFO_H_INCLUDED

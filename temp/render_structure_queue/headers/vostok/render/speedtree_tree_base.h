////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_TREE_BASE_H_INCLUDED
#define RENDER_SPEEDTREE_TREE_BASE_H_INCLUDED

/* INCLUDES */
class vostok::math::aabb;
class vostok::resources::unmanaged_resource;

namespace vostok {
namespace render {

struct speedtree_tree_base : public resources::unmanaged_resource {
	inline		speedtree_tree_base	( ) { /* no source */ }
	virtual		~speedtree_tree_base( ) { /* no source */ }

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	math::aabb		m_bbox;
}; // struct speedtree_tree_base

STATIC_SIZE_ASSERT(speedtree_tree_base, 0x120);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_TREE_BASE_H_INCLUDED

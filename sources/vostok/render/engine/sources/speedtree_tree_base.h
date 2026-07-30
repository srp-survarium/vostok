#ifndef VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_BASE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_BASE_H_INCLUDED

#include <vostok/math_aabb.h>
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace render {

struct speedtree_tree_base : public resources::unmanaged_resource {
	speedtree_tree_base( ) : m_bbox( math::create_zero_aabb( ) ) { }
	virtual ~speedtree_tree_base( ) { }

	math::aabb m_bbox;
}; // struct speedtree_tree_base

STATIC_SIZE_ASSERT(speedtree_tree_base, 0x120);

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_BASE_H_INCLUDED

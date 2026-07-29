////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_INSTANCE_H_INCLUDED
#define RENDER_SPEEDTREE_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_resource;
typedef vostok::resources::resource_ptr<vostok::render::speedtree_tree_base,vostok::resources::unmanaged_intrusive_base>
	vostok::render::speedtree_tree_base_ptr;
class vostok::render::speedtree_tree_base;

namespace vostok {
namespace render {

class speedtree_instance : public resources::unmanaged_resource {
public:
	inline	explicit	speedtree_instance	( speedtree_tree_base_ptr in_speedtree_tree_ptr ) { /* no source */ }
	virtual				~speedtree_instance	( ) { /* no source */ }

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	float4x4					m_transform;
	/* 0x0148 */	speedtree_tree_base_ptr		m_speedtree_tree_ptr;
}; // class speedtree_instance

STATIC_SIZE_ASSERT(speedtree_instance, 0x150);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_INSTANCE_H_INCLUDED

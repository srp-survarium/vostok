#ifndef VOSTOK_RENDER_ENGINE_SPEEDTREE_INSTANCE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPEEDTREE_INSTANCE_H_INCLUDED

#include <vostok/math_float4x4.h>
#include <vostok/resources_resource_ptr.h>

#include "speedtree_tree_base.h"

namespace vostok {
namespace render {

typedef resources::resource_ptr<
	speedtree_tree_base,
	resources::unmanaged_intrusive_base
> speedtree_tree_base_ptr;

class speedtree_instance : public resources::unmanaged_resource {
public:
	explicit speedtree_instance( speedtree_tree_base_ptr tree ) :
		m_speedtree_tree_ptr( tree )
	{
		// FUNCTION BODY[0x8c7c0]
	}

	virtual ~speedtree_instance( ) { }

	float4x4 m_transform;
	speedtree_tree_base_ptr m_speedtree_tree_ptr;
};

STATIC_SIZE_ASSERT( speedtree_instance, 0x150 );

typedef resources::resource_ptr<
	speedtree_instance,
	resources::unmanaged_intrusive_base
> speedtree_instance_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SPEEDTREE_INSTANCE_H_INCLUDED

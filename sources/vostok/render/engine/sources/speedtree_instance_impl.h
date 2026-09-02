// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_SPEEDTREE_INSTANCE_IMPL_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPEEDTREE_INSTANCE_IMPL_H_INCLUDED
#include <vostok/render/facade/model.h>

namespace SpeedTree {
class CInstance;
}

namespace vostok {
namespace render {

struct speedtree_forest;

class speedtree_instance_impl : public speedtree_instance {
public:
	explicit speedtree_instance_impl( speedtree_tree_base_ptr tree );
	virtual ~speedtree_instance_impl( );

	void set_transform( float4x4 const& transform );
	SpeedTree::CInstance const& get_speedtree_instance( ) const;

protected:
	friend struct speedtree_forest;

	SpeedTree::CInstance* m_speedtree_instance;
	s32 m_speedtree_instance_index;
};

STATIC_SIZE_ASSERT( speedtree_instance_impl, 0x158 );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_SPEEDTREE_INSTANCE_IMPL_H_INCLUDED

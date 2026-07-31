#include "pch.h"
#include "speedtree_convert_type.h"
#include "speedtree_instance_impl.h"

namespace vostok {
namespace render {

speedtree_instance_impl::speedtree_instance_impl( speedtree_tree_base_ptr tree ) :
	speedtree_instance( tree ),
	m_speedtree_instance( NULL ),
	m_speedtree_instance_index( -1 )
{
	// FUNCTION BODY[0x5f3180]
	m_speedtree_instance = NEW(SpeedTree::CInstance);
}

speedtree_instance_impl::~speedtree_instance_impl( )
{
	// FUNCTION BODY[0x5f3100]
	DELETE(m_speedtree_instance);
}

void speedtree_instance_impl::set_transform( float4x4 const& transform )
{
	// FUNCTION BODY[0x5f2f90]
	m_transform							= transform;
	float3 rotation_angles				= transform.get_angles(math::rotation_zxy);
	float3 scale						= transform.get_scale();
	m_speedtree_instance->SetPos		(vostok_to_speedtree(transform.lines[3].xyz()));
	m_speedtree_instance->SetRotation	(-rotation_angles.y);
	m_speedtree_instance->SetScale		(math::max(scale.x, math::max(scale.y, scale.z)));
}

SpeedTree::CInstance const& speedtree_instance_impl::get_speedtree_instance( ) const
{
	// FUNCTION BODY[0x5f2f80]
	return *m_speedtree_instance;
}

} // namespace render
} // namespace vostok

#include "pch.h"
#include "speedtree_instance_impl.h"

namespace vostok {
namespace render {

speedtree_instance_impl::speedtree_instance_impl( speedtree_tree_base_ptr tree ) :
	speedtree_instance( tree ),
	m_speedtree_instance( NULL ),
	m_speedtree_instance_index( -1 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f3180]
}

speedtree_instance_impl::~speedtree_instance_impl( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f3100]
}

void speedtree_instance_impl::set_transform( float4x4 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f2f90]
}

SpeedTree::CInstance const& speedtree_instance_impl::get_speedtree_instance( ) const
{
	// FUNCTION BODY[0x5f2f80]
	return *m_speedtree_instance;
}

} // namespace render
} // namespace vostok

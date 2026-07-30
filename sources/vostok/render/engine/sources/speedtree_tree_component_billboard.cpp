#include "pch.h"
#include "speedtree_tree_component_billboard.h"

namespace vostok {
namespace render {

speedtree_tree_component_billboard::speedtree_tree_component_billboard( speedtree_tree& parent ) :
	speedtree_tree_component( parent ),
	m_is_init( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f5f70]
}

void speedtree_tree_component_billboard::init(
	speedtree_forest&,
	SpeedTree::CArray<SpeedTree::CInstance, 1> const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f55e0]
}

void speedtree_tree_component_billboard::set_material( material_ptr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f55b0]
}

void speedtree_tree_component_billboard::set_default_material( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f5590]
}

void speedtree_tree_component_billboard::render( lod_entry const*, renderer_context* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f55a0]
}

} // namespace render
} // namespace vostok

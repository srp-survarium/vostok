#include "pch.h"
#include "speedtree_tree_component_leafcard.h"

namespace vostok {
namespace render {

speedtree_tree_component_leafcard::speedtree_tree_component_leafcard( speedtree_tree& parent ) :
	speedtree_tree_component( parent )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f64e0]
}

void speedtree_tree_component_leafcard::init_index_buffer(
	SpeedTree::SLeafCards const*,
	vector<u16>&,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f5fd0]
}

void speedtree_tree_component_leafcard::set_material( material_ptr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f64b0]
}

void speedtree_tree_component_leafcard::set_default_material( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f5fb0]
}

void speedtree_tree_component_leafcard::render( lod_entry const*, renderer_context* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f5fc0]
}

} // namespace render
} // namespace vostok

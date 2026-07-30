#include "pch.h"
#include "speedtree_tree_component_branch.h"

namespace vostok {
namespace render {

speedtree_tree_component_branch::speedtree_tree_component_branch( speedtree_tree& parent ) :
	speedtree_tree_component( parent )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f8760]
}

void speedtree_tree_component_branch::init_index_buffer( SpeedTree::SIndexedTriangles const*, vector<u16>& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f83b0]
}

void speedtree_tree_component_branch::set_material( material_ptr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f8730]
}

void speedtree_tree_component_branch::set_default_material( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f8390]
}

void speedtree_tree_component_branch::render( lod_entry const*, renderer_context* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f83a0]
}

} // namespace render
} // namespace vostok

#include "pch.h"
#include "speedtree_tree_component_leafmesh.h"

namespace vostok {
namespace render {

speedtree_tree_component_leafmesh::speedtree_tree_component_leafmesh( speedtree_tree& parent ) :
	speedtree_tree_component( parent )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f7160]
}

void speedtree_tree_component_leafmesh::init_index_buffer( SpeedTree::SIndexedTriangles const*, vector<u16>& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f6fb0]
}

void speedtree_tree_component_leafmesh::set_material( material_ptr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f7130]
}

void speedtree_tree_component_leafmesh::set_default_material( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f6f90]
}

void speedtree_tree_component_leafmesh::render( lod_entry const*, renderer_context* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5f6fa0]
}

} // namespace render
} // namespace vostok

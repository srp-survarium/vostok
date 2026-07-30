#include "pch.h"
#include "speedtree_tree.h"

namespace vostok {
namespace render {

speedtree_tree_component::speedtree_tree_component( speedtree_tree& parent ) :
	m_parent( &parent )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63cb10]
}

speedtree_tree_component::~speedtree_tree_component( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63cac0]
}

material_effects& speedtree_tree_component::get_material_effects( )
{
	// FUNCTION BODY[0x63c740]
	return m_materail_effects_instance->get_material_effects( );
}

void speedtree_tree_component::set_material_effects( material_effects_instance_ptr instance, pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63c930]
	m_materail_effects_instance = instance;
}

speedtree_tree::speedtree_tree( pcvoid, u32 ) :
	m_branch_component( 0 ),
	m_frond_component( 0 ),
	m_leafmesh_component( 0 ),
	m_leafcard_component( 0 ),
	m_billboard_component( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63cc50]
}

speedtree_tree::~speedtree_tree( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63c760]
}

lod_render_info& speedtree_tree::get_lod_render_info( SpeedTree::EGeometryType type )
{
	// FUNCTION BODY[0x63c730]
	return m_lod_render_info[type];
}

void speedtree_tree::load( pcvoid, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63cb50]
}

void speedtree_tree::set_material_effects( material_effects_instance_ptr, component_type, pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63ca00]
}

} // namespace render
} // namespace vostok

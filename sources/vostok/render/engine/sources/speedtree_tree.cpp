#include "pch.h"
#include "speedtree_tree.h"

#include "material_manager.h"
#include "speedtree_tree_component_billboard.h"
#include "speedtree_tree_component_branch.h"
#include "speedtree_tree_component_frond.h"
#include "speedtree_tree_component_leafcard.h"
#include "speedtree_tree_component_leafmesh.h"

namespace vostok {
namespace render {

using namespace SpeedTree;

static float const scale_speedtree_to_vostok = 1.0f;//0.3048f;

speedtree_tree_component::speedtree_tree_component( speedtree_tree& parent )
{
	m_parent = &parent;
}

speedtree_tree_component::~speedtree_tree_component( )
{
	material_manager::ref().remove_material_effects(m_materail_effects_instance);
}

material_effects& speedtree_tree_component::get_material_effects( )
{
	if ( !m_materail_effects_instance.c_ptr( ) )
		return material::nomaterial_material( get_vertex_input_type( ) );

	return m_materail_effects_instance->get_material_effects( );
}

void speedtree_tree_component::set_material_effects( material_effects_instance_ptr instance, pcstr material_name )
{
	if (instance)
	{
		material_manager::ref().remove_material_effects(m_materail_effects_instance);

		m_materail_effects_instance	= instance;

		material_manager::ref().add_material_effects(m_materail_effects_instance, material_name);
	}
}

speedtree_tree::speedtree_tree( pcvoid data, u32 size ) :
	m_branch_component( 0 ),
	m_frond_component( 0 ),
	m_leafmesh_component( 0 ),
	m_leafcard_component( 0 ),
	m_billboard_component( 0 )
{
	m_bbox.identity();
	load(data, size);
}

speedtree_tree::~speedtree_tree( )
{
	if (m_branch_component)		DELETE(m_branch_component);
	if (m_frond_component)		DELETE(m_frond_component);
	if (m_leafmesh_component)	DELETE(m_leafmesh_component);
	if (m_leafcard_component)	DELETE(m_leafcard_component);
	if (m_billboard_component)	DELETE(m_billboard_component);

	for (u32 i=0; i<SpeedTree::GEOMETRY_TYPE_NUM_3D_TYPES; i++)
	{
		if (m_lod_render_info[i].num_lods)
		{
			DELETE_ARRAY(m_lod_render_info[i].lods);
		}
	}
}

lod_render_info& speedtree_tree::get_lod_render_info( SpeedTree::EGeometryType type )
{
	return m_lod_render_info[type];
}

void speedtree_tree::load( pcvoid data, u32 size )
{
	LoadTree((st_byte const*)data, size, false, scale_speedtree_to_vostok);

	const SGeometry* tree_geometry = GetGeometry( );

	if (!tree_geometry)
		return;

	if (tree_geometry->m_nNumBranchLods)	m_branch_component	 = NEW(speedtree_tree_component_branch)  (*this);
	if (tree_geometry->m_nNumFrondLods)		m_frond_component	 = NEW(speedtree_tree_component_frond)   (*this);
	if (tree_geometry->m_nNumLeafMeshLods)	m_leafmesh_component = NEW(speedtree_tree_component_leafmesh)(*this);
	if (tree_geometry->m_nNumLeafCardLods)	m_leafcard_component = NEW(speedtree_tree_component_leafcard)(*this);

	if (HasGeometryType(GEOMETRY_TYPE_VERTICAL_BILLBOARDS))	m_billboard_component = NEW(speedtree_tree_component_billboard)(*this);
}

void speedtree_tree::set_material_effects( material_effects_instance_ptr mtl_instance_ptr, component_type in_component_type, pcstr material_name )
{
	speedtree_tree_component* mtl_set_component				= 0;

	switch (in_component_type)
	{
		case speedtree_tree::branch:	mtl_set_component	= m_branch_component; break;
		case speedtree_tree::frond:		mtl_set_component	= m_frond_component; break;
		case speedtree_tree::leafmesh:	mtl_set_component	= m_leafmesh_component; break;
		case speedtree_tree::leafcard:	mtl_set_component	= m_leafcard_component; break;
		case speedtree_tree::billboard: mtl_set_component	= m_billboard_component; break;
		default: NODEFAULT(return;);
	}

	if (mtl_set_component)
	{
		if (mtl_instance_ptr)
			mtl_set_component->set_material_effects(mtl_instance_ptr, material_name);
		else
			mtl_set_component->set_default_material			();
	}
}

} // namespace render
} // namespace vostok

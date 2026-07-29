////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_TREE_COMPONENT_BRANCH_H_INCLUDED
#define RENDER_SPEEDTREE_TREE_COMPONENT_BRANCH_H_INCLUDED

/* INCLUDES */
class vostok::render::speedtree_tree_component;
class vostok::render::enum_vertex_input_type;
class vostok::render::material;

/* FORWARD REFS */
class vostok::render::lod_entry;
class vostok::render::renderer_context;
class vostok::render::speedtree_tree;
class vostok::render::vector<unsigned short>;

namespace vostok {
namespace render {

class speedtree_tree_component_branch : public speedtree_tree_component {
public:
			explicit					speedtree_tree_component_branch	( speedtree_tree& parent );

	virtual	void						set_material					( material_ptr mtl_ptr ) override;
	virtual	void						set_default_material			( ) override;

	virtual	SpeedTree::EGeometryType	get_geometry_type				( ) const override { /* no source */ }

	virtual	void						render							( lod_entry const* lod, renderer_context* context ) override;

	virtual	enum_vertex_input_type		get_vertex_input_type			( ) override { /* no source */ }

			void						init_index_buffer				( SpeedTree::SIndexedTriangles const* lod, vector< u16 >& out_indices );

	virtual								~speedtree_tree_component_branch( ) { /* no source */ }
}; // class speedtree_tree_component_branch

STATIC_SIZE_ASSERT(speedtree_tree_component_branch, 0x70);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_TREE_COMPONENT_BRANCH_H_INCLUDED

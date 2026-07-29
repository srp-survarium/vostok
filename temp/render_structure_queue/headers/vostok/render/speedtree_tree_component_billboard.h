////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_TREE_COMPONENT_BILLBOARD_H_INCLUDED
#define RENDER_SPEEDTREE_TREE_COMPONENT_BILLBOARD_H_INCLUDED

/* INCLUDES */
class vostok::render::speedtree_tree_component;
class vostok::render::enum_vertex_input_type;
class vostok::render::material;

/* FORWARD REFS */
class vostok::render::lod_entry;
class vostok::render::renderer_context;
class vostok::render::speedtree_forest;
class vostok::render::speedtree_tree;

namespace vostok {
namespace render {

class speedtree_tree_component_billboard : public speedtree_tree_component {
public:
			explicit					speedtree_tree_component_billboard	( speedtree_tree& parent );

			void						init								(
											speedtree_forest&		forest,
											SpeedTree::CArray< SpeedTree::CInstance, 1 > const&	instances_of_tree
										);

	virtual	void						set_material						( material_ptr mtl_ptr ) override;
	virtual	void						set_default_material				( ) override;

	virtual	SpeedTree::EGeometryType	get_geometry_type					( ) const override { /* no source */ }

	virtual	void						render								( lod_entry const* lod, renderer_context* context ) override;

	virtual	enum_vertex_input_type		get_vertex_input_type				( ) override { /* no source */ }

	inline	bool						is_initialized						( ) const { /* no source */ }

	virtual								~speedtree_tree_component_billboard	( ) { /* no source */ }

	/* 0x0000 */	/* speedtree_tree_component */
	/* 0x0070 */	bool	m_is_init;
}; // class speedtree_tree_component_billboard

STATIC_SIZE_ASSERT(speedtree_tree_component_billboard, 0x74);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_TREE_COMPONENT_BILLBOARD_H_INCLUDED

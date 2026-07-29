////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_TREE_COMPONENT_H_INCLUDED
#define RENDER_SPEEDTREE_TREE_COMPONENT_H_INCLUDED

/* INCLUDES */
class vostok::render::speedtree_tree;
struct vostok::render::render_geometry;
class vostok::render::enum_vertex_input_type;
class vostok::render::material;
typedef vostok::resources::resource_ptr<vostok::render::material_effects_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::material_effects_instance_ptr;
class vostok::render::material_effects_instance;

/* FORWARD REFS */
class vostok::render::lod_entry;
class vostok::render::material_effects;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class speedtree_tree_component {
public:
			explicit					speedtree_tree_component	( speedtree_tree& parent );
	virtual								~speedtree_tree_component	( );

	virtual	void						set_material				( material_ptr arg_0 ) = 0;
	virtual	void						set_default_material		( ) = 0;

	virtual	void						render						( lod_entry const* arg_0, renderer_context* arg_1 ) = 0;

	virtual	SpeedTree::EGeometryType	get_geometry_type			( ) const = 0;

			void						set_material_effects		( material_effects_instance_ptr mtl_instance_ptr, pcstr material_name );
			material_effects&			get_material_effects		( );
	virtual	enum_vertex_input_type		get_vertex_input_type		( ) = 0;

	/* 0x0004 */	speedtree_tree*						m_parent;
	/* 0x0008 */	render_geometry						m_render_geometry;
protected:
	/* 0x006c */	material_effects_instance_ptr		m_materail_effects_instance;
}; // class speedtree_tree_component

STATIC_SIZE_ASSERT(speedtree_tree_component, 0x70);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_TREE_COMPONENT_H_INCLUDED

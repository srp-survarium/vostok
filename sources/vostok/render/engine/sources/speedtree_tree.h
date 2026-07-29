#ifndef RENDER_SPEEDTREE_TREE_H_INCLUDED
#define RENDER_SPEEDTREE_TREE_H_INCLUDED

#include "lod_render_info.h"
#include "speedtree_tree_base.h"

#include <vostok/resources_resource_ptr.h>

namespace vostok {
namespace render {

class material_effects_instance;
class speedtree_tree_component;
class speedtree_tree_component_billboard;

typedef resources::resource_ptr<
	material_effects_instance,
	resources::unmanaged_intrusive_base
> material_effects_instance_ptr;

class speedtree_tree : public speedtree_tree_base , public SpeedTree::CCore {
public:
	enum component_type {
		branch,
		frond,
		leafmesh,
		leafcard,
		billboard,
	};

										speedtree_tree		( pcvoid data, u32 size );
	virtual								~speedtree_tree		( );

	inline	lod_render_info const&		get_lod_render_info	( SpeedTree::EGeometryType arg_0 ) const { return m_lod_render_info[ 0 ]; }
			lod_render_info&			get_lod_render_info	( SpeedTree::EGeometryType type );

			void						set_material_effects(
											material_effects_instance_ptr		mtl_instance_ptr,
											speedtree_tree::component_type		in_component_type,
											pcstr								material_name
										);

protected:
			void						load				( pcvoid data, u32 size );

	/* 0x0000 */	/* speedtree_tree_base */
	/* 0x0120 */	/* SpeedTree::CCore */
	/* 0x0f44 */	speedtree_tree_component*				m_branch_component;
	/* 0x0f48 */	speedtree_tree_component*				m_frond_component;
	/* 0x0f4c */	speedtree_tree_component*				m_leafmesh_component;
	/* 0x0f50 */	speedtree_tree_component*				m_leafcard_component;
	/* 0x0f54 */	speedtree_tree_component_billboard*		m_billboard_component;
	/* 0x0f58 */	lod_render_info							m_lod_render_info[6];
}; // class speedtree_tree

STATIC_SIZE_ASSERT(speedtree_tree, 0xF88);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_TREE_H_INCLUDED

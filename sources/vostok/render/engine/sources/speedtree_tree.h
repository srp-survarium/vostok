// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_H_INCLUDED
#include <speedtree/core/core.h>
#include <vostok/render/facade/model.h>
#include <vostok/render/facade/vertex_input_type.h>
#include "material.h"
#include "material_effects_instance.h"
#include "render_geometry.h"

namespace vostok {
namespace render {

class renderer_context;
class speedtree_tree;
class speedtree_tree_component_billboard;

struct lod_entry {
	lod_entry( ) : start_index( 0 ), num_indices( 0 ) { }

	u32 start_index;
	u32 num_indices;
}; // struct lod_entry

struct lod_render_info {
	lod_render_info( ) : lods( 0 ), num_lods( 0 ) { }

	lod_entry*	lods;
	u32			num_lods;

	inline bool has_geometry( u32 lod_index ) const
	{
		lod_entry& l = lods[lod_index];
		return l.num_indices != 0;
	}

	inline lod_entry const* is_active( u32 lod_index ) const
	{
		lod_entry* l = &lods[lod_index];

		if (l->num_indices == 0)
			return 0;

		return l;
	}
}; // struct lod_render_info

class speedtree_tree_component {
public:
	explicit speedtree_tree_component( speedtree_tree& parent );
	virtual ~speedtree_tree_component( );

	virtual void set_material( material_ptr material ) = 0;
	virtual void set_default_material( ) = 0;
	virtual void render( lod_entry const* lod, renderer_context* context ) = 0;
	virtual SpeedTree::EGeometryType get_geometry_type( ) const = 0;

	void set_material_effects( material_effects_instance_ptr instance, pcstr material_name );
	material_effects& get_material_effects( );
	virtual enum_vertex_input_type get_vertex_input_type( ) = 0;

	speedtree_tree* m_parent;
	render_geometry m_render_geometry;

protected:
	material_effects_instance_ptr m_materail_effects_instance;
};

STATIC_SIZE_ASSERT( speedtree_tree_component, 0x70 );

class speedtree_tree : public speedtree_tree_base, public SpeedTree::CCore {
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

	inline	lod_render_info const&		get_lod_render_info	( SpeedTree::EGeometryType type ) const { return m_lod_render_info[ type ]; }
			lod_render_info&			get_lod_render_info	( SpeedTree::EGeometryType type );

			void						set_material_effects(
											material_effects_instance_ptr		mtl_instance_ptr,
											speedtree_tree::component_type		in_component_type,
											pcstr								material_name
										);

protected:
			void						load				( pcvoid data, u32 size );

protected:
	friend class speedtree_tree_component;
	friend struct speedtree_cook;
	friend struct speedtree_forest;

	/* 0x0000 */	/* speedtree_tree_base */
	/* 0x0120 */	/* SpeedTree::CCore */
	/* 0x0f44 */	speedtree_tree_component*				m_branch_component;
	/* 0x0f48 */	speedtree_tree_component*				m_frond_component;
	/* 0x0f4c */	speedtree_tree_component*				m_leafmesh_component;
	/* 0x0f50 */	speedtree_tree_component*				m_leafcard_component;
	/* 0x0f54 */	speedtree_tree_component_billboard*		m_billboard_component;
	/* 0x0f58 */	lod_render_info							m_lod_render_info[SpeedTree::GEOMETRY_TYPE_COUNT];
}; // class speedtree_tree

STATIC_SIZE_ASSERT(speedtree_tree, 0xF88);

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_H_INCLUDED

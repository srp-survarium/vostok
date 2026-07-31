#ifndef VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_COMPONENT_BILLBOARD_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_COMPONENT_BILLBOARD_H_INCLUDED

#include "speedtree_tree.h"

namespace vostok {
namespace render {

struct speedtree_forest;

struct billboard_vertex {
	billboard_vertex(
		float3 const& position,
		float scale,
		float3 const direction,
		float rotation,
		u8 corner
	) :
		position_and_scale( position, scale ),
		direction_and_rotation( direction, rotation ),
		corner_index( corner )
	{
	}

	float4 position_and_scale;
	float4 direction_and_rotation;
	u8 corner_index;
};

STATIC_SIZE_ASSERT( billboard_vertex, 0x24 );

class speedtree_tree_component_billboard : public speedtree_tree_component {
public:
	explicit speedtree_tree_component_billboard( speedtree_tree& parent );
	virtual ~speedtree_tree_component_billboard( ) { }

	void init(
		speedtree_forest& forest,
		SpeedTree::CArray<SpeedTree::CInstance, 1> const& instances_of_tree
	);

	virtual void set_material( material_ptr material );
	virtual void set_default_material( );
	virtual void render( lod_entry const* lod, renderer_context* context );

	virtual SpeedTree::EGeometryType get_geometry_type( ) const
	{
		// FUNCTION BODY[0x3dde0]
		return SpeedTree::GEOMETRY_TYPE_VERTICAL_BILLBOARDS;
	}

	virtual enum_vertex_input_type get_vertex_input_type( )
	{
		// claude@NOTE: no legacy ancestor - consumed legacy inline returned the retired speedtree_billboard_vertex_input_type enumerator; matcher-phase work.
		// STATE[STUB]
		return unknown_vertex_input_type;
	}

	bool is_initialized( ) const { return m_is_init; }

private:
	bool m_is_init;
};

STATIC_SIZE_ASSERT( speedtree_tree_component_billboard, 0x74 );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_SPEEDTREE_TREE_COMPONENT_BILLBOARD_H_INCLUDED

#ifndef VOSTOK_RENDER_ENGINE_SPEEDTREE_FOREST_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPEEDTREE_FOREST_H_INCLUDED

#include <speedtree/core/core.h>
#include <speedtree/forest/forest.h>
#include <speedtree/forest/grass.h>

#include <vostok/math_float3.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/core/memory.h>
#include <vostok/resources_resource_ptr.h>

#include "speedtree_instance.h"
#include "speedtree_shader_parameters.h"
#include "speedtree_tree_base.h"

namespace vostok {
namespace render {

struct lod_entry;
class renderer_context;
class speedtree_tree_component;

struct speedtree_forest {
	struct tree_render_info {
		tree_render_info(
			lod_entry const* in_lod,
			SpeedTree::CInstance const* in_instance,
			SpeedTree::SInstanceLod const* in_instance_lod,
			speedtree_tree_component* in_tree_component
		) :
			lod			( in_lod ),
			instance		( in_instance ),
			instance_lod	( in_instance_lod ),
			tree_component	( in_tree_component )
		{
		}

		lod_entry const*				lod;
		SpeedTree::CInstance const*		instance;
		SpeedTree::SInstanceLod const*	instance_lod;
		speedtree_tree_component*		tree_component;
	};

	STATIC_SIZE_ASSERT( tree_render_info, 0x10 );

	typedef vector<tree_render_info> tree_render_info_array_type;
	typedef vector<speedtree_tree_base_ptr> trees_type;
	typedef vector<speedtree_instance_ptr> tree_instances_type;

	speedtree_forest( );
	~speedtree_forest( );

	SpeedTree::CForest& get_forest( ) const { return *m_forest; }
	void populate_forest( );

	void add_instance( speedtree_instance_ptr instance, float4x4 const& transform );
	void remove_instance( speedtree_instance_ptr instance );
	void set_transform( speedtree_instance_ptr instance, float4x4 const& transform );
	void set_wind_direction( float3 const& wind_direction );
	void set_wind_strength( float wind_strength );

	speedtree_wind_parameters& get_speedtree_wind_parameters( )
	{
		return m_speedtree_wind_parameters;
	}

	speedtree_billboard_parameters& get_speedtree_billboard_parameters( )
	{
		return m_speedtree_billboard_parameters;
	}

	speedtree_tree_parameters& get_speedtree_tree_parameters( )
	{
		return m_speedtree_tree_parameters;
	}

	speedtree_common_parameters& get_speedtree_common_parameters( )
	{
		return m_speedtree_common_parameters;
	}

	void get_visible_tree_components(
		renderer_context* context,
		float3 const& lod_reference_point,
		bool sort_result,
		tree_render_info_array_type& out_tree_render_info_array
	);

	void get_visible_tree_components(
		renderer_context* context,
		vector<speedtree_instance_ptr> const& instances,
		bool sort_result,
		tree_render_info_array_type& out_tree_render_info_array
	);

	u32 get_num_visible_instances( ) const
	{
		// claude@NOTE: the available developer body is only a stub.
		// STATE[STUB]
		return 0;
	}

	void tick( renderer_context* context );

	SpeedTree::CView& get_speedtree_view( ) { return m_view; }
	float4x4 get_instance_transform( SpeedTree::CInstance const& instance );

private:
	void cull_and_compute_lod(
		renderer_context* context,
		float3 const& lod_reference_point,
		bool sort_result
	);

public:
	speedtree_wind_parameters		m_speedtree_wind_parameters;
	speedtree_billboard_parameters	m_speedtree_billboard_parameters;
	speedtree_tree_parameters		m_speedtree_tree_parameters;
	speedtree_common_parameters		m_speedtree_common_parameters;
	SpeedTree::SForestCullResults	m_visible_trees;
	SpeedTree::SGrassCullResults		m_visuble_grass;
	SpeedTree::CView				m_view;
	SpeedTree::CGrass				m_grass;
	SpeedTree::CForest*				m_forest;
	trees_type						m_trees;
	tree_instances_type				m_tree_instances;
	SpeedTree::CWind				m_wind_leader;
};

STATIC_SIZE_ASSERT( speedtree_forest, 0x1CF4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SPEEDTREE_FOREST_H_INCLUDED

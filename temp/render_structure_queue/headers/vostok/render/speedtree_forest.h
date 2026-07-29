////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_FOREST_H_INCLUDED
#define RENDER_SPEEDTREE_FOREST_H_INCLUDED

/* INCLUDES */
class SpeedTree::CForest;
class SpeedTree::CGrass;
class SpeedTree::CView;
class SpeedTree::CWind;
struct SpeedTree::SForestCullResults;
struct SpeedTree::SGrassCullResults;
struct vostok::render::speedtree_billboard_parameters;
struct vostok::render::speedtree_common_parameters;
struct vostok::render::speedtree_tree_parameters;
struct vostok::render::speedtree_wind_parameters;
typedef vostok::render::vector<vostok::resources::resource_ptr<vostok::render::speedtree_instance,vostok::resources::unmanaged_intrusive_base> >
	vostok::render::vector<vostok::render::speedtree_instance_ptr >;
typedef vostok::render::vector<vostok::resources::resource_ptr<vostok::render::speedtree_tree_base,vostok::resources::unmanaged_intrusive_base> >
	vostok::render::vector<vostok::render::speedtree_tree_base_ptr >;
class vostok::render::speedtree_instance;
class vostok::render::speedtree_tree_base;

/* FORWARD REFS */
class vostok::render::renderer_context;
class vostok::render::vector<vostok::render::speedtree_forest::tree_render_info>;

namespace vostok {
namespace render {

struct speedtree_forest {
												speedtree_forest					( );
												~speedtree_forest					( );
	inline	SpeedTree::CForest&					get_forest							( ) const { /* no source */ }
			void								populate_forest						( );

			void								add_instance						( speedtree_instance_ptr st_instance_ptr, float4x4 const& transform );
			void								remove_instance						( speedtree_instance_ptr st_instance_ptr );

			void								set_transform						( speedtree_instance_ptr st_instance_ptr, float4x4 const& transform );
			void								set_wind_direction					( float3 const& wind_direction );
			void								set_wind_strength					( float wind_strength );

	inline	speedtree_wind_parameters&			get_speedtree_wind_parameters		( ) { /* no source */ }
	inline	speedtree_billboard_parameters&		get_speedtree_billboard_parameters	( ) { /* no source */ }
	inline	speedtree_tree_parameters&			get_speedtree_tree_parameters		( ) { /* no source */ }
	inline	speedtree_common_parameters&		get_speedtree_common_parameters		( ) { /* no source */ }
			void								get_visible_tree_components			(
													renderer_context*		context,
													float3 const&			lod_reference_point,
													bool					sort_result,
													vector< speedtree_forest::tree_render_info >&	out_tree_render_info_array
												);
	inline	void								get_visible_tree_components			(
													renderer_context*		arg_0,
													vector< speedtree_instance_ptr > const&	arg_1,
													bool					arg_2,
													vector< speedtree_forest::tree_render_info >&	arg_3
												) { /* no source */ }
	inline	u32									get_num_visible_instances			( ) const { /* no source */ }

			void								tick								( renderer_context* context );

	inline	SpeedTree::CView&					get_speedtree_view					( ) { /* no source */ }
			float4x4							get_instance_transform				( SpeedTree::CInstance const& in_instance );

private:
			void								cull_and_compute_lod				( renderer_context* context, float3 const& lod_reference_point, bool sort_result );

	/* 0x0000 */	speedtree_wind_parameters				m_speedtree_wind_parameters;
	/* 0x002c */	speedtree_billboard_parameters			m_speedtree_billboard_parameters;
	/* 0x0048 */	speedtree_tree_parameters				m_speedtree_tree_parameters;
	/* 0x0050 */	speedtree_common_parameters				m_speedtree_common_parameters;
	/* 0x005c */	SpeedTree::SForestCullResults			m_visible_trees;
	/* 0x00c8 */	SpeedTree::SGrassCullResults			m_visuble_grass;
	/* 0x0104 */	SpeedTree::CView						m_view;
	/* 0x03a0 */	SpeedTree::CGrass						m_grass;
	/* 0x10fc */	SpeedTree::CForest*						m_forest;
	/* 0x1100 */	vector< speedtree_tree_base_ptr >		m_trees;
	/* 0x110c */	vector< speedtree_instance_ptr >		m_tree_instances;
	/* 0x1118 */	SpeedTree::CWind						m_wind_leader;
}; // struct speedtree_forest

STATIC_SIZE_ASSERT(speedtree_forest, 0x1CF4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_FOREST_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_PATCH_H_INCLUDED
#define RENDER_GRASS_PATCH_H_INCLUDED

/* INCLUDES */
class vostok::collision::geometry_instance;
class vostok::collision::object;
class vostok::math::aabb;
class vostok::render::vector<vostok::render::grass_instance *>;
struct vostok::collision::space_partitioning_tree;
struct vostok::render::grass_patch::sort_info;
struct vostok::render::grass_template;
class vostok::collision::space_partitioning_tree* const;
class vostok::render::enum_render_stage_type;
typedef vostok::intrusive_ptr<vostok::render::render_target,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::render_target_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
class vostok::render::render_target;
class vostok::render::res_geometry;
class vostok::render::res_texture;
class vostok::render::untyped_buffer;

/* FORWARD REFS */
class vostok::render::grass_world;
class vostok::render::renderer;
class vostok::render::renderer_context;
class vostok::render::res_effect;
class vostok::render::trample_desc;

namespace vostok {
namespace render {

struct grass_patch : public boost::noncopyable {
									grass_patch				(
										collision::space_partitioning_tree* const	in_collision_tree,
										grass_template*		templ,
										float3 const&		in_origin,
										const float			in_size
									);
									~grass_patch			( );

			void					merge_instances			( );
			void					sort_instances			( float3 const& view_position );

			void					init_collision			( );

			u32						get_stream_1_stride		( ) const;
			u32						get_index_count			( ) const;
	inline	math::aabb const&		get_aabb				( ) const { /* no source */ }

			bool					is_occluded				( ) const;

			void					remove_trample			( );

			void					render					(
										grass_world*				in_grass_world,
										renderer_context*			context,
										float3 const&				viewer_position,
										enum_render_stage_type		stage_type,
										const u32					tech_index,
										const float					draw_distance,
										res_effect*					debug_effect,
										const u32					cascade_index
									);

			u32						get_valid_lod_index		( const u32 lod_index ) const;

	inline	void					update_movement_texture	( renderer_context* arg_0 ) { /* no source */ }

			void					try_accumulate_trample	(
										trample_desc&			desc,
										grass_world*			in_grass_world,
										renderer*				in_renderer,
										renderer_context*		in_context
									);

	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	float								m_movement_data[64][64];
	/* 0x4000 */	render_target_ptr					m_movement_rt;
	/* 0x4004 */	res_texture_ptr						m_movement_texture;
	/* 0x4008 */	float3								m_prev_view_pos;
	/* 0x4014 */	math::aabb							m_aabb;
	/* 0x402c */	float3								m_origin;
	/* 0x4038 */	float								m_size;
	/* 0x403c */	u32									m_occlusion_info_index;
	/* 0x4040 */	u32									m_current_lod_index;
	/* 0x4044 */	u32									m_num_avaliable_lods;
	/* 0x4048 */	vector< grass_instance* >			m_instances;
	/* 0x4054 */	res_geometry_ptr					m_geometry[3];
	/* 0x4060 */	untyped_buffer_ptr					m_vb_stream_1[3];
	/* 0x406c */	u16*								m_merged_indices[3];
	/* 0x4078 */	grass_patch::sort_info*				m_sort_info[3];
	/* 0x4084 */	u32									m_num_merged_vertices[3];
	/* 0x4090 */	u32									m_num_merged_indices[3];
	/* 0x409c */	grass_template*						m_template;
	/* 0x40a0 */	collision::space_partitioning_tree* const	m_collision_tree;
	/* 0x40a4 */	collision::geometry_instance*		m_collision_geometry;
	/* 0x40a8 */	collision::object*					m_collision_object;
	/* 0x40ac */	bool								m_visible;
	/* 0x40ad */	bool								m_occluded;
}; // struct grass_patch

STATIC_SIZE_ASSERT(grass_patch, 0x40B0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_PATCH_H_INCLUDED

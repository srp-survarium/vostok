////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_SHADOW_DIRECT_H_INCLUDED
#define RENDER_STAGE_SHADOW_DIRECT_H_INCLUDED

/* INCLUDES */
class vostok::render::cuboid2;
class vostok::render::shader_constant_host;
class vostok::render::stage;
typedef vostok::intrusive_ptr<vostok::render::render_target,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::render_target_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::render_target;
class vostok::render::res_effect;
class vostok::render::res_texture;

/* FORWARD REFS */
class vostok::render::geometry_batch;
class vostok::render::renderer;
class vostok::render::renderer_context;
class vostok::render::vector<vostok::render::render_surface_instance *>;

namespace vostok {
namespace render {

class stage_shadow_direct : public stage {
public:
						stage_shadow_direct			( renderer* in_renderer, renderer_context* context );
	virtual				~stage_shadow_direct		( );

	inline	void		invalidate_shadow			( ) { /* no source */ }

	virtual	void		execute						( ) override;

	virtual	void		execute_disabled			( ) override;
			void		execute_cascade				( u32 cascade_id, u32 cascade_index, u32 shadow_map_size );

			void		draw_debug					( u32 in_cascade_id );

			void		prepare_models				(
							vector< render_surface_instance* >&		m_caster_model,
							float4x4 const&							orig_view_projection,
							const u32								cascade_index,
							const u32								shadow_map_size,
							float3 const&							view_pos
						);
			void		render_models				(
							vector< render_surface_instance* >&		m_caster_model,
							float4x4 const&							orig_view_projection,
							const u32								cascade_index,
							const u32								shadow_map_size,
							float3 const&							real_view_pos,
							const u32								pass_index,
							const u32								refresh_rate
						);
			void		render_dynamic_models		( const u32 cascade_index, const u32 shadow_map_size, float3 const& view_pos );

private:
			float3		compute_aligment			(
							float3 const&		lightXZshift,
							float4x4 const&		light_space_transform,
							float				smap_res,
							float				mult,
							float3&				gran
						);
	inline	float3		compute_aligment1			( float3 const& arg_0, float4x4 const& arg_1, float arg_2 ) { /* no source */ }

	inline	u32			index_to_shadow_size		( u32 arg_0 ) const { /* no source */ }

	inline	void		render_speedtree_instances	( float3 const& arg_0, u32 arg_1 ) { /* no source */ }

	inline	void		pre_batch_render			( geometry_batch const& arg_0 ) { /* no source */ }
	inline	void		post_batch_render			( geometry_batch const& arg_0 ) { /* no source */ }

			bool		is_effects_ready			( ) const;

	/* 0x0000 */	/* stage */
	/* 0x0010 */	shader_constant_host*		m_c_light_direction;
	/* 0x0014 */	shader_constant_host*		m_c_light_position;
	/* 0x0018 */	shader_constant_host*		m_c_light_attenuation_power;
	/* 0x001c */	shader_constant_host*		m_c_start_corner;
	/* 0x0020 */	shader_constant_host*		m_wind_info_parameters;
	/* 0x0024 */	shader_constant_host*		m_shadow_cascade_index;
	/* 0x0028 */	res_effect_ptr				m_effect_shadow_direct;
	/* 0x002c */	bool						m_cascade_texture_index[4];
	/* 0x0030 */	cuboid2						m_frustums[4];
	/* 0x0540 */	float3						m_previous_adjastment[4];
	/* 0x0570 */	float3						m_previous_direction;
	/* 0x057c */	float3						m_previous_position[4];
	/* 0x05ac */	float4x4					m_previous_view_matrix[4];
	/* 0x06ac */	float4x4					m_previous_projection_matrix[4];
	/* 0x07ac */	float4x4					m_previous_real_view_matrix[4];
	/* 0x08ac */	float4x4					m_view_to_shadow[4];
	/* 0x09ac */	float4x4					m_prev_view_to_shadow[4];
	/* 0x0aac */	u32							m_cascade_shadow_map_size;
	/* 0x0ab0 */	render_target_ptr			m_rt_shadow_map;
	/* 0x0ab4 */	res_texture_ptr				m_t_shadow_map;
	/* 0x0ab8 */	bool						m_invalid_shadow;
}; // class stage_shadow_direct

STATIC_SIZE_ASSERT(stage_shadow_direct, 0xABC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_SHADOW_DIRECT_H_INCLUDED

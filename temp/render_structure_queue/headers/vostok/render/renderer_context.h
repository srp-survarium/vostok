////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDERER_CONTEXT_H_INCLUDED
#define RENDER_RENDERER_CONTEXT_H_INCLUDED

/* INCLUDES */
class vostok::fixed_vector<vostok::math::float4x4,16>;
class vostok::fixed_vector<vostok::render::sun_cascade,4>;
class vostok::math::uint2;
class vostok::render::renderer_context_targets;
class vostok::render::scene;
class vostok::render::shader_constant_host;
class vostok::render::vector<vostok::render::speedtree_forest::tree_render_info>;
struct vostok::render::render_target_instance;
class vostok::render::enum_render_target_index;
class vostok::render::render_target;
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
typedef vostok::resources::resource_ptr<vostok::render::base_scene_view,vostok::resources::unmanaged_intrusive_base>
	vostok::render::base_scene_view_ptr;
class vostok::render::base_scene_view;
class vostok::render::res_geometry;
class vostok::render::res_texture;
class vostok::render::untyped_buffer;

/* FORWARD REFS */
class vostok::render::scene_view;

namespace vostok {
namespace render {

class renderer_context {
public:
									renderer_context					( );

			void					set_scene							( scene* scene );

	inline	scene*					scene								( ) { /* no source */ }

			void					set_scene_view						( base_scene_view_ptr view_ptr );
			scene_view const*		scene_view							( ) const;

	inline	void					create_casceded_shadow_map_buffers	( u32 arg_0 ) { /* no source */ }

			void					set_target_context					( renderer_context_targets const* targets_context, bool force_set );

			void					clear_resources						( );

			void					set_w								( float4x4 const& m );
			void					set_v								( float4x4 const& m );
			void					set_p								( float4x4 const& m );
	inline	void					set_w_identity						( ) { /* no source */ }

	inline	void					push_set_w							( float4x4 const& arg_0 ) { /* no source */ }
	inline	void					push_set_v							( float4x4 const& arg_0 ) { /* no source */ }
	inline	void					push_set_p							( float4x4 const& arg_0 ) { /* no source */ }

	inline	void					pop_w								( ) { /* no source */ }
	inline	void					pop_v								( ) { /* no source */ }
	inline	void					pop_p								( ) { /* no source */ }

	inline	float4x4 const&			get_w								( ) const { /* no source */ }
	inline	float4x4 const&			get_v								( ) const { /* no source */ }
	inline	float4x4 const&			get_v_inverted						( ) const { /* no source */ }
	inline	float4x4 const&			get_culling_v						( ) const { /* no source */ }
	inline	float4x4 const&			get_culling_v_inverted				( ) const { /* no source */ }
	inline	float4x4 const&			get_p								( ) const { /* no source */ }
	inline	float4x4 const&			get_p_inverted						( ) const { /* no source */ }
	inline	float					get_near							( ) const { /* no source */ }
	inline	float					get_far								( ) const { /* no source */ }
	inline	float3 const*			get_eye_rays						( ) { /* no source */ }
	inline	float4 const&			get_near_far						( ) { /* no source */ }
	inline	float4x4 const&			get_wv								( ) const { /* no source */ }
	inline	float4x4 const&			get_vp								( ) const { /* no source */ }
	inline	float4x4 const&			get_culling_vp						( ) const { /* no source */ }
	inline	float4x4 const&			get_wvp								( ) const { /* no source */ }

			void					set_view2shadow						( float4x4 const& view2shadow, u32 index );
			float4x4 const&			get_view2shadow						( u32 index ) const;
	inline	float3 const&			get_view_pos						( ) const { /* no source */ }
	inline	float3 const&			get_view_dir						( ) const { /* no source */ }
	inline	float3 const&			get_culling_view_pos				( ) const { /* no source */ }
	inline	float3 const&			get_culling_view_dir				( ) const { /* no source */ }

	inline	void					set_solid_diffusecolor_specularintensity( float3 arg_0, float arg_1 ) { /* no source */ }
	inline	void					set_solid_material_parameters		(
										float		arg_0,
										float		arg_1,
										float		arg_2,
										float		arg_3
									) { /* no source */ }
	inline	void					set_solid_emission_color			( float3 arg_0 ) { /* no source */ }

			void					reset_matrices						( );

			void					update_eye_rays						( );

	inline	u32						get_lighting_material_strtegy		( ) { /* no source */ }
	inline	float					get_shadow_map_z_bias				( ) { /* no source */ }

			void					set_time_delta						( float time_delta );
			void					set_current_time					( float current_time );

	inline	float					get_time_delta						( ) const { /* no source */ }
	inline	float					get_current_time					( ) const { /* no source */ }
	inline	float4					get_screen_resolution				( ) const { /* no source */ }
	inline	render_target_ptr		get_rt								( enum_render_target_index index ) { /* no source */ }
	inline	res_texture_ptr			get_t								( enum_render_target_index index ) { /* no source */ }
			scene_view*				get_scene_view						( );

private:
			void					update_near_far						( );

public:
	inline							~renderer_context					( ) { /* no source */ }

	/* 0x0000 */	renderer_context_targets const*		m_targets;
	/* 0x0004 */	render_target_instance				m_family[70];
	/* 0x2bc4 */	math::uint2							m_current_size;
	/* 0x2bcc */	res_texture_ptr						m_t_null;
	/* 0x2bd0 */	res_geometry_ptr					m_g_quad_uv;
	/* 0x2bd4 */	res_geometry_ptr					m_g_quad_2uv;
	/* 0x2bd8 */	res_geometry_ptr					m_g_quad_eye_ray;
	/* 0x2bdc */	untyped_buffer_ptr					m_quad_ib;
	/* 0x2be0 */	u32									m_light_marker_id;
	/* 0x2be4 */	res_texture_ptr						m_t_shadow_cascade;
	/* 0x2be8 */	float								m_time_delta;
	/* 0x2bec */	float								m_current_time;
	/* 0x2bf0 */	fixed_vector< sun_cascade, 4 >		m_sun_cascades;
	/* 0x3048 */	vector< speedtree_forest::tree_render_info >	m_visible_trees;
private:
	/* 0x3054 */	float4								m_near_far_invn_invf;
	/* 0x3064 */	scene*								m_scene;
	/* 0x3068 */	base_scene_view_ptr					m_scene_view;
	/* 0x306c */	fixed_vector< float4x4, 16 >		m_w_stack;
	/* 0x3474 */	fixed_vector< float4x4, 16 >		m_v_stack;
	/* 0x387c */	fixed_vector< float4x4, 16 >		m_p_stack;
	/* 0x3c84 */	float4x4							m_w;
	/* 0x3cc4 */	float4x4							m_w_transposed;
	/* 0x3d04 */	float4x4							m_v;
	/* 0x3d44 */	float4x4							m_v_transposed;
	/* 0x3d84 */	float4x4							m_v_inverted;
	/* 0x3dc4 */	float4x4							m_v_inverted_transposed;
	/* 0x3e04 */	float4x4							m_wv_inverted_transposed;
	/* 0x3e44 */	float4x4							m_p;
	/* 0x3e84 */	float4x4							m_p_transposed;
	/* 0x3ec4 */	float4x4							m_p_inverted;
	/* 0x3f04 */	float4x4							m_wv;
	/* 0x3f44 */	float4x4							m_wv_transposed;
	/* 0x3f84 */	float4x4							m_vp;
	/* 0x3fc4 */	float4x4							m_vp_transposed;
	/* 0x4004 */	float4x4							m_wvp;
	/* 0x4044 */	float4x4							m_wvp_transposed;
	/* 0x4084 */	float4x4							m_v2shadow0;
	/* 0x40c4 */	float4x4							m_v2shadow1;
	/* 0x4104 */	float4x4							m_v2shadow2;
	/* 0x4144 */	float4x4							m_v2shadow3;
	/* 0x4184 */	float3								m_eye_rays[4];
	/* 0x41b4 */	float4								m_fog_params;
	/* 0x41c4 */	float4								m_screen_resolution;
	/* 0x41d4 */	float4								m_solid_color_specular;
	/* 0x41e4 */	float4								m_solid_material_parameters;
	/* 0x41f4 */	float4								m_solid_emission_color;
	/* 0x4204 */	float4								m_view_pos;
	/* 0x4214 */	float4								m_eye_pos_view_space;
	/* 0x4224 */	float4								m_view_dir;
	/* 0x4234 */	shader_constant_host const*			m_c_w;
	/* 0x4238 */	shader_constant_host const*			m_c_w_inv;
	/* 0x423c */	shader_constant_host const*			m_c_v;
	/* 0x4240 */	shader_constant_host const*			m_c_p;
	/* 0x4244 */	shader_constant_host const*			m_c_wv;
	/* 0x4248 */	shader_constant_host const*			m_c_vp;
	/* 0x424c */	shader_constant_host const*			m_c_wvp;
	/* 0x4250 */	shader_constant_host const*			m_c_v2w;
	/* 0x4254 */	shader_constant_host const*			m_c_wv_inv;
	/* 0x4258 */	shader_constant_host const*			m_c_near_far;
	/* 0x425c */	shader_constant_host const*			m_c_fog_params;
	/* 0x4260 */	shader_constant_host const*			m_c_screen_resolution;
	/* 0x4264 */	shader_constant_host const*			m_c_view_pos;
	/* 0x4268 */	shader_constant_host const*			m_c_eye_pos_view_space;
	/* 0x426c */	shader_constant_host const*			m_c_view_dir;
	/* 0x4270 */	shader_constant_host const*			m_c_solid_color_specular;
	/* 0x4274 */	shader_constant_host const*			m_c_solid_material_parameters;
	/* 0x4278 */	shader_constant_host const*			m_c_solid_emission_color;
	/* 0x427c */	shader_constant_host const*			m_c_scene_time;
	/* 0x4280 */	u32									m_frame_index;
}; // class renderer_context

STATIC_SIZE_ASSERT(renderer_context, 0x4284);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDERER_CONTEXT_H_INCLUDED

#ifndef VOSTOK_RENDER_ENGINE_RENDERER_CONTEXT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDERER_CONTEXT_H_INCLUDED

#include <vostok/fixed_vector.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4.h>
#include <vostok/math_float4x4.h>
#include <vostok/math_uint2.h>
#include <vostok/render/core/untyped_buffer.h>
#include <vostok/render/engine/base_classes.h>

#include "renderer_context_targets.h"
#include "res_geometry.h"
#include "speedtree_forest.h"
#include "sun_cascade.h"

namespace vostok {
namespace render {

class scene;
class scene_view;
class shader_constant_host;

class renderer_context {
public:
	renderer_context( );
	~renderer_context( ) { }

	void set_scene( render::scene* in_scene );
	render::scene* scene( ) { return m_scene; }

	scene_view const* scene_view( ) const;
	void set_scene_view( base_scene_view_ptr view_ptr );
	render::scene_view* get_scene_view( );

	void create_casceded_shadow_map_buffers( u32 ) { }

	void set_view2shadow( float4x4 const& view2shadow, u32 index );
	float4x4 const& get_view2shadow( u32 index ) const;

	void set_target_context( renderer_context_targets const* targets_context, bool force_set );
	void set_time_delta( float time_delta );
	void set_current_time( float current_time );
	void set_w( float4x4 const& matrix );
	void set_v( float4x4 const& matrix );
	void set_p( float4x4 const& matrix );
	void set_w_identity( ) { set_w( float4x4( ).identity( ) ); }

	void push_set_w( float4x4 const& matrix ) { m_w_stack.push_back( m_w ); set_w( matrix ); }
	void push_set_v( float4x4 const& matrix ) { m_v_stack.push_back( m_v ); set_v( matrix ); }
	void push_set_p( float4x4 const& matrix ) { m_p_stack.push_back( m_p ); set_p( matrix ); }

	void pop_w( ) { set_w( m_w_stack.back( ) ); m_w_stack.pop_back( ); }
	void pop_v( ) { set_v( m_v_stack.back( ) ); m_v_stack.pop_back( ); }
	void pop_p( ) { set_p( m_p_stack.back( ) ); m_p_stack.pop_back( ); }

	float4x4 const& get_w( ) const { return m_w; }
	float4x4 const& get_v( ) const { return m_v; }
	float4x4 const& get_v_inverted( ) const { return m_v_inverted; }
	float4x4 const& get_culling_v( ) const { return m_v; }
	float4x4 const& get_culling_v_inverted( ) const { return m_v_inverted; }
	float4x4 const& get_p( ) const { return m_p; }
	float4x4 const& get_p_inverted( ) const { return m_p_inverted; }

	float get_near( ) const { return m_near_far_invn_invf.x; }
	float get_far( ) const { return m_near_far_invn_invf.y; }
	float3 const* get_eye_rays( ) { return m_eye_rays; }
	float4 const& get_near_far( ) { return m_near_far_invn_invf; }

	float4x4 const& get_wv( ) const { return m_wv; }
	float4x4 const& get_vp( ) const { return m_vp; }
	float4x4 const& get_culling_vp( ) const { return m_vp; }
	float4x4 const& get_wvp( ) const { return m_wvp; }

	float3 const& get_view_pos( ) const { return m_view_pos.xyz( ); }
	float3 const& get_view_dir( ) const { return m_view_dir.xyz( ); }
	float3 const& get_culling_view_pos( ) const { return m_view_pos.xyz( ); }
	float3 const& get_culling_view_dir( ) const { return m_view_dir.xyz( ); }

	void set_solid_diffusecolor_specularintensity( float3 color, float specular_intensity )
	{
		m_solid_color_specular.set( color.x, color.y, color.z, specular_intensity );
	}

	void set_solid_material_parameters(
		float specular_power,
		float diffuse_power,
		float translucency,
		float material_id
	)
	{
		m_solid_material_parameters.set(
			specular_power,
			diffuse_power,
			translucency,
			material_id
		);
	}

	void set_solid_emission_color( float3 color )
	{
		m_solid_emission_color.x = color.x;
		m_solid_emission_color.y = color.y;
		m_solid_emission_color.z = color.z;
	}

	void reset_matrices( );
	void update_eye_rays( );
	void clear_resources( );

	u32 get_lighting_material_strtegy( ) { return 0; }
	float get_shadow_map_z_bias( ) { return 0.0f; }

	float get_time_delta( ) const { return m_time_delta; }
	float get_current_time( ) const { return m_current_time; }
	float4 get_screen_resolution( ) const { return m_screen_resolution; }

	render_target_ptr get_rt( enum_render_target_index index )
	{
		// FUNCTION BODY[0x64550]
		return m_targets->m_family[index].target;
	}

	res_texture_ptr get_t( enum_render_target_index index )
	{
		// FUNCTION BODY[0x64580]
		return m_family[index].texture;
	}

private:
	void update_near_far( );

public:
	renderer_context_targets const*			m_targets;
	render_target_instance				m_family[rt_num_render_targets];
	math::uint2						m_current_size;
	res_texture_ptr						m_t_null;
	res_geometry_ptr						m_g_quad_uv;
	res_geometry_ptr						m_g_quad_2uv;
	res_geometry_ptr						m_g_quad_eye_ray;
	untyped_buffer_ptr					m_quad_ib;
	u32								m_light_marker_id;
	res_texture_ptr						m_t_shadow_cascade;
	float								m_time_delta;
	float								m_current_time;
	fixed_vector< sun_cascade, 4 >		m_sun_cascades;
	speedtree_forest::tree_render_info_array_type m_visible_trees;

private:
	float4							m_near_far_invn_invf;
	render::scene*					m_scene;
	base_scene_view_ptr				m_scene_view;
	fixed_vector< float4x4, 16 >		m_w_stack;
	fixed_vector< float4x4, 16 >		m_v_stack;
	fixed_vector< float4x4, 16 >		m_p_stack;
	float4x4						m_w;
	float4x4						m_w_transposed;
	float4x4						m_v;
	float4x4						m_v_transposed;
	float4x4						m_v_inverted;
	float4x4						m_v_inverted_transposed;
	float4x4						m_wv_inverted_transposed;
	float4x4						m_p;
	float4x4						m_p_transposed;
	float4x4						m_p_inverted;
	float4x4						m_wv;
	float4x4						m_wv_transposed;
	float4x4						m_vp;
	float4x4						m_vp_transposed;
	float4x4						m_wvp;
	float4x4						m_wvp_transposed;
	float4x4						m_v2shadow0;
	float4x4						m_v2shadow1;
	float4x4						m_v2shadow2;
	float4x4						m_v2shadow3;
	float3							m_eye_rays[4];
	float4							m_fog_params;
	float4							m_screen_resolution;
	float4							m_solid_color_specular;
	float4							m_solid_material_parameters;
	float4							m_solid_emission_color;
	float4							m_view_pos;
	float4							m_eye_pos_view_space;
	float4							m_view_dir;
	shader_constant_host const*			m_c_w;
	shader_constant_host const*			m_c_w_inv;
	shader_constant_host const*			m_c_v;
	shader_constant_host const*			m_c_p;
	shader_constant_host const*			m_c_wv;
	shader_constant_host const*			m_c_vp;
	shader_constant_host const*			m_c_wvp;
	shader_constant_host const*			m_c_v2w;
	shader_constant_host const*			m_c_wv_inv;
	shader_constant_host const*			m_c_near_far;
	shader_constant_host const*			m_c_fog_params;
	shader_constant_host const*			m_c_screen_resolution;
	shader_constant_host const*			m_c_view_pos;
	shader_constant_host const*			m_c_eye_pos_view_space;
	shader_constant_host const*			m_c_view_dir;
	shader_constant_host const*			m_c_solid_color_specular;
	shader_constant_host const*			m_c_solid_material_parameters;
	shader_constant_host const*			m_c_solid_emission_color;
	shader_constant_host const*			m_c_scene_time;
	u32								m_frame_index;
};

STATIC_SIZE_ASSERT( renderer_context, 0x4284 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDERER_CONTEXT_H_INCLUDED

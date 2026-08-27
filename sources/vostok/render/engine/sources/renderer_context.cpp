#include "pch.h"
#include "renderer_context.h"

#include <vostok/console_command.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/res_texture.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/shader_constant_binding.h>

#include "renderer_register_effects.h"
#include "scene_view.h"

static u32 s_lighting_material_strategy = 0;
static vostok::console_commands::cc_u32 s_cc_lighting_material_strategy(
	"lighting_material_strategy",
	s_lighting_material_strategy,
	0,
	4,
	true,
	vostok::console_commands::command_type_engine_internal
);

static float s_shadow_map_z_bias = 0.0001f;
static vostok::console_commands::cc_float s_cc_shadow_map_z_bias(
	"shadow_map_z_bias",
	s_shadow_map_z_bias,
	0,
	4,
	true,
	vostok::console_commands::command_type_engine_internal
);

namespace vostok {
namespace render {

renderer_context::renderer_context( ) :
	m_targets				( NULL ),
	m_t_null				( NULL ),
	m_g_quad_uv				( NULL ),
	m_g_quad_2uv			( NULL ),
	m_g_quad_eye_ray		( NULL ),
	m_quad_ib				( NULL ),
	m_t_shadow_cascade		( NULL ),
	m_time_delta			( 0.f ),
	m_scene					( NULL ),
	m_scene_view			( NULL ),
	m_view_pos				( 0.f, 0.f, 0.f, 1.f ),
	m_eye_pos_view_space	( 0.f, 0.f, 0.f, 1.f ),
	m_view_dir				( 0.f, 0.f, 1.f, 0.1f ),
	m_w						( float4x4( ).identity( ) ),
	m_w_transposed			( float4x4( ).identity( ) ),
	m_v						( float4x4( ).identity( ) ),
	m_v_transposed			( float4x4( ).identity( ) ),
	m_v_inverted			( float4x4( ).identity( ) ),
	m_v_inverted_transposed	( float4x4( ).identity( ) ),
	m_p						( float4x4( ).identity( ) ),
	m_p_transposed			( float4x4( ).identity( ) ),
	m_p_inverted			( float4x4( ).identity( ) ),
	m_wv					( float4x4( ).identity( ) ),
	m_wv_transposed			( float4x4( ).identity( ) ),
	m_vp					( float4x4( ).identity( ) ),
	m_vp_transposed			( float4x4( ).identity( ) ),
	m_wvp					( float4x4( ).identity( ) ),
	m_wvp_transposed		( float4x4( ).identity( ) ),
	m_v2shadow0				( float4x4( ).identity( ) ),
	m_v2shadow1				( float4x4( ).identity( ) ),
	m_v2shadow2				( float4x4( ).identity( ) ),
	m_v2shadow3				( float4x4( ).identity( ) ),
	m_c_w					( NULL ),
	m_c_w_inv				( NULL ),
	m_c_v					( NULL ),
	m_c_p					( NULL ),
	m_c_wv					( NULL ),
	m_c_vp					( NULL ),
	m_c_wvp					( NULL ),
	m_c_v2w					( NULL ),
	m_solid_color_specular		( 0.f, 0.f, 0.f, 0.f ),
	m_solid_material_parameters	( 10.f, 1.f, 0.f, 0.f )
{
	for ( u32 i = 0; i < rt_num_render_targets; ++i )
	{
		m_family[i].target	= NULL;
		m_family[i].texture	= NULL;
	}

	m_current_size			= math::uint2( 0, 0 );
	m_t_shadow_cascade		= NULL;

	memset					( &m_eye_rays, 0, sizeof( m_eye_rays ) );
	register_effect_descriptors( );

	sun_cascade cascades[sun_cascade::num_max_sun_shadow_cascades];

	cascades[0].size		= 10.0f;
	cascades[0].bias		= 0.0001f;

	cascades[1].size		= 30.0f;
	cascades[1].bias		= 0.0005f;

	cascades[2].size		= 75.0f;
	cascades[2].bias		= 0.001f;

	cascades[3].size		= 240.0f;
	cascades[3].bias		= 0.003f;

	for ( u32 cascade_index = 0; cascade_index < sun_cascade::num_max_sun_shadow_cascades; cascade_index++ )
		m_sun_cascades.push_back( cascades[cascade_index] );

	memset					( &m_eye_rays, 0, sizeof( m_eye_rays ) );
	float n					= 6000.f;
	float f					= 10000.f;
	float r					= 1.f / ( f - n );
	m_fog_params.set		( -n * r, r, r, r );
	m_screen_resolution		= float4( 1.f, 1.f, 1.f, 1.f );

	reset_matrices			( );
	m_c_w					= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_W", &m_w_transposed ) );
	m_c_v					= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_V", &m_v_transposed ) );
	m_c_p					= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_P", &m_p_transposed ) );
	m_c_v2w					= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_V2W", &m_v_inverted_transposed ) );
	m_c_wv_inv				= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_WV_inverted", &m_wv_inverted_transposed ) );
	m_c_wv					= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_WV", &m_wv_transposed ) );
	m_c_vp					= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_VP", &m_vp_transposed ) );
	m_c_wvp					= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_WVP", &m_wvp_transposed ) );
	m_c_near_far			= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "near_far_invn_invf", &m_near_far_invn_invf ) );

	m_c_fog_params			= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "fog_params", &m_fog_params ) );
	m_c_screen_resolution	= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "screen_res", &m_screen_resolution ) );
	m_c_view_pos			= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "eye_position", &m_view_pos ) );
	m_c_eye_pos_view_space	= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "eye_position_view_space", &m_eye_pos_view_space ) );
	m_c_view_dir			= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "eye_direction", &m_view_dir ) );

	m_c_solid_color_specular		= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "solid_color_specular", &m_solid_color_specular ) );
	m_c_solid_material_parameters	= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "solid_material_params", &m_solid_material_parameters ) );
	m_c_solid_emission_color		= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "solid_emission_color", &m_solid_emission_color ) );

	m_c_scene_time			= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "scene_time", &m_current_time ) );

	for ( u32 i = 0; i < rt_num_render_targets; ++i )
	{
		pcstr const name	= rt_index_to_name( enum_render_target_index( i ) );
		if ( name ) m_family[i].texture = resource_manager::ref( ).create_texture( name, 0, 0, false, true, true, 0 );
	}

	m_t_null				= resource_manager::ref( ).create_texture( "$user$null", 0, 0, false, true, true, 0 );
}

void renderer_context::set_scene( render::scene* in_scene )
{

	m_scene					= in_scene;
}

scene_view const* renderer_context::scene_view( ) const
{

	return static_cast_checked< vostok::render::scene_view const* >( m_scene_view.c_ptr( ) );
}

scene_view* renderer_context::get_scene_view( )
{

	return static_cast_checked< vostok::render::scene_view* >( m_scene_view.c_ptr( ) );
}

void renderer_context::set_scene_view( base_scene_view_ptr view_ptr )
{

	m_scene_view			= view_ptr;

	vostok::render::scene_view* view = static_cast_checked< vostok::render::scene_view* >( view_ptr.c_ptr( ) );

	set_v					( view->camera( ).get_view_transform( ) );
	set_p					( view->camera( ).get_projection_transform( ) );
}

void renderer_context::set_view2shadow( float4x4 const& view2shadow, u32 index )
{
	// STATICS
	// static < NoType > 				 = <0x64270c>;

	switch ( index )
	{
		case 0:
			m_v2shadow0		= view2shadow;
			break;
		case 1:
			m_v2shadow1		= view2shadow;
			break;
		case 2:
			m_v2shadow2		= view2shadow;
			break;
		case 3:
			m_v2shadow3		= view2shadow;
			break;
	}
}

float4x4 const& renderer_context::get_view2shadow( u32 index ) const
{
	// STATICS
	// static < NoType > 				 = <0x642694>;

	switch ( index )
	{
		case 0:
			return m_v2shadow0;
		case 1:
			return m_v2shadow1;
		case 2:
			return m_v2shadow2;
		case 3:
			return m_v2shadow3;
	}
	return m_v2shadow0;
}

// claude@NOTE: LTCG selects different `this` handling across this call boundary.
void renderer_context::set_target_context( renderer_context_targets const* targets_context, bool force_set )
{
	if (
		!force_set &&
		m_targets == targets_context &&
		m_current_size == targets_context->size( )
	)
		return;

	m_targets				= targets_context;
	m_current_size			= targets_context->size( );

	if ( !targets_context )
	{
		for ( u32 i = 0; i < rt_num_render_targets; ++i )
			m_family[i].texture->clone( &*m_t_null );
		return;
	}

	m_screen_resolution		= float4(
		float( m_current_size.x ),
		float( m_current_size.y ),
		1.f / float( m_current_size.x ),
		1.f / float( m_current_size.y )
	);

	for ( u32 i = 0; i < rt_num_render_targets; ++i )
	{
		res_texture_ptr const& source = targets_context->m_family[i].texture;
		if ( source )
			m_family[i].texture->clone( &*source );
	}
}

void renderer_context::set_time_delta( const float time_delta )
{

	m_time_delta			= time_delta;
}

void renderer_context::set_current_time( const float current_time )
{

	m_current_time			= current_time;
}

void renderer_context::set_w( float4x4 const& m )
{

	m_w						= m;
	m_wv					= m_w * m_v;
	m_wvp					= mul4x4( m_wv, m_p );

	m_wv_inverted_transposed = m_wv;
	m_wv_inverted_transposed.try_invert( m_wv_inverted_transposed );
	m_wv_inverted_transposed = transpose( m_wv_inverted_transposed );

	m_w_transposed			= transpose( m_w );
	m_wv_transposed			= transpose( m_wv );
	m_wvp_transposed		= transpose( m_wvp );

	backend::ref( ).set_vs_constant( m_c_w, m_w_transposed );
	backend::ref( ).set_vs_constant( m_c_wv_inv, m_wv_inverted_transposed );
	backend::ref( ).set_vs_constant( m_c_wv, m_wv_transposed );
	backend::ref( ).set_vs_constant( m_c_wvp, m_wvp_transposed );

	backend::ref( ).set_ps_constant( m_c_w, m_w_transposed );
	backend::ref( ).set_ps_constant( m_c_wv, m_wv_transposed );
	backend::ref( ).set_ps_constant( m_c_wvp, m_wvp_transposed );
}

void renderer_context::set_v( float4x4 const& m )
{

	m_v_transposed			= transpose( m_v = m );

	m_v_inverted.try_invert	( m );
	m_v_inverted_transposed	= transpose( m_v_inverted );

	m_wv_transposed			= transpose( m_wv = m_w * m_v );
	m_vp_transposed			= transpose( m_vp = mul4x4( m_v, m_p ) );
	m_wvp_transposed		= transpose( m_wvp = mul4x4( m_wv, m_p ) );

	m_view_pos				= float4( m_v_inverted.c.xyz( ), 1 );
	m_view_dir				= float4( m_v_inverted.k.xyz( ), 0 );
	m_eye_pos_view_space	= float4( m_v.transform_position( m_view_pos.xyz( ) ), 1.f );

	backend::ref( ).set_ps_constant( m_c_view_pos, m_view_pos );
	backend::ref( ).set_vs_constant( m_c_view_pos, m_view_pos );
	backend::ref( ).set_ps_constant( m_c_eye_pos_view_space, m_eye_pos_view_space );
	backend::ref( ).set_vs_constant( m_c_eye_pos_view_space, m_eye_pos_view_space );
	backend::ref( ).set_ps_constant( m_c_view_dir, m_view_dir );
	backend::ref( ).set_vs_constant( m_c_view_dir, m_view_dir );

	backend::ref( ).set_vs_constant( m_c_v, m_v_transposed );
	backend::ref( ).set_vs_constant( m_c_vp, m_vp_transposed );
	backend::ref( ).set_vs_constant( m_c_wv, m_wv_transposed );
	backend::ref( ).set_vs_constant( m_c_wvp, m_wvp_transposed );

	backend::ref( ).set_ps_constant( m_c_v, m_v_transposed );
	backend::ref( ).set_ps_constant( m_c_vp, m_vp_transposed );
	backend::ref( ).set_ps_constant( m_c_wv, m_wv_transposed );
	backend::ref( ).set_ps_constant( m_c_wvp, m_wvp_transposed );
}

void renderer_context::set_p( float4x4 const& m )
{

	m_p						= m;
	m_p_transposed			= transpose( m_p = m );
	m_p						= m;

	m_p_inverted			= math::invert4x4( m );

	m_vp_transposed			= transpose( m_vp = mul4x4( m_v, m_p ) );

	m_wvp_transposed		= transpose( m_wvp = mul4x4( m_wv, m_p ) );

	update_near_far			( );
	update_eye_rays			( );

	backend::ref( ).set_vs_constant( m_c_p, m_p_transposed );
	backend::ref( ).set_vs_constant( m_c_vp, m_vp_transposed );
	backend::ref( ).set_vs_constant( m_c_wvp, m_wvp_transposed );

	backend::ref( ).set_ps_constant( m_c_p, m_p_transposed );
	backend::ref( ).set_ps_constant( m_c_vp, m_vp_transposed );
	backend::ref( ).set_ps_constant( m_c_wvp, m_wvp_transposed );
}

void renderer_context::update_near_far( )
{

	float4 tmp_transformed	= m_p_inverted.transform( float4( 0.f, 0.f, 0.f, 1.f ) );
	m_near_far_invn_invf.x	= tmp_transformed.z / tmp_transformed.w;

	tmp_transformed			= m_p_inverted.transform( float4( 0.f, 0.f, 1.f, 1.f ) );
	m_near_far_invn_invf.y	= tmp_transformed.z / tmp_transformed.w;

	m_near_far_invn_invf.z	= 1.f / m_near_far_invn_invf.x;
	m_near_far_invn_invf.w	= 1.f / m_near_far_invn_invf.y;
}

void renderer_context::reset_matrices( )
{

	m_w.identity			( );
	m_w_transposed.identity	( );
	m_v.identity			( );
	m_v_transposed.identity	( );
	m_v_inverted.identity	( );
	m_v_inverted_transposed.identity( );
	m_p.identity			( );
	m_p_transposed.identity	( );
	m_p_inverted.identity	( );
	m_wv.identity			( );
	m_wv_transposed.identity( );
	m_vp.identity			( );
	m_vp_transposed.identity( );
	m_wvp.identity			( );
	m_wvp_transposed.identity( );

	m_near_far_invn_invf	= float4( 0.f, 0.f, 0.f, 0.f );
}

u32 renderer_context::get_lighting_material_strtegy( )
{
	return s_lighting_material_strategy;
}

float renderer_context::get_shadow_map_z_bias( )
{
	return s_shadow_map_z_bias;
}

void renderer_context::update_eye_rays( )
{

	float4x4 const& inv_proj = m_p_inverted;

	float4 eye_00			( -1.f, 1.f, 1.f, 1.f );

	eye_00					= inv_proj.transform( eye_00 );

	float inv_w				= 1.f / eye_00.w;
	eye_00.x				*= inv_w;
	eye_00.y				*= inv_w;
	eye_00.z				*= inv_w;
	m_eye_rays[0]			= float3( eye_00.x, eye_00.y, eye_00.z );
	m_eye_rays[1]			= float3( eye_00.x, -eye_00.y, eye_00.z );
	m_eye_rays[2]			= float3( -eye_00.x, eye_00.y, eye_00.z );
	m_eye_rays[3]			= float3( -eye_00.x, -eye_00.y, eye_00.z );
}

void renderer_context::clear_resources( )
{

	m_scene_view			= NULL;
}

	// TYPEDEFS
	// typedef
	// 	pbyte
	// 	iterator_type;

	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_pass_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_shader_technique_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_buffer_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// typedef
	// 	vostok::render::speedtree_forest::tree_render_info*
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

} // namespace render
} // namespace vostok

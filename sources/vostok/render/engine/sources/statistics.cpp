#include "pch.h"
#include "statistics.h"

namespace vostok {
namespace render {

namespace ui {
struct vertex;
}

statistics_group::statistics_group( pcstr group_name ) :
	first_statistics( 0 ),
	m_name( group_name ),
	m_next( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x638c60]
}

statistics_group::~statistics_group( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6387d0]
}

void statistics_group::start( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6387a0]
}

static void push_point(
	vectora< ui::vertex >&,
	u32,
	float,
	float,
	float,
	u32,
	float,
	float
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x638870]
}

static void make_ui_vertices(
	vectora< ui::vertex >&,
	pcstr,
	vostok::ui::font const&,
	float2 const&,
	math::color const&,
	math::color const&,
	u32,
	bool,
	u32,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x638910]
}

statistics_base::statistics_base( statistics_group* group, pcstr name ) :
	m_name( name ? name : "" ),
	m_next( 0 ),
	m_group( group )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6388c0]
}

statistics_base::~statistics_base( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6387c0]
}

void statistics_float::print( fs_new::virtual_path_string& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x638d20]
}

u32 get_num_digits( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x638830]
	return 0;
}

fixed_string< 260 > u32_to_string( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x638cf0]
	return fixed_string< 260 >( );
}

void statistics_int::print( fs_new::virtual_path_string& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x639520]
}

void statistics_cpu_gpu::print( fs_new::virtual_path_string& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x638ca0]
}

void statistics_cpu_gpu::start( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x638770]
}

gbuffer_statistics_group::gbuffer_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	pre_pass_execute_time( this, "pre pass execute time" ),
	material_pass_execute_time( this, "material pass execute time" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63a4b0]
}

visibility_statistics_group::visibility_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	portal_culling_time( this, "portal culling time" ),
	frustums_count( this, "frustums count" ),
	culling_time( this, "culling time" ),
	models_updating_time( this, "models updating time" ),
	num_draw_calls( this, "num draw calls" ),
	num_triangles( this, "num triangles" ),
	num_total_rendered_triangles( this, "num total rendered triangles" ),
	num_total_rendered_points( this, "num total rendered points" ),
	num_surfaces( this, "num surfaces" ),
	num_lights( this, "num lights" ),
	num_particle_instances( this, "num particle instances" ),
	num_speedtree_instances( this, "num speedtree instances" ),
	num_environment_probes( this, "num environment probes" ),
	num_ambient_volumes( this, "num ambient volumes" ),
	num_occlusion_culled_surfaces( this, "num occlusion culled surfaces" ),
	num_occlusion_culled_lights( this, "num occlusion culled lights" ),
	num_occlusion_culled_grass_patches( this, "num occlusion culled grass patches" ),
	num_occlusion_culled_particle_instances( this, "num occlusion culled particle instances" ),
	num_occlusion_culled_decals( this, "num occlusion culled decals" ),
	num_occlusion_culled_env_probes( this, "num occlusion culled env probes" ),
	num_occlusion_culled_portals( this, "num occlusion culled portals" ),
	num_occlusion_culled_ambient_volumes( this, "num occlusion culled ambient volumes" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6392d0]
}

lights_statistics_group::lights_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	accumulate_lighting_time( this, "accumulate lighting time" ),
	forward_lighting_time( this, "forward lighting time" ),
	shadow_map_time( this, "shadow map time" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63a390]
}

particles_statistics_group::particles_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	execute_time( this, "execute time" ),
	sprites_execute_time( this, "sprites execute time" ),
	beamtrails_execute_time( this, "beamtrails execute time" ),
	meshes_execute_time( this, "meshes execute time" ),
	num_total_instances( this, "num total instances" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63a210]
}

cascaded_sun_shadow_statistics_group::cascaded_sun_shadow_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	execute_time_cascade_1( this, "execute time cascade 1" ),
	execute_time_cascade_2( this, "execute time cascade 2" ),
	execute_time_cascade_3( this, "execute time cascade 3" ),
	execute_time_cascade_4( this, "execute time cascade 4" ),
	num_dips_cascade_1( this, "num dips cascade 1" ),
	num_dips_cascade_2( this, "num dips cascade 2" ),
	num_dips_cascade_3( this, "num dips cascade 3" ),
	num_dips_cascade_4( this, "num dips cascade 4" ),
	num_dips( this, "num dips" ),
	num_clipped_dips( this, "num clipped dips" ),
	num_triangles( this, "num triangles" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63a000]
}

postprocess_statistics_group::postprocess_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	execute_time( this, "execute time" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x639f70]
}

forward_stage_statistics_group::forward_stage_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	execute_time( this, "execute time" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x639ee0]
}

general_statistics_group::general_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	render_frame_time( this, "render frame time" ),
	cpu_render_frame_time( this, "cpu render frame time" ),
	num_setted_shader_constants( this, "num setted shader constants" ),
	fps( this, "fps" ),
	cpu_fps( this, "cpu fps" ),
	render_only_time( this, "render only time" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x639dd0]
}

speedtree_statistics_group::speedtree_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	render_time( this, "render time" ),
	culling_time( this, "culling time" ),
	num_instances( this, "num instances" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x639d10]
}

forward_decals_statistics_group::forward_decals_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	execute_time( this, "execute time" ),
	num_decals( this, "num decals" ),
	num_decal_draw_calls( this, "num decal draw calls" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x639240]
}

deferred_decals_statistics_group::deferred_decals_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	execute_time( this, "execute time" ),
	num_decals( this, "num decals" ),
	num_decal_draw_calls( this, "num decal draw calls" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6391b0]
}

distortion_pass_statistics_group::distortion_pass_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	accumulate_time( this, "accumulate time" ),
	apply_time( this, "apply time" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x639c30]
}

ssao_statistics_group::ssao_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	ssao_accumulate_time( this, "ssao accumulate time" ),
	ssao_blurring_time( this, "ssao blurring time" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x639b50]
}

sky_statistics_group::sky_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	execute_time( this, "execute time" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x639ac0]
}

lpv_statistics_group::lpv_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	lpv_lookup_time( this, "lpv lookup time" ),
	propagation_time( this, "propagation time" ),
	gv_injection_time( this, "gv injection time" ),
	vpl_injection_time( this, "vpl injection time" ),
	rsm_downsample_time( this, "rsm downsample time" ),
	rsm_rendering_time( this, "rsm rendering time" ),
	num_dips( this, "num dips" ),
	num_dips_in_cascade_0( this, "num dips in cascade 0" ),
	num_dips_in_cascade_1( this, "num dips in cascade 1" ),
	num_dips_in_cascade_2( this, "num dips in cascade 2" ),
	num_clipped_dips( this, "num clipped dips" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x639860]
}

grass_statistics_group::grass_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	num_total_patches( this, "num total patches" ),
	num_rendered_patches( this, "num rendered patches" ),
	num_visible_patches( this, "num visible patches" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x639120]
}

debug_statistics_group::debug_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	avaliable_video_memory( this, "avaliable video memory" ),
	texture_video_memory( this, "texture video memory" ),
	render_tergets_video_memory( this, "render targets video memory" ),
	gbuffer_video_memory( this, "gbuffer video memory" ),
	num_dips_in_lpv( this, "num dips in lpv" ),
	num_vertex_shader_changes( this, "num vertex shader changes" ),
	num_pixel_shader_changes( this, "num pixel shader changes" ),
	num_vs_textures_changes( this, "num vs textures changes" ),
	num_vs_constants_changes( this, "num vs constants changes" ),
	num_vs_samplers_changes( this, "num vs samplers changes" ),
	num_ps_textures_changes( this, "num ps textures changes" ),
	num_ps_constants_changes( this, "num ps constants changes" ),
	num_ps_samplers_changes( this, "num ps samplers changes" ),
	num_input_layout_changes( this, "num input layout changes" ),
	textures_compression_duration( this, "textures compression duration" ),
	dxt_rt_tex_creation_duration( this, "dxt rt tex creation duration" ),
	cpu_textures_compression_duration( this, "cpu textures compression duration" ),
	gpu_num_compressed_textures( this, "gpu num compressed textures" ),
	cpu_num_compressed_textures( this, "cpu num compressed textures" )
{
	// STATE[STUB]
	// FUNCTION BODY[0x638f10]
}

statistics::statistics( ) :
	first_group( 0 ),
	postprocess_stat_group( "post-process statistics" ),
	ssao_stat_group( "ssao statistics" ),
	distortion_pass_stat_group( "distortion pass statistics" ),
	particles_stat_group( "particles statistics" ),
	sky_stat_group( "sky statistics" ),
	speedtree_stat_group( "speedtree statistics" ),
	forward_stage_stat_group( "forward stage statistics" ),
	cascaded_sun_shadow_stat_group( "cascaded sun shadow statistics" ),
	grass_stat_group( "grass statistics" ),
	gbuffer_stat_group( "g-buffer statistics" ),
	forward_decals_stat_group( "forward decals statistics" ),
	lpv_stat_group( "light propagation volumes statistics" ),
	deferred_decals_stat_group( "deferred decals statistics" ),
	lights_stat_group( "lights statistics" ),
	visibility_stat_group( "visibility statistics" ),
	general_stat_group( "general statistics" ),
	debug_stat_group( "debug statistics" ),
	m_max_string_width( 0 ),
	m_max_string_height( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63a590]
}

void statistics::start( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6387e0]
}

statistics::~statistics( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x638d70]
}

} // namespace render
} // namespace vostok

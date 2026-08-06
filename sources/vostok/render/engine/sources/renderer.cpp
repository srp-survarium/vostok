#include "pch.h"
#include "renderer.h"

#include <vostok/console_command.h>
#include <vostok/particle/world.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/res_xs.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/dx11/sampler_state_descriptor.h>
#include <vostok/render/core/shader_constant_binding.h>
#include <vostok/render/core/sources/event_query.h>
#include <vostok/render/facade/scene_view_mode.h>
#include <vostok/render/facade/sources/ui_renderer.h>
#include <vostok/scaleform/sources/flash_renderer.h>
#include <vostok/ui/ui.h>

#include "clouds.h"
#include "effect_editor_gbuffer_to_screen.h"
#include "effect_fill_environment_probe_face.h"
#include "effect_fill_sky_ao_map.h"
#include "effect_grass_trample.h"
#include "grass_world.h"
#include "light.h"
#include "lights_db.h"
#include "material.h"
#include "register_samplers.h"
#include "render_output_window.h"
#include "render_model_instance_impl.h"
#include "render_surface.h"
#include "render_surface_instance.h"
#include "renderer_context.h"
#include "scene.h"
#include "scene_view.h"
#include "speedtree_forest.h"
#include "stage_accumulate_distortion.h"
#include "stage_ambient_lighting.h"
#include "stage_ambient_occlusion.h"
#include "stage_apply_distortion.h"
#include "stage_atmosphere.h"
#include "stage_clouds.h"
#include "stage_debug.h"
#include "stage_decals_accumulate.h"
#include "stage_forward.h"
#include "stage_gbuffer.h"
#include "stage_light_propagation_volumes.h"
#include "stage_lights.h"
#include "stage_particles.h"
#include "stage_postprocess.h"
#include "stage_pre_lighting.h"
#include "stage_pre_rain.h"
#include "stage_rain.h"
#include "stage_resolve_lighting.h"
#include "stage_screen_image.h"
#include "stage_shadow_direct.h"
#include "stage_sun.h"
#include "stage_translucency.h"
#include "stage_view_mode.h"
#include "stage_visibility.h"
#include "stage_volume_fog.h"
#include "statistics.h"
#include "system_renderer.h"
#include "temporal_projection_matrix_modifier.h"

// claude@NOTE: the target records `dynamic initializer for 's_ui_enabled_console_command'`
// WITHOUT a namespace qualifier while every other console command in this TU carries
// `vostok::render::` - the pair lives at global scope, ahead of the namespaces.
static bool s_ui_enabled = true;
static vostok::console_commands::cc_bool s_ui_enabled_console_command(
	"ui",
	s_ui_enabled,
	false,
	vostok::console_commands::command_type_user_specific
);

namespace vostok {
namespace render {

// defined in stage_gbuffer.cpp
void fill_surface( render_target_ptr surf, renderer_context* context );

static bool s_do_stages_profiling = false;
static console_commands::cc_bool s_do_stages_profiling_cc(
	"r_show_stage_stats",
	s_do_stages_profiling,
	false,
	console_commands::command_type_user_specific
);

static bool s_dxt_env_probe = true;
static console_commands::cc_bool s_dxt_env_probe_cc(
	"r_dxt_env_probe",
	s_dxt_env_probe,
	false,
	console_commands::command_type_user_specific
);

static bool s_execute_stages = true;
static console_commands::cc_bool s_execute_stages_cc(
	"r_execute_stages",
	s_execute_stages,
	false,
	console_commands::command_type_user_specific
);

static bool s_enable_rendering = true;
static console_commands::cc_bool s_enable_rendering_cc(
	"debug_enable_rendering",
	s_enable_rendering,
	false,
	console_commands::command_type_user_specific
);

static u32 s_view_mode_value = lit_view_mode;
static console_commands::cc_u32 s_view_mode(
	"r_view_mode",
	s_view_mode_value,
	0,
	num_view_modes,
	true,
	console_commands::command_type_user_specific
);

static bool s_use_gpu_sync_value = true;
static console_commands::cc_bool s_s_use_gpu_sync_cc(
	"r_gpu_sync",
	s_use_gpu_sync_value,
	false,
	console_commands::command_type_user_specific
);

static bool s_disabled_shader_constansts_set = false;
static vostok::console_commands::cc_bool s_disabled_shader_constansts_set_cc(
	"disabled_shader_constansts_set",
	s_disabled_shader_constansts_set,
	false,
	vostok::console_commands::command_type_engine_internal
);

static bool s_wireframe_value = false;
static console_commands::cc_bool s_wireframe_cc(
	"wireframe",
	s_wireframe_value,
	false,
	console_commands::command_type_user_specific
);

static bool s_unlit_value = false;
static console_commands::cc_bool s_unlit_cc(
	"unlit",
	s_unlit_value,
	true,
	console_commands::command_type_user_specific
);

static bool s_distortion_value = false;
static console_commands::cc_bool s_distortion_cc(
	"distortion",
	s_distortion_value,
	true,
	console_commands::command_type_engine_internal
);

static bool s_sorting_value = false;
static console_commands::cc_bool s_sorting_cc(
	"r_use_sorting_by_material",
	s_sorting_value,
	false,
	console_commands::command_type_engine_internal
);

static bool s_sorting2_value = false;
static console_commands::cc_bool s_sorting2_cc(
	"r_use_sorting_by_distance",
	s_sorting2_value,
	false,
	console_commands::command_type_engine_internal
);

static bool s_sorting3_value = true;
static console_commands::cc_bool s_sorting3_cc(
	"r_use_sorting_by_textures",
	s_sorting3_value,
	false,
	console_commands::command_type_engine_internal
);

static bool s_hiz_7_value = true;
static console_commands::cc_bool s_hiz_7(
	"hiz_7",
	s_hiz_7_value,
	false,
	console_commands::command_type_user_specific
);

static bool s_hiz_8_value = true;
static console_commands::cc_bool s_hiz_8(
	"hiz_8",
	s_hiz_8_value,
	false,
	console_commands::command_type_user_specific
);

static bool s_draw_frame_histogram_value = false;
static console_commands::cc_bool s_draw_fps_histogram_cc(
	"r_draw_frame_histogram",
	s_draw_frame_histogram_value,
	false,
	console_commands::command_type_user_specific
);

static u32 s_draw_most_dips_models_list_value = 3;
static console_commands::cc_u32 s_draw_most_dips_models_list_cc(
	"r_draw_most_dips_models_list_by_lod",
	s_draw_most_dips_models_list_value,
	0,
	10,
	false,
	console_commands::command_type_user_specific
);

static bool s_dump_scene_statistics_value = false;
static console_commands::cc_bool s_dump_scene_statistics_cc(
	"r_dump_scene_statistics",
	s_dump_scene_statistics_value,
	false,
	console_commands::command_type_user_specific
);

static bool s_debug_remove_trample = false;
static console_commands::cc_bool s_debug_remove_trample_cc(
	"r_remove_trample",
	s_debug_remove_trample,
	false,
	console_commands::command_type_user_specific
);

static bool s_debug_use_skeletel_mesh_lods_value = true;
static console_commands::cc_bool s_debug_use_skeletel_mesh_lods_cc(
	"r_debug_use_skeletel_mesh_lods",
	s_debug_use_skeletel_mesh_lods_value,
	false,
	console_commands::command_type_user_specific
);

struct stage_stat {
	double average_time( bool gpu_time ) const
	{
		double result	= 0.;
		for ( u32 i = 0; i < array_size( elapsed_gpu_msec ); ++i )
			result		+= gpu_time ? elapsed_gpu_msec[i] : elapsed_cpu_msec[i];

		return result / array_size( elapsed_gpu_msec );
	}

	u32 average_dips( ) const
	{
		double result	= 0.;
		for ( u32 i = 0; i < array_size( dips ); ++i )
			result		+= dips[i];

		return u32( result / array_size( dips ) );
	}

	double elapsed_gpu_msec[1];
	double elapsed_cpu_msec[1];
	u32 dips[1];
	stage* stg;
};

static stage_stat s_render_stages[num_render_stages];
static stage_stat s_visibility_stage_stats;

pcstr render_stage_names[] = {
	"g-buffer",
	"decals accumulate",
	"accumulate distortion",
	"pre rain_normal modify",
	"pre lighting",
	"ambient occlusion",
	"ambient lighting",
	"sun cascade shadow",
	"sun",
	"deferred lighting",
	"light propagation volumes",
	"translucency",
	"resolve lighting",
	"skybox",
	"skysphere",
	"clouds",
	"atmosphere",
	"forward",
	"atmosphere on geometry",
	"apply distortion",
	"forward sky",
	"rain",
	"lighting",
	"particles",
	"volume fog",
	"post process",
	"debug post process",
	"debug",
	"shadow",
	"visibility"
};

struct remove_model_filter_predicate {
	remove_model_filter_predicate( ) :
		m_need_static( false ),
		m_need_skeletal( false ),
		m_need_translucency( false )
	{
	}

	remove_model_filter_predicate(
		bool need_static,
		bool need_skeletal,
		bool need_translucency
	) :
		m_need_static( need_static ),
		m_need_skeletal( need_skeletal ),
		m_need_translucency( need_translucency )
	{
	}

	bool operator()( render_surface_instance* )
	{
		return false;
	}

	bool m_need_static;
	bool m_need_skeletal;
	bool m_need_translucency;
};

struct remove_model_if_not_in_frustum_predicate {
	explicit remove_model_if_not_in_frustum_predicate( math::frustum& frustum ) :
		m_frustum( &frustum )
	{
	}

	bool operator()( render_surface_instance* )
	{
		return false;
	}

	math::frustum* m_frustum;
};

struct remove_model_if_olt_predicate {
	remove_model_if_olt_predicate( ) :
		m_use_olt( false )
	{
	}

	explicit remove_model_if_olt_predicate( u32 use_olt ) :
		m_use_olt( use_olt != 0 )
	{
	}

	bool operator()( render_surface_instance* )
	{
		return false;
	}

	bool m_use_olt;
};

bool renderer::is_effects_ready( ) const
{
	// FUNCTION BODY[0x6473d0]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6473d0>|0x000|+0x033:'163'
	// <0x647403>|0x033|-0x001:'163'
	// <0x647402>|0x032|+0x003:'164'
	// ******

	return m_gbuffer_to_screen_shader.c_ptr( ) != NULL;
}

static statistics m_statistics;

// claude@NOTE: no legacy ancestor - effect_pick_light_luminance postdates the legacy corpus; matcher-phase work.
// STATE[STUB]
void effect_pick_light_luminance::compile( effect_compiler& compiler, custom_config_value const& config )
{
	// FUNCTION BODY[0x64c70]: 11
	// <0>
	// <0x64c7b>|0x00b|+0x009:'174'
	// <0x64c84>|0x014|+0x02d:'175'
	// <0x64cb1>|0x041|+0x048:'176'
	// <0x64cf9>|0x089|+0x00a:'177'
	// <0x64d03>|0x093|+0x00c:'178'
	// <0x64d0f>|0x09f|+0x014:'179'
	// <0x64d23>|0x0b3|+0x014:'180'
	// <0x64d37>|0x0c7|+0x014:'181'
	// <0x64d4b>|0x0db|+0x006:'182'
	// <0x64d51>|0x0e1|+0x005:'183'
	// ******
}

renderer::renderer( renderer_context* renderer_context ) :
	m_renderer_context	( renderer_context ),
	m_last_frame_time	( 0.f ),
	m_current_time		( 0.f ),
	m_simulation		( 32, 32, 32 )
{
	register_samplers				( );

	m_debug_readed_data[0]			= float4( 0.f, 0.f, 0.f, 0.f );
	m_debug_readed_data[1]			= float4( 0.f, 0.f, 0.f, 0.f );
	m_debug_readed_data[2]			= float4( 0.f, 0.f, 0.f, 0.f );
	m_debug_readed_data[3]			= float4( 0.f, 0.f, 0.f, 0.f );

	m_picking_lighting_luminance_mode			= false;

	m_selected_lighting_luminanace_in_screen	= float4( 0.f, 0.f, 0.f, 0.f );

	material::initialize_nomaterial_material( );

	effect_manager::ref( ).create_effect< effect_pick_light_luminance >		( &m_pick_light_luminance_effect );
	effect_manager::ref( ).create_effect< effect_editor_gbuffer_to_screen >	( &m_gbuffer_to_screen_shader );
	effect_manager::ref( ).create_effect< effect_fill_environment_probe_face >( &m_fill_environment_probe_face );
	effect_manager::ref( ).create_effect< effect_fill_sky_ao_map >			( &m_fill_sky_ao_map_effect );
	effect_manager::ref( ).create_effect< effect_grass_trample >				( &m_grass_trample_effect );

	m_c_pick_lighting_luminance_position	= backend::ref( ).register_constant_host( "pick_lighting_luminance_position", rc_float );
	m_gbuffer_to_screen_type				= backend::ref( ).register_constant_host( "gbuffer_to_screen_type", rc_int );
	m_c_eye_ray_corner						= backend::ref( ).register_constant_host( "eye_ray_corner", rc_float );
	m_c_probe_position						= backend::ref( ).register_constant_host( "probe_position", rc_float );

	m_view_to_rain_shadow			= float4x4( ).identity( );

	m_stages.resize					( num_render_stages );

	for ( stage** it = m_stages.begin( ); it != m_stages.end( ); ++it )
		*it = 0;

	m_stage_debug					= 0;
	m_view_mode_stage				= 0;
	m_present_stage					= 0;


	m_stages[gbuffer_render_stage]					= NEW( stage_gbuffer )					( this, m_renderer_context );
	m_stages[decals_accumulate_render_stage]		= NEW( stage_decals_accumulate )		( this, m_renderer_context );
	m_stages[accumulate_distortion_render_stage]	= NEW( stage_accumulate_distortion )	( this, m_renderer_context );
	m_stages[pre_rain_normal_modify_render_stage]	= NEW( stage_pre_rain )					( this, m_renderer_context );
	m_stages[pre_lighting_render_stage]				= NEW( stage_pre_lighting )				( this, m_renderer_context );
	m_stages[ambient_occlusion_render_stage]		= NEW( stage_ambient_occlusion )		( this, m_renderer_context );
	m_stages[ambient_lighting_render_stage]			= NEW( stage_ambient_lighting )			( this, m_renderer_context );
	m_stages[sun_shadows_accumulate_render_stage]	= NEW( stage_shadow_direct )			( this, m_renderer_context );
	m_stages[sun_render_stage]						= NEW( stage_sun )						( this, m_renderer_context, m_cloud_interp_textures, m_simulation );
	m_stages[deferred_lighting_render_stage]		= NEW( stage_lights )					( this, m_renderer_context, false );
	m_stages[light_propagation_volumes_render_stage]= NEW( stage_light_propagation_volumes )( this, m_renderer_context );
	m_stages[translucency_render_stage]				= NEW( stage_translucency )				( this, m_renderer_context );
	m_stages[resolve_lighting_render_stage]			= NEW( stage_resolve_lighting )			( this, m_renderer_context );
	m_stages[clouds_render_stage]					= NEW( stage_clouds )					( this, m_cloud_interp_textures, m_simulation, m_renderer_context );
	m_stages[atmosphere_render_stage]				= NEW( stage_atmosphere )				( this, m_renderer_context, stage_atmosphere::atmosphere_on_sky );
	m_stages[forward_render_stage]					= NEW( stage_forward )					( this, m_renderer_context, stage_forward::forward_base );
	m_stages[atmosphere_on_geometry_render_stage]	= NEW( stage_atmosphere )				( this, m_renderer_context, stage_atmosphere::atmosphere_on_geometry );
	m_stages[forward_sky_render_stage]				= NEW( stage_forward )					( this, m_renderer_context, stage_forward::forward_sky );
	m_stages[apply_distortion_render_stage]			= NEW( stage_apply_distortion )			( this, m_renderer_context );
	m_stages[rain_render_stage]						= NEW( stage_rain )						( this, m_renderer_context );
	m_stages[particles_render_stage]				= NEW( stage_particles )				( this, m_renderer_context );
	m_stages[lighting_render_stage]					= NEW( stage_lights )					( this, m_renderer_context, true );
	m_stages[volume_fog_render_stage]				= NEW( stage_volume_fog )				( this, m_renderer_context );
	m_stages[post_process_render_stage]				= NEW( stage_postprocess )				( this, m_renderer_context );

	m_visibility_stage				= NEW( stage_visibility )	( this, m_renderer_context );
	m_stage_debug					= NEW( stage_debug )		( this, m_renderer_context );
	m_view_mode_stage				= NEW( stage_view_mode )	( this, m_renderer_context );
	m_present_stage					= NEW( stage_screen_image )	( this, m_renderer_context );

	m_frame_sync_event				= NEW( event_query )( );
	m_timing_event					= NEW( event_query )( );
	m_timer.start					( );
}

renderer::~renderer( )
{
	// LOCALS
	// std::reverse_iterator< stage** > e
	// ******

	// CALL SITE INFO
	// <0x6483b4> -> void* < unknown >( u32 )
	// <0x648402> -> void* < unknown >( u32 )
	// <0x648441> -> void* < unknown >( u32 )
	// <0x648481> -> void* < unknown >( u32 )
	// <0x6484c1> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x648310]: 12
	// <0>
	// <1>
	// <0x64831a>|0x00a|+0x034:'271'
	// <0x64834e>|0x03e|+0x031:'272'
	// <0>
	// <0x64837f>|0x06f|+0x014:'274'
	// <0x648393>|0x083|+0x047:'275'
	// <0>
	// <0x6483da>|0x0ca|+0x041:'277'
	// <0x64841b>|0x10b|+0x040:'278'
	// <0x64845b>|0x14b|+0x040:'279'
	// <0x64849b>|0x18b|+0x040:'280'
	// ******

	DELETE					( m_frame_sync_event );
	DELETE					( m_timing_event );

	for ( std::reverse_iterator< stage** > it = m_stages.rbegin( ), e = m_stages.rend( ); it != e; ++it )
		DELETE				( *it );

	DELETE					( m_visibility_stage );
	DELETE					( m_view_mode_stage );
	DELETE					( m_present_stage );
	DELETE					( m_stage_debug );
}

void renderer::recreate_stage( enum_render_stage_type arg_0 )
{
	switch ( arg_0 )
	{
	case gbuffer_render_stage:					DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_gbuffer )( this, m_renderer_context ); break;
	case decals_accumulate_render_stage:		DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_decals_accumulate )( this, m_renderer_context ); break;
	case accumulate_distortion_render_stage:	DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_accumulate_distortion )( this, m_renderer_context ); break;
	case pre_rain_normal_modify_render_stage:	DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_pre_rain )( this, m_renderer_context ); break;
	case pre_lighting_render_stage:				DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_pre_lighting )( this, m_renderer_context ); break;
	case ambient_occlusion_render_stage:		DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_ambient_occlusion )( this, m_renderer_context ); break;
	case ambient_lighting_render_stage:			DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_ambient_lighting )( this, m_renderer_context ); break;
	case sun_shadows_accumulate_render_stage:	DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_shadow_direct )( this, m_renderer_context ); break;
	case sun_render_stage:						DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_sun )( this, m_renderer_context, m_cloud_interp_textures, m_simulation ); break;
	case deferred_lighting_render_stage:		DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_lights )( this, m_renderer_context, false ); break;
	case light_propagation_volumes_render_stage: DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_light_propagation_volumes )( this, m_renderer_context ); break;
	case translucency_render_stage:				DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_translucency )( this, m_renderer_context ); break;
	case resolve_lighting_render_stage:			DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_resolve_lighting )( this, m_renderer_context ); break;
	case clouds_render_stage:					DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_clouds )( this, m_cloud_interp_textures, m_simulation, m_renderer_context ); break;
	case atmosphere_render_stage:				DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_atmosphere )( this, m_renderer_context, stage_atmosphere::atmosphere_on_sky ); break;
	case forward_render_stage:					DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_forward )( this, m_renderer_context, stage_forward::forward_base ); break;
	case atmosphere_on_geometry_render_stage:	DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_atmosphere )( this, m_renderer_context, stage_atmosphere::atmosphere_on_geometry ); break;
	case apply_distortion_render_stage:			DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_apply_distortion )( this, m_renderer_context ); break;
	case forward_sky_render_stage:				DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_forward )( this, m_renderer_context, stage_forward::forward_sky ); break;
	case rain_render_stage:						DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_rain )( this, m_renderer_context ); break;
	case particles_render_stage:					DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_particles )( this, m_renderer_context ); break;
	case lighting_render_stage:					DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_lights )( this, m_renderer_context, true ); break;
	case volume_fog_render_stage:				DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_volume_fog )( this, m_renderer_context ); break;
	case post_process_render_stage:				DELETE( m_stages[arg_0] ); m_stages[arg_0] = NEW( stage_postprocess )( this, m_renderer_context ); break;
	default:									break;
	}
}

// claude@NOTE: no legacy ancestor - absent from the legacy renderer.cpp remainder; matcher-phase work.
// STATE[STUB]
void renderer::set_target_context( renderer_context_targets const* targets_context, bool force_set )
{
	// FUNCTION BODY[0x6476d0]: 1
	// <0x6476d1>|0x001|+0x00d:'316'
	// ******
}

void renderer::setup_render_output_window(
	base_output_window_ptr				in_output_window,
	math::rectangle< float2 > const&	viewport
)
{
	// LOCALS
	// math::rectangle< float2 > 		res_viewport
	// D3D11_VIEWPORT 					d3d_viewport
	// ******

	// CALL SITE INFO
	// <0x6482d6> -> void < unknown >( u32, D3D11_VIEWPORT const* )
	// ******

	// FUNCTION BODY[0x648130]: 27
	// <0>
	// <0x648133>|0x003|+0x00a:'322'
	// <0>
	// <0x64813d>|0x00d|+0x015:'324'
	// <0x648152>|0x022|+0x011:'325'
	// <0x648163>|0x033|+0x011:'326'
	// <0>
	// <1>
	// <0x648174>|0x044|+0x06a:'329'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6481de>|0x0ae|+0x010:'334'
	// <0x6481ee>|0x0be|+0x012:'335'
	// <0x648200>|0x0d0|+0x016:'336'
	// <0x648216>|0x0e6|+0x012:'337'
	// <0>
	// <0x648228>|0x0f8|+0x018:'339'
	// <0x648240>|0x110|+0x015:'340'
	// <0>
	// <1>
	// <2>
	// <0x648255>|0x125|+0x034:'344'
	// <0>
	// <1>
	// <0x648289>|0x159|+0x04f:'347'
	// ******

	R_ASSERT				( in_output_window );
	render_output_window* const output_window = (render_output_window*)in_output_window.c_ptr( );

	m_renderer_context->set_target_context( &output_window->target_context( ), false );
	backend::ref( ).set_render_output( output_window->render_output( ) );
	backend::ref( ).reset_depth_stencil_target( );

	math::rectangle< float2 > res_viewport;
	res_viewport			= math::rectangle< float2 >( float2( 0, 0 ), float2( 1.f, 1.f ) );

	R_ASSERT				( viewport.width( ) );
	R_ASSERT				( viewport.height( ) );

	res_viewport.left		= math::max( res_viewport.left, viewport.left );
	res_viewport.right		= math::min( res_viewport.right, viewport.right );
	res_viewport.top		= math::max( res_viewport.top, viewport.top );
	res_viewport.bottom		= math::min( res_viewport.bottom, viewport.bottom );

	u32 const window_width	= backend::ref( ).target_width( );
	u32 const window_height	= backend::ref( ).target_height( );

	D3D11_VIEWPORT d3d_viewport = { window_width * res_viewport.left, window_height * res_viewport.top, window_width * res_viewport.width( ), window_height * res_viewport.height( ), 0.f, 1.f };

	backend::ref( ).set_viewport( d3d_viewport );
}

void renderer::toggle_render_stage( enum_render_stage_type stage_type, bool toggle )
{
	// FUNCTION BODY[0x6473b0]: 2
	// <0x6473b0>|0x000|+0x00f:'352'
	// <0x6473bf>|0x00f|+0x009:'353'
	// ******

	if ( m_stages[stage_type] )
		m_stages[stage_type]->set_enabled( toggle );
}

// claude@NOTE: no legacy ancestor - absent from the legacy renderer.cpp remainder (model_manager/engine-world clear_resources are different classes); matcher-phase work.
// STATE[STUB]
void renderer::clear_resources( )
{
	// FUNCTION BODY[0x6488a0]: 1
	// <0x6488a1>|0x001|+0x014:'358'
	// ******
}

void renderer::execute_stages( )
{
	if ( !s_execute_stages )
	{
		return;
	}

	if ( s_do_stages_profiling )
	{

		m_timing_event->issue	( );
		m_timing_event->wait	( );
	}

	stage_stat* stat			= s_render_stages;
	for ( stage** it = m_stages.begin( ); it != m_stages.end( ); ++it, ++stat )
	{
		stage* current_stage	= *it;

		stat->stg				= current_stage;

		if ( !current_stage )
			continue;

		u32 prev_draw_calls;




		if ( s_do_stages_profiling )
		{
			m_timing_timer.start( );
			prev_draw_calls		= backend::ref( ).num_draw_calls;
		}

		current_stage->execute	( );

		if ( s_do_stages_profiling )
		{
			stat->dips[0]				= backend::ref( ).num_draw_calls - prev_draw_calls;
			stat->elapsed_cpu_msec[0]	= m_timing_timer.get_elapsed_sec( ) * 1000.0;

			m_timing_event->issue	( );
			m_timing_event->wait	( );

			stat->elapsed_gpu_msec[0]	= m_timing_timer.get_elapsed_sec( ) * 1000.0;
		}
	}
}

bool sort_by_vs_predicate::operator()( render_surface_instance const* left, render_surface_instance const* right ) const
{
	material_effects const& left_material_effects =
		left->m_render_surface->get_material_effects( );
	material_effects const& right_material_effects =
		right->m_render_surface->get_material_effects( );

	res_pass const* const left_pass = left_material_effects.m_effects[m_stage_type]
		->get_technique( m_tech_index )->get_pass( 0 );
	res_pass const* const right_pass = right_material_effects.m_effects[m_stage_type]
		->get_technique( m_tech_index )->get_pass( 0 );

	return left_pass->get_ps( )->hardware_shader( )->hardware_shader( )
		< right_pass->get_ps( )->hardware_shader( )->hardware_shader( );
}

bool sort_by_distance_predicate::operator()( render_surface_instance const* left, render_surface_instance const* right ) const
{
	float3 pos0 = left->m_transform->c.xyz( );
	float3 pos1 = right->m_transform->c.xyz( );

	pos0.x = float( math::floor( pos0.x ) );
	pos0.y = float( math::floor( pos0.y ) );
	pos0.z = float( math::floor( pos0.z ) );

	pos1.x = float( math::floor( pos1.x ) );
	pos1.y = float( math::floor( pos1.y ) );
	pos1.z = float( math::floor( pos1.z ) );

	float const distance0 = math::squared_length( pos0 - m_eye_position );
	float const distance1 = math::squared_length( pos1 - m_eye_position );

	return m_from_near_to_far ? distance0 < distance1 : distance0 > distance1;
}

bool sort_by_texture_predicate::operator()( render_surface_instance const* left, render_surface_instance const* right ) const
{
	material_effects const& left_material_effects =
		left->m_render_surface->get_material_effects( );
	material_effects const& right_material_effects =
		right->m_render_surface->get_material_effects( );

	res_pass const* const left_pass = left_material_effects.m_effects[m_stage_type]
		->get_technique( m_tech_index )->get_pass( 0 );
	res_pass const* const right_pass = right_material_effects.m_effects[m_stage_type]
		->get_technique( m_tech_index )->get_pass( 0 );

	return left_pass->get_ps( )->m_textures->compare(
		*right_pass->get_ps( )->m_textures
	) < 0;
}

static float screen_factor( float3 const& view_position, math::aabb bbox, float4x4 const& model_transform )
{
	bbox.modify					( model_transform );

	float3 const center			= bbox.center( );
	float3 const extents		= bbox.extents( );
	float const distance		= math::squared_length( view_position - center );
	return						( math::clamp_r( math::max( extents.x, extents.y, extents.z )/math::max( distance, math::epsilon_6 ), 0.f, 1.f ) );
}

void renderer::fill_opaque_models( )
{
	if ( !options::ref( ).current.m_use_shader_lods )
	{
		vector<render_surface_instance*>& opaque_models =
			m_renderer_context->get_scene_view( )->get_visible_opaque_models( );
		opaque_models.clear( );

		for ( render_surface_instance* const* i = m_renderer_context->get_scene_view( )->get_visible_models( ).begin( ), * const* e = m_renderer_context->get_scene_view( )->get_visible_models( ).end( ); i != e; ++i )
		{
			material_effects const& me = ( *i )->m_render_surface->get_material_effects( );
			if ( me.m_effects[gbuffer_render_stage].c_ptr( ) )
				opaque_models.push_back( *i );
		}

		sort_models( opaque_models, gbuffer_render_stage, 0 );
		return;
	}

	vector<render_surface_instance*>& opaque_models =
		m_renderer_context->get_scene_view( )->get_visible_opaque_models( );
	opaque_models.clear( );

	vector<render_surface_instance*> opaque_models_lod0, opaque_models_lod1;

	for ( render_surface_instance* const* i = m_renderer_context->get_scene_view( )->get_visible_models( ).begin( ), * const* e = m_renderer_context->get_scene_view( )->get_visible_models( ).end( ); i != e; ++i )
	{
		material_effects const& me =
			( *i )->m_render_surface->get_material_effects( );
		if ( !me.m_effects[gbuffer_render_stage].c_ptr( ) )
			continue;

		float const factor = screen_factor(
			m_renderer_context->get_view_pos( ),
			( *i )->m_parent->get_aabb( ),
			*( *i )->m_transform
		);

		bool const skeletal_mesh =
			me.get_vertex_input_type( ) == skeletal_4_bones_mesh_vertex_input_type
			|| me.get_vertex_input_type( ) == skeletal_3_bones_mesh_vertex_input_type
			|| me.get_vertex_input_type( ) == skeletal_2_bones_mesh_vertex_input_type
			|| me.get_vertex_input_type( ) == skeletal_1_bones_mesh_vertex_input_type;

		float const lod0_threshold =
			s_debug_use_skeletel_mesh_lods_value && skeletal_mesh ? 0.01f : 0.005f;

		( *i )->m_shader_lod_index = factor > lod0_threshold
			? 0
			: factor <= 0.0000333f
				&& ( *i )->m_render_surface
				&& ( *i )->m_render_surface->get_vertex_input_type( ) == static_mesh_vertex_input_type
				? 2
				: 1;

		( *i )->m_dynamic_screen_factor = factor;

		if ( ( *i )->m_shader_lod_index == 0 )
			opaque_models_lod0.push_back( *i );
		else if ( ( *i )->m_shader_lod_index == 1 )
			opaque_models_lod1.push_back( *i );
	}

	if ( opaque_models_lod0.size( ) )
		sort_models( opaque_models_lod0, gbuffer_render_stage, 1 );

	if ( opaque_models_lod1.size( ) )
		sort_models( opaque_models_lod1, gbuffer_render_stage, 3 );

	if (
		!opaque_models_lod0.size( )
		&& !opaque_models_lod1.size( )
	)
		return;

	opaque_models.reserve( opaque_models_lod0.size( ) + opaque_models_lod1.size( ) );

	for ( render_surface_instance* const* i = opaque_models_lod0.begin( ), * const* e = opaque_models_lod0.end( ); i != e; ++i )
		opaque_models.push_back( *i );

	for ( render_surface_instance* const* i = opaque_models_lod1.begin( ), * const* e = opaque_models_lod1.end( ); i != e; ++i )
		opaque_models.push_back( *i );
}

void renderer::sort_models_by_distance( vector< render_surface_instance* >& instances, bool from_near_to_far )
{
	float3 pos = m_renderer_context->get_view_pos( );

	pos.x = float( math::floor( pos.x ) );
	pos.y = float( math::floor( pos.y ) );
	pos.z = float( math::floor( pos.z ) );

	std::sort(
		instances.begin( ),
		instances.end( ),
		sort_by_distance_predicate(
			pos,
			from_near_to_far
		)
	);
}

void renderer::sort_models(
	vector<render_surface_instance*>& instances,
	enum_render_stage_type stage_type,
	u32 const tech_index
)
{
	if ( s_sorting_value )
		std::sort(
			instances.begin( ),
			instances.end( ),
			sort_by_vs_predicate( stage_type, tech_index )
		);

	if ( s_sorting3_value )
		std::sort(
			instances.begin( ),
			instances.end( ),
			sort_by_texture_predicate( stage_type, tech_index )
		);
}

static void push_point(
	vectora< ui::vertex >&		out_vertices,
	u32							index,
	float						x,
	float						y,
	float						z,
	u32							c,
	float						u,
	float						v
)
{
	// FUNCTION BODY[0x638870]
	// claude@NOTE: VA corrected from stale 0x647360; match.db attributes 0x638870 to statistics.cpp:89 (per-TU static twin).
	vostok::render::ui::vertex& vertex_item	= out_vertices[index];
	vertex_item.m_position.set				(x, y, z, 1);
	vertex_item.m_uv.set					(u, v);
	vertex_item.m_color						= c;
}

static void make_ui_vertices(
	vectora< ui::vertex >&		out_vertices,
	pcstr						in_text,
	vostok::ui::font const&				in_font,
	float2 const&				in_position,
	math::color const&			in_color,
	math::color const&			in_selection_color,
	u32							max_line_width,
	bool						is_multiline,
	u32							start_selection_index,
	u32							end_selection_index
)
{
	// FUNCTION BODY[0x647410]: 50
	using namespace vostok::math;

	float2 pos_rt			(0, 0);
	float2 pos				= in_position;
	u32 symb_count			= strlen(in_text);

	pcstr ch				= in_text;
	float const height		= in_font.get_height();
	float height_ts			= in_font.get_height_ts();

	float curr_word_len		= 0.0f;
	pcstr next_word			= NULL;

	if(is_multiline)
		in_font.parse_word	(ch, curr_word_len, next_word);

	u32 index = 0;

	for(u32 i=0; i<symb_count; ++i, ++ch)
	{
		u32 clr = (i>=start_selection_index && i<end_selection_index) ? in_selection_color.m_value : in_color.m_value;

		float3 uv			= in_font.get_char_tc_ts(*ch);
		float3 const& tc	= in_font.get_char_tc(*ch);

		if (in_text[i]=='\n' || in_text[i]=='\r\n')
		{
			in_font.parse_word	(ch, curr_word_len, next_word);
			pos_rt.x			= 0.0f;
			pos_rt.y			+= height;
		}

		if(is_multiline && (ch==next_word))
		{
			in_font.parse_word		(ch, curr_word_len, next_word);
			if(pos_rt.x + curr_word_len > max_line_width)
			{
				pos_rt.x		= 0.0f;
				pos_rt.y		+= height;
			}
		}

		push_point(out_vertices, index++, pos.x+pos_rt.x,		pos.y+pos_rt.y+height,	0.0f, clr, uv.x,		uv.y+height_ts);
		push_point(out_vertices, index++, pos.x+pos_rt.x,		pos.y+pos_rt.y,			0.0f, clr, uv.x,		uv.y);
		push_point(out_vertices, index++, pos.x+pos_rt.x+tc.z,	pos.y+pos_rt.y+height,	0.0f, clr, uv.x+uv.z,	uv.y+height_ts);
		push_point(out_vertices, index++, pos.x+pos_rt.x+tc.z,	pos.y+pos_rt.y,			0.0f, clr, uv.x+uv.z,	uv.y);

		pos_rt.x			+= tc.z;
	}
}

static void draw_text(
	vostok::ui::font const*		in_font,
	pcstr				str,
	u32					pos_x,
	u32					pos_y,
	u32					clr
)
{
	// FUNCTION BODY[0x648b70]: 20
	// claude@NOTE: legacy (statistics.cpp) took ui::world& and looked up default_font(); the
	// canonical signature receives the font directly
	u32 const string_length		= strlen(str);
	math::color string_color	(clr);

	vostok::vectora<vostok::render::ui::vertex> out_vertices(g_allocator);
	out_vertices.resize(string_length * 4);

	make_ui_vertices(
		out_vertices,
		str,
		*in_font,
		float2(float(pos_x), float(pos_y)),
		string_color,
		string_color,
		400,
		false,
		0,
		string_length
	);

	system_renderer::ref().draw_ui_vertices((vertex_formats::TL*)&out_vertices.front(), out_vertices.size(), 0, 0);
}

static void draw_text_shadowed(
	vostok::ui::font const*		in_font,
	pcstr				str,
	u32					pos_x,
	u32					pos_y,
	u32					clr
)
{
	// FUNCTION BODY[0x648c80]: 2
	draw_text(in_font, str, pos_x + 1, pos_y + 1, math::color_rgba(0.f, 0.f, 0.f, 1.f));
	draw_text(in_font, str, pos_x, pos_y, clr);
}

// claude@NOTE: the second disjunct of the line-1229 guard is an LTCG-folded
// constant (`mov edx, <code address>; test edx, edx; je`) whose source spelling
// could not be recovered - only the `output_window.c_ptr( )` half is written here.
void renderer::render(
	base_scene_ptr const&				in_scene,
	base_scene_view_ptr const&			in_view,
	base_output_window_ptr const&		output_window,
	math::rectangle< float2 > const&	viewport,
	boost::function< void( bool ) > const&	on_draw_scene,
	bool								draw_debug_terrain,
	vostok::ui::font const*				default_font
)
{
	backend::ref( ).num_vs_changes	= 0;
	backend::ref( ).num_ps_changes	= 0;
	backend::ref( ).num_il_changes	= 0;
	backend::ref( ).num_vsc_changes	= 0;
	backend::ref( ).num_vst_changes	= 0;
	backend::ref( ).num_vss_changes	= 0;
	backend::ref( ).num_psc_changes	= 0;
	backend::ref( ).num_pst_changes	= 0;
	backend::ref( ).num_pss_changes	= 0;

	float const frame_time		= m_fps_timer.get_elapsed_sec( );

	m_fps_timer.start			( );

	statistics::ref( ).start	( );

	VOSTOK_UNREFERENCED_PARAMETER( draw_debug_terrain );


	if ( !static_cast_checked< render_output_window* >( output_window.c_ptr( ) )->render_output( )->valid_present( ) )
	{
		static_cast_checked< render_output_window* >( output_window.c_ptr( ) )->render_output( )->present( );

		return;
	}





	if ( !s_enable_rendering )
	{
		static_cast_checked< render_output_window* >( output_window.c_ptr( ) )->render_output( )->present( );

		static_cast_checked< render::scene* >( in_scene.c_ptr( ) )->flush( on_draw_scene, true, true );

		return;
	}

	render::scene* const scene		= static_cast_checked< render::scene* >( in_scene.c_ptr( ) );
	render::scene_view* view		= static_cast_checked< render::scene_view* >( in_view.c_ptr( ) );



	scene_view_mode view_mode		= view->get_view_mode( );
	if ( s_view_mode_value != lit_view_mode )
		view_mode					= scene_view_mode( s_view_mode_value );
	if ( s_wireframe_value )		view_mode = wireframe_view_mode;
	if ( s_unlit_value )			view_mode = unlit_view_mode;
	if ( s_distortion_value )		view_mode = unlit_view_mode;

	view->inc_render_frame_index( );

	m_current_time				= m_timer.get_elapsed_sec( );

	float const time_delta		= math::max( m_current_time - m_last_frame_time, 0.f ) * scene->get_slomo( );

	scene->lights( ).tick		( time_delta );

	m_renderer_context->set_scene		( scene );
	m_renderer_context->set_scene_view	( view );
	m_renderer_context->set_current_time	( m_last_frame_time );
	m_renderer_context->set_time_delta	( time_delta );


	static_cast_checked< render_output_window* >( output_window.c_ptr( ) )->resize( device::ref( ).m_device_removed );

	setup_render_output_window	( output_window, viewport );

	u32 const window_size_x		= static_cast_checked< render_output_window* >( output_window.c_ptr( ) )->render_output( )->width( );
	u32 const window_size_y		= static_cast_checked< render_output_window* >( output_window.c_ptr( ) )->render_output( )->height( );

	if ( !is_effects_ready( ) || !material::is_nomaterial_material_ready( ) )
	{
		static_cast_checked< render::scene* >( in_scene.c_ptr( ) )->flush( on_draw_scene, true, true );

		return;
	}


	if ( options::ref( ).current.m_use_texture_streaming )
		scene->process_streaming( m_renderer_context->get_p( ), m_renderer_context->get_view_pos( ), window_size_x, window_size_y );



	bool const need_temporal_jittering = options::ref( ).current.m_use_temporal_antialiasing && view_mode == lit_view_mode && m_renderer_context->scene_view( )->m_use_post_process;

	temporal_projection_matrix_modifier temporal_jitterer( m_renderer_context, window_size_x, window_size_y, need_temporal_jittering );
	temporal_jitterer.push_jittering( );


	if ( m_renderer_context->scene( )->get_clouds( ) )
	{
		light* const sun		= m_renderer_context->scene( )->lights( ).get_sun( ).c_ptr( );

		if ( sun )
			m_renderer_context->scene( )->get_clouds( )->set_sun_direction( sun->direction );

		m_renderer_context->scene( )->get_clouds( )->set_time( m_current_time * 0.1f );
	}

	m_renderer_context->m_light_marker_id = 1;

	m_renderer_context->scene( )->update_models( );

	if ( scene->get_speedtree_forest( ) )
		scene->get_speedtree_forest( )->tick( m_renderer_context );

	particle::world* const part_world = m_renderer_context->scene( )->particle_world( );
	if ( part_world )
		part_world->tick		( time_delta, m_renderer_context->get_v( ) );

	backend::ref( ).reset		( );



	if ( s_do_stages_profiling )
	{

		m_timing_event->issue	( );
		m_timing_event->wait	( );
		m_timing_timer.start	( );
	}

	m_visibility_stage->execute	( );

	if ( s_do_stages_profiling )
	{
		s_visibility_stage_stats.dips[0]				= 0;


		s_visibility_stage_stats.elapsed_cpu_msec[0]	= m_timing_timer.get_elapsed_sec( ) * 1000.0;
		s_visibility_stage_stats.stg					= m_visibility_stage;


		m_timing_event->issue	( );
		m_timing_event->wait	( );

		s_visibility_stage_stats.elapsed_gpu_msec[0]	= m_timing_timer.get_elapsed_sec( ) * 1000.0;
	}


	backend::ref( ).reset		( );

	backend::ref( ).reset_depth_stencil_target( );


	if ( scene->get_grass( ) && options::ref( ).current.m_use_vegetation_trample )
	{
		if ( s_debug_remove_trample )
		{
			scene->get_grass( )->remove_trample( );
			s_debug_remove_trample = false;
		}

		scene->get_grass( )->accumulate_trample( this, m_renderer_context );
	}

	backend::ref( ).set_render_targets( &*m_renderer_context->get_rt( rt_generic_0 ), &*m_renderer_context->get_rt( rt_generic_1 ), 0, 0 );
	backend::ref( ).clear_render_targets( math::color( 0.f, 0.f, 0.f, 0.f ) );









	fill_opaque_models			( );

	backend::ref( ).disable_DrawIndexed = false;

	execute_stages				( );


	backend::ref( ).reset_depth_stencil_target( );
	backend::ref( ).set_render_targets( &*m_renderer_context->get_rt( rt_present ), 0, 0, 0 );
	scene->flush				( on_draw_scene, true, false );
#ifndef MASTER_GOLD
	// claude@NOTE: the non-MASTER_GOLD view-mode debug pipeline occupied target lines
	// 1003-1209; it is compiled out of the shipped build and was never recovered.
#endif // #ifndef MASTER_GOLD
	temporal_jitterer.pop_jittering( );

	draw_debug					( scene, view, frame_time, default_font );



	backend::ref( ).flush_rt_shader_resources( );
	scene->flush				( on_draw_scene, true, false );





	backend::ref( ).reset_depth_stencil_target( );
	backend::ref( ).clear_depth_stencil( D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.f, 0 );


	present						( output_window, viewport );

	if ( static_cast_checked< render_output_window* >( output_window.c_ptr( ) ) )
	{



		if ( s_ui_enabled && static_cast_checked< render_output_window* >( output_window.c_ptr( ) )->m_flash_renderer )
		{



			scene_view const* s	= m_renderer_context->scene_view( );

			survarium::flash_text_manager* text_manager = s->m_flash_text_manager;


			vector< survarium::flash_movie* > movies_vec;
			for ( u32 i = 0; i < s->m_flash_movies.size( ); ++i )
				movies_vec.push_back( s->m_flash_movies[i]->movie );

			backend::ref( ).reset_depth_stencil_target( );
			backend::ref( ).clear_depth_stencil( D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.f, 0 );
			backend::ref( ).flush( );

			static_cast_checked< render_output_window* >( output_window.c_ptr( ) )->m_flash_renderer->present( movies_vec.begin( ), movies_vec.size( ), text_manager );
		}



		backend::ref( ).flush_rt_shader_resources( );
		scene->flush			( on_draw_scene, false, true );


		device::ref( ).m_device_removed = false;

		static_cast_checked< render_output_window* >( output_window.c_ptr( ) )->render_output( )->present( );
	}

	if ( s_use_gpu_sync_value )
	{
		m_frame_sync_event->wait( );
		m_frame_sync_event->issue( );
	}

	m_last_frame_time			= m_current_time;

	scene->unmove_all_models	( );

	if ( s_draw_frame_histogram_value )
	{
		if ( m_fps_history.size( ) >= 512 )
		{
			frame_histogram_info* old_info = m_fps_history.pop_front( );
			DELETE				( old_info );
		}

		frame_histogram_info* const info = NEW( frame_histogram_info );
		info->time				= math::clamp_r( 1000.f / math::max( float( statistics::ref( ).general_stat_group.fps.value ), 1.f ), 0.f, 1000.f );
		info->mem				= 0.f;
		info->dips				= math::max( statistics::ref( ).visibility_stat_group.num_draw_calls.value, 1 );
		m_fps_history.push_back	( info );
	}

	if ( s_dump_scene_statistics_value )
	{
		s_dump_scene_statistics_value = false;
		scene->dump_scene_statistics( );
	}
}

// claude@NOTE: the remaining unreproduced guards are the ones whose callee is still an empty
// stub, so LTCG deletes the guard with it: draw_frame_histogram and
// culling::portal_sector_system::render (reached through scene::draw_portal_system). The
// render_target_ptr temp is a second, non-steerable gap - the target inlines the intrusive
// refcount ops that we emit as calls to
// threading::single_threading_policy::increment/decrement. The grass row carries an extra
// cmp/je because our LTCG partial-inlines grass_world::render_debug's entry test to here.
void renderer::draw_debug(
	scene*				scene,
	scene_view*			view,
	const float			frame_time,
	vostok::ui::font const*		default_font
)
{
	backend::ref( ).disable_DrawIndexed		= false;

	backend::ref( ).reset_depth_stencil_target( );

	backend::ref( ).set_render_targets( &*m_renderer_context->get_rt( rt_present ), 0, 0, 0 );

	if ( m_picking_lighting_luminance_mode && default_font )
		draw_luminance_picker_info			( default_font );

	if ( s_do_stages_profiling && default_font )
		draw_stages_stats					( default_font );

	u32 num_vs_changes		= backend::ref( ).num_vs_changes;
	u32 num_ps_changes		= backend::ref( ).num_ps_changes;
	u32 num_il_changes		= backend::ref( ).num_il_changes;
	u32 num_vsc_changes		= backend::ref( ).num_vsc_changes;
	u32 num_vst_changes		= backend::ref( ).num_vst_changes;
	u32 num_vss_changes		= backend::ref( ).num_vss_changes;
	u32 num_psc_changes		= backend::ref( ).num_psc_changes;
	u32 num_pst_changes		= backend::ref( ).num_pst_changes;
	u32 num_pss_changes		= backend::ref( ).num_pss_changes;

	statistics::ref( ).debug_stat_group.textures_compression_duration.value		= backend::ref( ).m_texture_compression_time;
	statistics::ref( ).debug_stat_group.dxt_rt_tex_creation_duration.value		= backend::ref( ).m_dxt_rt_tex_creation_time;
	statistics::ref( ).debug_stat_group.cpu_textures_compression_duration.value	= backend::ref( ).m_cpu_compression_time;
	statistics::ref( ).debug_stat_group.gpu_num_compressed_textures.value		= backend::ref( ).m_gpu_num_compressed_textures;
	statistics::ref( ).debug_stat_group.cpu_num_compressed_textures.value		= backend::ref( ).m_cpu_num_compressed_textures;

	statistics::ref( ).debug_stat_group.num_vertex_shader_changes.value			= num_vs_changes;
	statistics::ref( ).debug_stat_group.num_pixel_shader_changes.value			= num_ps_changes;
	statistics::ref( ).debug_stat_group.num_vs_textures_changes.value			= num_vst_changes;
	statistics::ref( ).debug_stat_group.num_vs_constants_changes.value			= num_vsc_changes;
	statistics::ref( ).debug_stat_group.num_vs_samplers_changes.value			= num_vss_changes;
	statistics::ref( ).debug_stat_group.num_ps_textures_changes.value			= num_pst_changes;
	statistics::ref( ).debug_stat_group.num_ps_constants_changes.value			= num_psc_changes;
	statistics::ref( ).debug_stat_group.num_ps_samplers_changes.value			= num_pss_changes;
	statistics::ref( ).debug_stat_group.num_input_layout_changes.value			= num_il_changes;

	statistics::ref( ).general_stat_group.cpu_render_frame_time.value			= frame_time;

	statistics::ref( ).visibility_stat_group.num_total_rendered_triangles.value	= backend::ref( ).num_total_rendered_triangles;
	statistics::ref( ).visibility_stat_group.num_total_rendered_points.value	= backend::ref( ).num_total_rendered_points;

	const double es2 = frame_time;

	statistics::ref( ).general_stat_group.render_frame_time.cpu_time.value		= es2 * 1000.;
	statistics::ref( ).general_stat_group.render_frame_time.gpu_time.value		= es2 * 1000.;

	statistics::ref( ).general_stat_group.fps.value			= math::floor( es2 > 0. ? 1. / es2 : 0. );
	statistics::ref( ).general_stat_group.cpu_fps.value		= math::floor( es2 > 0. ? 1. / es2 : 0. );

	statistics::ref( ).general_stat_group.num_setted_shader_constants.value		= backend::ref( ).num_setted_shader_constants;
	statistics::ref( ).visibility_stat_group.num_draw_calls.value				= backend::ref( ).num_draw_calls;
	statistics::ref( ).general_stat_group.render_only_time.value				= 0.;
	statistics::ref( ).debug_stat_group.texture_video_memory.value				= resource_manager::ref( ).get_texture_video_memory_size( );
	statistics::ref( ).debug_stat_group.avaliable_video_memory.value			= device::ref( ).get_avaliable_video_memory( );
	statistics::ref( ).debug_stat_group.gbuffer_video_memory.value				= m_renderer_context->m_targets->memory_usage( ) >> 20;
	statistics::ref( ).debug_stat_group.render_tergets_video_memory.value		=
		( resource_manager::ref( ).get_render_target_video_memory( ) >> 20 ) -
		statistics::ref( ).debug_stat_group.gbuffer_video_memory.value;

	if ( m_stage_debug && view->editor_debug_mode )
		m_stage_debug->execute				( );

	if ( m_stages[sun_shadows_accumulate_render_stage] )
		m_stages[sun_shadows_accumulate_render_stage]->m_context->set_w_identity( );

	if ( m_visibility_stage )
		m_visibility_stage->debug_render	( );

	if ( s_draw_frame_histogram_value )
		draw_frame_histogram				( );

	scene->draw_portal_system(
		system_renderer::ref( ),
		m_renderer_context->get_view_pos( ),
		m_renderer_context->get_vp( )
	);

	if ( m_stages[lighting_render_stage] )
		m_stages[lighting_render_stage]->debug_render( );

	if ( scene->get_grass( ) )
		scene->get_grass( )->render_debug	( m_renderer_context );

	if ( m_stages[decals_accumulate_render_stage] )
		m_stages[decals_accumulate_render_stage]->debug_render( );
}

// claude@NOTE: residual cause - the target inlines draw_text_shadowed (and math::color_rgba
// with its four math::floor expansions) at every call site here and in
// draw_luminance_picker_info, ~0x250 bytes per site; our LTCG emits a plain call, and it
// also leaves fixed_string<32>::fixed_string<32> out-of-line where the target inlines it.
// The statement shape below is complete - the gap is the inliner, not the source.
void renderer::draw_luminance_picker_info( vostok::ui::font const* default_font )
{
	fixed_string< 64 > strings[8];
	math::color rgbl_colors[4]				= {
		math::color( math::color_rgba( 0.5f, 1.f, 0.5f, 1.f ) ),
		math::color( math::color_rgba( 0.5f, 0.5f, 1.f, 1.f ) ),
		math::color( math::color_rgba( 1.f, 0.5f, 0.5f, 1.f ) ),
		math::color( math::color_rgba( 1.f, 1.f, 1.f, 1.f ) )
	};
	const float lum_diffuse					= m_debug_readed_data[0].x * 0.2125f + m_debug_readed_data[0].y * 0.7154f + m_debug_readed_data[0].z * 0.0721f;
	const float lum_final					= m_debug_readed_data[1].x * 0.2125f + m_debug_readed_data[1].y * 0.7154f + m_debug_readed_data[1].z * 0.0721f;

	strings[0].assignf						( "r: %f", m_debug_readed_data[0].x );
	strings[1].assignf						( "g: %f", m_debug_readed_data[0].y );
	strings[2].assignf						( "b: %f", m_debug_readed_data[0].z );
	strings[3].assignf						( "lum: %f", lum_diffuse );

	strings[4].assignf						( "r: %f", m_debug_readed_data[1].x );
	strings[5].assignf						( "g: %f", m_debug_readed_data[1].y );
	strings[6].assignf						( "b: %f", m_debug_readed_data[1].z );
	strings[7].assignf						( "lum: %f", lum_final );

	draw_text_shadowed	( default_font, "hdr diffuse lighting only:", 5, 5, rgbl_colors[3].m_value );

	for ( u32 i = 0; i < 4; ++i )
		draw_text_shadowed	( default_font, strings[i].get_buffer( ), 5, 17 + i * 12, rgbl_colors[i].m_value );

	draw_text_shadowed	( default_font, "hdr final scene:", 5, 70, math::color_rgba( 1.f, 1.f, 1.f, 1.f ) );

	for ( u32 i = 0; i < 4; ++i )
		draw_text_shadowed	( default_font, strings[4 + i].get_buffer( ), 5, 82 + i * 12, rgbl_colors[i].m_value );
}

void renderer::draw_stages_stats( vostok::ui::font const* default_font )
{
	double total_gpu_time					= 0.;
	double total_cpu_time					= 0.;
	u32 total_dips							= 0;

	fixed_string< 32 > total_result_string_gpu_time;
	fixed_string< 32 > total_result_string_cpu_time;
	fixed_string< 32 > total_result_string_dips;

	u32 string_index						= 0;

	for ( u32 stage_index = 0; stage_index < num_render_stages + 1; ++stage_index )
	{
		stage_stat const& stat				= stage_index == num_render_stages ? s_visibility_stage_stats : s_render_stages[stage_index];

		if ( !stat.stg )
			continue;

		fixed_string< 32 > result_string_gpu_time;
		fixed_string< 32 > result_string_cpu_time;
		fixed_string< 32 > result_string_dips;

		result_string_gpu_time.assignf		( "all: %4.4f", stat.average_time( true ) );
		result_string_cpu_time.assignf		( "cpu: %4.4f", stat.average_time( false ) );
		result_string_dips.assignf			( "dips: %d", stat.average_dips( ) );

		const u32 char_color				= string_index & 1 ?
			math::color_rgba( 0.75f, 1.f, 0.75f, 1.f ) :
			math::color_rgba( 0.5f, 0.75f, 1.f, 1.f );

		draw_text_shadowed	( default_font, render_stage_names[stage_index], 5, string_index * 12 + 5, char_color );
		draw_text_shadowed	( default_font, result_string_gpu_time.c_str( ), 201, string_index * 12 + 5, char_color );
		draw_text_shadowed	( default_font, result_string_cpu_time.c_str( ), 285, string_index * 12 + 5, char_color );
		draw_text_shadowed	( default_font, result_string_dips.c_str( ), 369, string_index * 12 + 5, char_color );



		total_gpu_time						+= stat.average_time( true );

		total_cpu_time						+= stat.average_time( false );
		total_dips							+= stat.average_dips( );

		++string_index;
	}

	const u32 y_pos							= string_index * 12 + 5;

	const u32 char_color					= math::color_rgba( 0.5f, 1.f, 0.5f, 1.f );

	total_result_string_gpu_time.assignf	( "all: %4.4f", total_gpu_time );
	total_result_string_cpu_time.assignf	( "cpu: %4.4f", total_cpu_time );
	total_result_string_dips.assignf		( "dips: %d", total_dips );

	draw_text_shadowed	( default_font, "total", 5, y_pos, char_color );
	draw_text_shadowed	( default_font, total_result_string_gpu_time.c_str( ), 201, y_pos, char_color );
	draw_text_shadowed	( default_font, total_result_string_cpu_time.c_str( ), 285, y_pos, char_color );
	draw_text_shadowed	( default_font, total_result_string_dips.c_str( ), 369, y_pos, char_color );
}

void renderer::present(
	base_output_window_ptr				in_output_window,
	math::rectangle< float2 > const&	viewport
)
{
	// FUNCTION BODY[0x648b10]: 6
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x648b10>|0x000|+0x02c:'1519'
	// ******

	render_output_window* const output_window = static_cast_checked< render_output_window* >( in_output_window.c_ptr( ) );
	setup_render_output_window	( in_output_window, viewport );

	// Present the final image to base render target
	m_present_stage->execute	( m_renderer_context->get_t( rt_present ) );

	if ( output_window )
	{
		if ( output_window->m_flash_renderer )
			output_window->m_flash_renderer->present( 0, 0, 0 );

		output_window->render_output( )->present( );
	}

	backend::ref( ).reset_depth_stencil_target( );
	backend::ref( ).clear_depth_stencil( D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.0f, 0 );

	backend::ref( ).reset		( );
	backend::ref( ).flush		( );

	m_last_frame_time			= m_current_time;
}

// claude@NOTE: the three float scales are loop-invariant and get hoisted into the
// loop preheader, so only screen_width/screen_height keep their own statements; the
// dips scale never needs a frame slot because its factor stays on the x87 stack
// (info->dips goes through fild) while the time scale is reloaded into SSE.
// claude@NOTE: residual cause - the target folds the inner `m_render_output ? ... : 0`
// out of backend::target_width/target_height (the guard above already proves it
// non-null) and reuses the guard's register for `->width()`/`->height()`; our base
// re-tests through a different expression, which adds 4 CFG blocks and ~0x1a bytes
// (sema: base 26 blocks vs target 22, first skeleton divergence at B3). Those
// accessors live in render/core/dx11/backend_inline.h, outside this lane's scope.
void renderer::draw_frame_histogram( ) const
{
	if ( !backend::ref( ).m_render_output )
		return;

	float const screen_width	= float( backend::ref( ).target_width( ) );
	float const screen_height	= float( backend::ref( ).target_height( ) );

	float const scale_x			= screen_width / 512.f;
	float const scale_time_y	= screen_height / 100.f;
	float const scale_dips_y	= screen_height / 10000.f;

	float3 lines_time[512];
	float3 lines_dips[512];

	u32 count					= 0;
	for ( frame_histogram_info* info = m_fps_history.front( ); info; info = info->next )
	{
		lines_time[count]		= float3(
			count * scale_x * 2.f / screen_width - 1.f,
			info->time * scale_time_y * 2.f / screen_height - 1.f,
			0.f
		);

		lines_dips[count]		= float3(
			count * scale_x * 2.f / screen_width - 1.f,
			info->dips * scale_dips_y * 2.f / screen_height - 1.f,
			0.f
		);
		++count;
	}

	if ( count > 1 )
	{
		system_renderer::ref( ).draw_screen_lines( lines_time, count, math::color( math::color_rgba( 0.f, 1.f, 0.f, 1.f ) ), 1, 0xffffffff, true, true );
		system_renderer::ref( ).draw_screen_lines( lines_dips, count, math::color( math::color_rgba( .5f, .5f, .7f, 1.f ) ), 1, 0xffffffff, true, true );
	}
}

	// TYPEDEFS
	// typedef
	// 	pbyte
	// 	iterator_type;

	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	u16*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::effect_compiler::shader_cache_info*
	// 	iterator_type;

	// typedef
	// 	vostok::render::geometry_batch*
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
	// 	vostok::render::stage**
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// typedef
	// 	vostok::render::ui::vertex*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok

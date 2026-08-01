#include "pch.h"
#include "renderer.h"

#include <vostok/console_command.h>
#include <vostok/particle/world.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/res_effect.h>
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

// claude@NOTE: the console-command string of s_debug_use_skeletel_mesh_lods_cc could not be
// read back - the object sits past the last one whose `dynamic initializer` was dumped.
static bool s_debug_use_skeletel_mesh_lods_value = true;
static console_commands::cc_bool s_debug_use_skeletel_mesh_lods_cc(
	"r_use_skeletel_mesh_lods",
	s_debug_use_skeletel_mesh_lods_value,
	false,
	console_commands::command_type_user_specific
);

struct stage_stat {
	double average_time( bool gpu_time ) const
	{
		return gpu_time ? elapsed_gpu_msec[0] : elapsed_cpu_msec[0];
	}

	u32 average_dips( ) const
	{
		return dips[0];
	}

	double elapsed_gpu_msec[1];
	double elapsed_cpu_msec[1];
	u32 dips[1];
	stage* stg;
};

static stage_stat s_visibility_stage_stats;

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

	DELETE					( m_view_mode_stage );
	DELETE					( m_present_stage );
	DELETE					( m_stage_debug );

	for ( std::reverse_iterator< stage** > it = m_stages.rbegin( ), e = m_stages.rend( ); it != e; ++it )
		DELETE				( *it );
}

// claude@NOTE: no legacy ancestor - absent from the legacy renderer.cpp remainder; matcher-phase work.
// STATE[STUB]
void renderer::recreate_stage( enum_render_stage_type arg_0 )
{
	// STATICS
	// static < NoType > 				 = <0x64beb0>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63a5ff }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN24") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63a654 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN23") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63a6ad }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN22") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63a704 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN21") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63a75e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN20") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63a7c3 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN19") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63a81d }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN18") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63a879 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN17") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63a8d5 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN16") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63a93c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN15") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63a99a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN14") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63a9f6 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN13") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63aa50 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN12") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63aaaa }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN11") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63ab14 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN10") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63ab70 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63abcf }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63ac2b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63ac85 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63ace4 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63ad41 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63ad9b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63adf9 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63ae53 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

	// FUNCTION BODY[0x64b5f0]: 27
	// <0x64b5f8>|0x008|+0x007:'285'
	// <0>
	// <0x64b5ff>|0x00f|+0x044:'287'
	// <0x64b643>|0x053|+0x011:'287'
	// <0x64b654>|0x064|+0x047:'288'
	// <0x64b69b>|0x0ab|+0x012:'288'
	// <0x64b6ad>|0x0bd|+0x045:'289'
	// <0x64b6f2>|0x102|+0x012:'289'
	// <0x64b704>|0x114|+0x048:'290'
	// <0x64b74c>|0x15c|+0x012:'290'
	// <0x64b75e>|0x16e|+0x053:'291'
	// <0x64b7b1>|0x1c1|+0x012:'291'
	// <0x64b7c3>|0x1d3|+0x048:'292'
	// <0x64b80b>|0x21b|+0x012:'292'
	// <0x64b81d>|0x22d|+0x04a:'293'
	// <0x64b867>|0x277|+0x012:'293'
	// <0x64b879>|0x289|+0x04a:'294'
	// <0x64b8c3>|0x2d3|+0x012:'294'
	// <0x64b8d5>|0x2e5|+0x055:'295'
	// <0x64b92a>|0x33a|+0x012:'295'
	// <0x64b93c>|0x34c|+0x04c:'296'
	// <0x64b988>|0x398|+0x012:'296'
	// <0x64b99a>|0x3aa|+0x04a:'297'
	// <0x64b9e4>|0x3f4|+0x012:'297'
	// <0x64b9f6>|0x406|+0x048:'298'
	// <0x64ba3e>|0x44e|+0x012:'298'
	// <0x64ba50>|0x460|+0x048:'299'
	// <0x64ba98>|0x4a8|+0x012:'299'
	// <0x64baaa>|0x4ba|+0x058:'300'
	// <0x64bb02>|0x512|+0x012:'300'
	// <0x64bb14>|0x524|+0x04a:'301'
	// <0x64bb5e>|0x56e|+0x012:'301'
	// <0x64bb70>|0x580|+0x04d:'302'
	// <0x64bbbd>|0x5cd|+0x012:'302'
	// <0x64bbcf>|0x5df|+0x04a:'303'
	// <0x64bc19>|0x629|+0x012:'303'
	// <0x64bc2b>|0x63b|+0x048:'304'
	// <0x64bc73>|0x683|+0x012:'304'
	// <0x64bc85>|0x695|+0x04d:'305'
	// <0x64bcd2>|0x6e2|+0x012:'305'
	// <0x64bce4>|0x6f4|+0x04b:'306'
	// <0x64bd2f>|0x73f|+0x012:'306'
	// <0x64bd41>|0x751|+0x048:'307'
	// <0x64bd89>|0x799|+0x012:'307'
	// <0x64bd9b>|0x7ab|+0x04c:'308'
	// <0x64bde7>|0x7f7|+0x012:'308'
	// <0x64bdf9>|0x809|+0x048:'309'
	// <0x64be41>|0x851|+0x012:'309'
	// <0x64be53>|0x863|+0x04a:'310'
	// <0x64be9d>|0x8ad|-0x861:'310'
	// <0>
	// <0x64b63c>|0x04c|+0x011:'312'
	// <0x64b64d>|0x05d|+0x047:'312'
	// <0x64b694>|0x0a4|+0x012:'312'
	// <0x64b6a6>|0x0b6|+0x045:'312'
	// <0x64b6eb>|0x0fb|+0x012:'312'
	// <0x64b6fd>|0x10d|+0x048:'312'
	// <0x64b745>|0x155|+0x012:'312'
	// <0x64b757>|0x167|+0x053:'312'
	// <0x64b7aa>|0x1ba|+0x012:'312'
	// <0x64b7bc>|0x1cc|+0x048:'312'
	// <0x64b804>|0x214|+0x012:'312'
	// <0x64b816>|0x226|+0x04a:'312'
	// <0x64b860>|0x270|+0x012:'312'
	// <0x64b872>|0x282|+0x04a:'312'
	// <0x64b8bc>|0x2cc|+0x012:'312'
	// <0x64b8ce>|0x2de|+0x055:'312'
	// <0x64b923>|0x333|+0x012:'312'
	// <0x64b935>|0x345|+0x04c:'312'
	// <0x64b981>|0x391|+0x012:'312'
	// <0x64b993>|0x3a3|+0x04a:'312'
	// <0x64b9dd>|0x3ed|+0x012:'312'
	// <0x64b9ef>|0x3ff|+0x048:'312'
	// <0x64ba37>|0x447|+0x012:'312'
	// <0x64ba49>|0x459|+0x048:'312'
	// <0x64ba91>|0x4a1|+0x012:'312'
	// <0x64baa3>|0x4b3|+0x058:'312'
	// <0x64bafb>|0x50b|+0x012:'312'
	// <0x64bb0d>|0x51d|+0x04a:'312'
	// <0x64bb57>|0x567|+0x012:'312'
	// <0x64bb69>|0x579|+0x04d:'312'
	// <0x64bbb6>|0x5c6|+0x012:'312'
	// <0x64bbc8>|0x5d8|+0x04a:'312'
	// <0x64bc12>|0x622|+0x012:'312'
	// <0x64bc24>|0x634|+0x048:'312'
	// <0x64bc6c>|0x67c|+0x012:'312'
	// <0x64bc7e>|0x68e|+0x04d:'312'
	// <0x64bccb>|0x6db|+0x012:'312'
	// <0x64bcdd>|0x6ed|+0x04b:'312'
	// <0x64bd28>|0x738|+0x012:'312'
	// <0x64bd3a>|0x74a|+0x048:'312'
	// <0x64bd82>|0x792|+0x012:'312'
	// <0x64bd94>|0x7a4|+0x04c:'312'
	// <0x64bde0>|0x7f0|+0x012:'312'
	// <0x64bdf2>|0x802|+0x048:'312'
	// <0x64be3a>|0x84a|+0x012:'312'
	// <0x64be4c>|0x85c|+0x04a:'312'
	// <0x64be96>|0x8a6|+0x012:'312'
	// ******
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
	// LOCALS
	// stage** 							it
	// u32 								prev_draw_calls
	// ******

	// CALL SITE INFO
	// <0x6478c8> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x647810]: 46
	// <0x647819>|0x009|+0x00f:'425'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x647828>|0x018|+0x009:'430'
	// <0>
	// <1>
	// <0x647831>|0x021|+0x019:'433'
	// <0x64784a>|0x03a|+0x01f:'434'
	// <0>
	// <1>
	// <0x647869>|0x059|-0x016:'437'
	// <0x647853>|0x043|+0x0d8:'438'
	// <0x64792b>|0x11b|-0x0bb:'438'
	// <0>
	// <0x647870>|0x060|+0x002:'440'
	// <0>
	// <0x647872>|0x062|+0x003:'442'
	// <0>
	// <0x647875>|0x065|+0x008:'444'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x64787d>|0x06d|+0x016:'452'
	// <0>
	// <0x647893>|0x083|+0x02e:'454'
	// <0>
	// <1>
	// <0x6478c1>|0x0b1|+0x009:'457'
	// <0>
	// <0x6478ca>|0x0ba|+0x009:'459'
	// <0>
	// <0x6478d3>|0x0c3|+0x00d:'461'
	// <0x6478e0>|0x0d0|+0x012:'462'
	// <0>
	// <1>
	// <0x6478f2>|0x0e2|+0x01c:'465'
	// <0x64790e>|0x0fe|+0x009:'466'
	// <0>
	// <0x647917>|0x107|+0x02a:'468'
	// <0>
	// <1>
	// ******

	for ( stage** it = m_stages.begin( ); it != m_stages.end( ); ++it )
	{
		stage* current_stage = *it;

		if ( current_stage )
			current_stage->execute( );
	}
}

// claude@NOTE: no legacy ancestor - sort predicate postdates the legacy corpus; matcher-phase work.
// STATE[STUB]
bool sort_by_vs_predicate::operator()( render_surface_instance const* left, render_surface_instance const* right ) const
{
	return false;

	// FUNCTION BODY[0x64d60]: 7
	// <0x64d60>|0x000|+0x029:'481'
	// <0x64d89>|0x029|+0x028:'482'
	// <0>
	// <1>
	// <0x64db1>|0x051|+0x02c:'485'
	// <0>
	// <0x64ddd>|0x07d|+0x01b:'487'
	// ******
}

// claude@NOTE: no legacy ancestor - sort predicate postdates the legacy corpus; matcher-phase work.
// STATE[STUB]
bool sort_by_distance_predicate::operator()( render_surface_instance const* left, render_surface_instance const* right ) const
{
	// LOCALS
	// float3 							pos0
	// float3 							pos1
	// ******

	return false;

	// FUNCTION BODY[0x64e00]: 17
	// <0x64e03>|0x003|+0x00e:'501'
	// <0x64e11>|0x011|+0x00b:'502'
	// <0>
	// <0x64e1c>|0x01c|+0x021:'504'
	// <0x64e3d>|0x03d|+0x016:'505'
	// <0x64e53>|0x053|+0x016:'506'
	// <0>
	// <0x64e69>|0x069|+0x016:'508'
	// <0x64e7f>|0x07f|+0x016:'509'
	// <0x64e95>|0x095|+0x016:'510'
	// <0>
	// <0x64eab>|0x0ab|+0x03a:'512'
	// <0x64ee5>|0x0e5|+0x036:'513'
	// <0>
	// <1>
	// <2>
	// <0x64f1b>|0x11b|+0x028:'517'
	// <0x64f43>|0x143|-0x004:'517'
	// <0x64f3f>|0x13f|+0x006:'518'
	// ******
}

// claude@NOTE: no legacy ancestor - sort predicate postdates the legacy corpus; matcher-phase work.
// STATE[STUB]
bool sort_by_texture_predicate::operator()( render_surface_instance const* left, render_surface_instance const* right ) const
{
	return false;

	// FUNCTION BODY[0x64f50]: 7
	// <0x64f50>|0x000|+0x02c:'531'
	// <0x64f7c>|0x02c|+0x028:'532'
	// <0>
	// <1>
	// <0x64fa4>|0x054|+0x02c:'535'
	// <0>
	// <0x64fd0>|0x080|+0x021:'537'
	// ******
}

// claude@NOTE: no legacy ancestor - screen_factor postdates the legacy corpus; matcher-phase work.
// STATE[STUB]
float screen_factor( float3 const& view_position, math::aabb bbox, float4x4 const& model_transform )
{
	return 0.0f;

	// FUNCTION BODY[0x647730]: 8
	// <0x647730>|0x000|+0x009:'565'
	// <0>
	// <0x647739>|0x009|+0x023:'567'
	// <0x64775c>|0x02c|+0x036:'568'
	// <0x647792>|0x062|+0x04c:'569'
	// <0x6477de>|0x0ae|+0x02c:'570'
	// <0>
	// <1>
	// ******
}

// claude@NOTE: no legacy ancestor - absent from the legacy corpus (opaque-model gather is new-in-target); matcher-phase work.
// STATE[STUB]
void renderer::fill_opaque_models( )
{
	// LOCALS
	// vector< render_surface_instance* >& opaque_models
	// vector< render_surface_instance* > opaque_models_lod0
	// vector< render_surface_instance* > opaque_models_lod1
	// render_surface_instance* const* 	e
	// material_effects const& 			me
	// render_surface_instance* const* 	i
	// render_surface_instance* const* 	i
	// ******

	// CALL SITE INFO
	// <0x647c2b> -> math::aabb < unknown >()
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63712e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1177") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x636ffb }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1183") })
	// ******

	// FUNCTION BODY[0x647a00]: 72
	// <0x647a00>|0x000|+0x01e:'577'
	// <0>
	// <0x647a1e>|0x01e|+0x00c:'579'
	// <0>
	// <1>
	// <0x647a2a>|0x02a|+0x03a:'582'
	// <0>
	// <0x647a64>|0x064|+0x02c:'584'
	// <0>
	// <0x647a90>|0x090|+0x029:'586'
	// <0x647ab9>|0x0b9|+0x009:'587'
	// <0x647ac2>|0x0c2|+0x020:'588'
	// <0>
	// <0x647ae2>|0x0e2|+0x05c:'590'
	// <0>
	// <1>
	// <2>
	// <0x647b3e>|0x13e|+0x00c:'594'
	// <0>
	// <0x647b4a>|0x14a|+0x03e:'596'
	// <0>
	// <0x647b88>|0x188|+0x01a:'598'
	// <0>
	// <0x647ba2>|0x1a2|+0x02e:'600'
	// <0>
	// <0x647bd0>|0x1d0|+0x003:'602'
	// <0>
	// <0x647bd3>|0x1d3|+0x027:'604'
	// <0>
	// <0x647bfa>|0x1fa|+0x011:'606'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x647c0b>|0x20b|+0x11e:'613'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x647d29>|0x329|+0x024:'618'
	// <0>
	// <0x647d4d>|0x34d|+0x022:'620'
	// <0>
	// <1>
	// <2>
	// <0x647d6f>|0x36f|+0x02f:'624'
	// <0>
	// <0x647d9e>|0x39e|+0x005:'626'
	// <0>
	// <0x647da3>|0x3a3|+0x00a:'628'
	// <0x647dad>|0x3ad|+0x01b:'629'
	// <0x647dc8>|0x3c8|+0x005:'630'
	// <0x647dcd>|0x3cd|+0x031:'631'
	// <0>
	// <1>
	// <0x647dfe>|0x3fe|+0x013:'634'
	// <0x647e11>|0x411|+0x038:'635'
	// <0x647e49>|0x449|+0x00f:'636'
	// <0x647e58>|0x458|+0x044:'637'
	// <0>
	// <0x647e9c>|0x49c|+0x006:'639'
	// <0x647ea2>|0x4a2|+0x006:'640'
	// <0>
	// <0x647ea8>|0x4a8|+0x00c:'642'
	// <0>
	// <0x647eb4>|0x4b4|+0x010:'644'
	// <0x647ec4>|0x4c4|+0x12d:'645'
	// <0x647ff1>|0x5f1|-0x027:'645'
	// <0>
	// <0x647fca>|0x5ca|+0x012:'647'
	// <0x647fdc>|0x5dc|+0x01f:'648'
	// <0x647ffb>|0x5fb|-0x4c7:'648'
	// <0x647b34>|0x134|+0x5b4:'649'
	// ******
}

// claude@NOTE: no legacy ancestor - absent from the legacy corpus; matcher-phase work.
// STATE[STUB]
void renderer::sort_models_by_distance( vector< render_surface_instance* >& instances, bool from_near_to_far )
{
	// LOCALS
	// float3 							pos
	// ******

	// FUNCTION BODY[0x647950]: 16
	// <0x647953>|0x003|+0x020:'653'
	// <0>
	// <0x647973>|0x023|+0x011:'655'
	// <0x647984>|0x034|+0x016:'656'
	// <0x64799a>|0x04a|+0x016:'657'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x6479b0>|0x060|+0x03e:'668'
	// ******
}

// claude@NOTE: no legacy ancestor - absent from the legacy corpus; matcher-phase work.
// STATE[STUB]
void renderer::sort_models(
	vector< render_surface_instance* >&		instances,
	enum_render_stage_type					stage_type,
	const u32								tech_index
)
{
	// FUNCTION BODY[0x6476e0]: 30
	// <0x6476e0>|0x000|+0x014:'673'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6476f4>|0x014|+0x00f:'681'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x647703>|0x023|+0x009:'694'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x64770c>|0x02c|+0x00f:'702'
	// ******
}

void push_point(
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

void make_ui_vertices(
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

void draw_text(
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

void draw_text_shadowed(
	vostok::ui::font const*		in_font,
	pcstr				str,
	u32					pos_x,
	u32					pos_y,
	u32					clr
)
{
	// FUNCTION BODY[0x648c80]: 2
	draw_text(in_font, str, pos_x + 1, pos_y + 1, vostok::math::color(0, 0, 0, 220).m_value);
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
	backend::ref( ).num_vs_changes	=
	backend::ref( ).num_ps_changes	=
	backend::ref( ).num_il_changes	=
	backend::ref( ).num_vsc_changes	=
	backend::ref( ).num_vst_changes	=
	backend::ref( ).num_vss_changes	=
	backend::ref( ).num_psc_changes	=
	backend::ref( ).num_pst_changes	=
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

// claude@NOTE: no legacy ancestor - absent from the legacy corpus (debug overlay is new-in-target); matcher-phase work.
// STATE[STUB]
void renderer::draw_debug(
	scene*				scene,
	scene_view*			view,
	const float			frame_time,
	vostok::ui::font const*		default_font
)
{
	// LOCALS
	// u32 								num_vss_changes
	// u32 								num_pss_changes
	// u32 								num_pst_changes
	// const double 					es2
	// u32 								num_psc_changes
	// u32 								num_vst_changes
	// ******

	// CALL SITE INFO
	// <0x64b52b> -> void < unknown >()
	// <0x64b564> -> void < unknown >()
	// <0x64b5b1> -> void < unknown >()
	// <0x64b5d6> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x64b230]: 117
	// <0>
	// <1>
	// <2>
	// <0x64b23a>|0x00a|+0x006:'1299'
	// <0>
	// <0x64b240>|0x010|+0x025:'1301'
	// <0>
	// <0x64b265>|0x035|+0x37d:'1303'
	// <0x64b5e2>|0x3b2|-0x2ef:'1303'
	// <0>
	// <0x64b2f3>|0x0c3|+0x008:'1305'
	// <0x64b2fb>|0x0cb|+0x00e:'1306'
	// <0>
	// <1>
	// <0x64b309>|0x0d9|+0x00c:'1309'
	// <0x64b315>|0x0e5|+0x00c:'1310'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x64b321>|0x0f1|+0x003:'1317'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <0x64b324>|0x0f4|+0x044:'1331'
	// <0x64b368>|0x138|+0x013:'1332'
	// <0x64b37b>|0x14b|+0x013:'1333'
	// <0x64b38e>|0x15e|+0x006:'1334'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <0x64b394>|0x164|+0x06d:'1349'
	// <0>
	// <0x64b401>|0x1d1|+0x009:'1351'
	// <0x64b40a>|0x1da|+0x003:'1352'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x64b40d>|0x1dd|+0x014:'1357'
	// <0x64b421>|0x1f1|+0x008:'1358'
	// <0>
	// <0x64b429>|0x1f9|+0x030:'1360'
	// <0x64b459>|0x229|+0x033:'1361'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x64b48c>|0x25c|+0x015:'1367'
	// <0x64b4a1>|0x271|+0x01c:'1368'
	// <0x64b4bd>|0x28d|+0x006:'1369'
	// <0x64b4c3>|0x293|+0x029:'1370'
	// <0x64b4ec>|0x2bc|+0x01d:'1371'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x64b509>|0x2d9|+0x017:'1381'
	// <0x64b520>|0x2f0|+0x00d:'1382'
	// <0>
	// <0x64b52d>|0x2fd|+0x00c:'1384'
	// <0>
	// <0x64b539>|0x309|+0x017:'1386'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x64b550>|0x320|+0x009:'1392'
	// <0x64b559>|0x329|+0x00d:'1393'
	// <0>
	// <0x64b566>|0x336|+0x009:'1395'
	// <0x64b56f>|0x33f|+0x006:'1396'
	// <0>
	// <0x64b575>|0x345|+0x026:'1398'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x64b59b>|0x36b|+0x00c:'1404'
	// <0x64b5a7>|0x377|+0x00c:'1405'
	// <0>
	// <0x64b5b3>|0x383|+0x00a:'1407'
	// <0x64b5bd>|0x38d|+0x005:'1408'
	// <0>
	// <0x64b5c2>|0x392|+0x00c:'1410'
	// <0x64b5ce>|0x39e|+0x00a:'1411'
	// <0>
	// ******
}

// claude@NOTE: no legacy ancestor - absent from the legacy corpus; matcher-phase work.
// STATE[STUB]
void renderer::draw_luminance_picker_info( vostok::ui::font const* default_font )
{
	// LOCALS
	// fixed_string< 64 >[8] 			strings
	// const float 						lum_final
	// math::color[4] 					rgbl_colors
	// const float 						lum_diffuse
	// ******

	// FUNCTION BODY[0x64a5a0]: 29
	// <0x64a5ac>|0x00c|+0x07b:'1417'
	// <0>
	// <0x64a627>|0x087|+0x023:'1419'
	// <0x64a64a>|0x0aa|+0x027:'1420'
	// <0x64a671>|0x0d1|+0x027:'1421'
	// <0x64a698>|0x0f8|+0x021:'1422'
	// <0>
	// <0x64a6b9>|0x119|+0x04a:'1424'
	// <0x64a703>|0x163|+0x032:'1425'
	// <0>
	// <0x64a735>|0x195|+0x02e:'1427'
	// <0x64a763>|0x1c3|+0x025:'1428'
	// <0x64a788>|0x1e8|+0x025:'1429'
	// <0x64a7ad>|0x20d|+0x020:'1430'
	// <0>
	// <0x64a7cd>|0x22d|+0x025:'1432'
	// <0x64a7f2>|0x252|+0x025:'1433'
	// <0x64a817>|0x277|+0x025:'1434'
	// <0x64a83c>|0x29c|+0x020:'1435'
	// <0>
	// <0x64a85c>|0x2bc|+0x060:'1437'
	// <0>
	// <1>
	// <0x64a8bc>|0x31c|+0x25f:'1440'
	// <0>
	// <0x64ab1b>|0x57b|+0x4a5:'1442'
	// <0>
	// <1>
	// <0x64afc0>|0xa20|+0x25c:'1445'
	// ******
}

// claude@NOTE: no legacy ancestor - absent from the legacy corpus; matcher-phase work.
// STATE[STUB]
void renderer::draw_stages_stats( vostok::ui::font const* default_font )
{
	// LOCALS
	// double 							total_cpu_time
	// double 							total_gpu_time
	// u32 								total_dips
	// fixed_string< 32 > 				total_result_string_cpu_time
	// fixed_string< 32 > 				total_result_string_gpu_time
	// fixed_string< 32 > 				total_result_string_dips
	// const u32 						y_pos
	// u32 								string_index
	// const u32 						char_color
	// u32 								stage_index
	// fixed_string< 32 > 				result_string_dips
	// fixed_string< 32 > 				result_string_gpu_time
	// fixed_string< 32 > 				result_string_cpu_time
	// const u32 						char_color
	// ******

	// FUNCTION BODY[0x648cd0]: 60
	// <0>
	// <1>
	// <0x648cd6>|0x006|+0x01d:'1452'
	// <0x648cf3>|0x023|+0x009:'1453'
	// <0x648cfc>|0x02c|+0x004:'1454'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x648d00>|0x030|+0x00f:'1462'
	// <0>
	// <1>
	// <0x648d0f>|0x03f|+0x020:'1465'
	// <0>
	// <0x648d2f>|0x05f|+0x00a:'1467'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x648d39>|0x069|+0x0a1:'1474'
	// <0x648dda>|0x10a|+0x01b:'1475'
	// <0x648df5>|0x125|+0x048:'1476'
	// <0>
	// <1>
	// <2>
	// <0x648e3d>|0x16d|+0x0a3:'1480'
	// <0>
	// <1>
	// <0x648ee0>|0x210|+0x262:'1483'
	// <0x649142>|0x472|+0x298:'1484'
	// <0x6493da>|0x70a|+0x261:'1485'
	// <0x64963b>|0x96b|+0x26a:'1486'
	// <0>
	// <1>
	// <2>
	// <0x6498a5>|0xbd5|+0x008:'1490'
	// <0>
	// <0x6498ad>|0xbdd|+0x085:'1492'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x649932>|0xc62|+0x259:'1500'
	// <0>
	// <0x649b8b>|0xebb|+0x07e:'1502'
	// <0x649c09>|0xf39|+0x01f:'1503'
	// <0x649c28>|0xf58|+0x017:'1504'
	// <0>
	// <0x649c3f>|0xf6f|+0x245:'1506'
	// <0x649e84>|0x11b4|+0x257:'1507'
	// <0x64a0db>|0x140b|+0x25b:'1508'
	// <0x64a336>|0x1666|+0x25d:'1509'
	// ******
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

// claude@NOTE: no legacy ancestor - absent from the legacy corpus; matcher-phase work.
// STATE[STUB]
void renderer::draw_frame_histogram( ) const
{
	// LOCALS
	// float3[512] 						lines_time
	// float3[512] 						lines_dips
	// const float 						screen_width
	// const float 						scale_x
	// const float 						scale_time_y
	// const float 						screen_height
	// ******

	// FUNCTION BODY[0x6488c0]: 38
	// <0x6488ca>|0x00a|+0x016:'1619'
	// <0>
	// <1>
	// <0x6488e0>|0x020|+0x028:'1622'
	// <0x648908>|0x048|+0x021:'1623'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x648929>|0x069|+0x03c:'1635'
	// <0x648965>|0x0a5|+0x008:'1636'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x64896d>|0x0ad|+0x06d:'1642'
	// <0x6489da>|0x11a|-0x01a:'1642'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6489c0>|0x100|+0x026:'1647'
	// <0x6489e6>|0x126|+0x044:'1647'
	// <0x648a2a>|0x16a|+0x005:'1648'
	// <0x648a2f>|0x16f|+0x036:'1649'
	// <0>
	// <1>
	// <0x648a65>|0x1a5|+0x094:'1652'
	// <0x648af9>|0x239|-0x08b:'1652'
	// <0>
	// <0x648a6e>|0x1ae|+0x034:'1654'
	// <0x648aa2>|0x1e2|+0x04b:'1655'
	// <0>
	// <0x648aed>|0x22d|+0x00e:'1657'
	// ******
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

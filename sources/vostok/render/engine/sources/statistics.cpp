#include "pch.h"
#include <vostok/render/core/backend.h>
#include <vostok/render/facade/sources/ui_renderer.h>
#include <vostok/ui/ui.h>
#include <vostok/console_command.h>
#include "statistics.h"

static bool s_short_statistics_value = false;
static vostok::console_commands::cc_bool s_short_statistics(
	"r_short_statistics",
	s_short_statistics_value,
	true,
	vostok::console_commands::command_type_user_specific
);

namespace vostok {
namespace render {

namespace ui {
struct vertex;
}

statistics_group::statistics_group( pcstr group_name ) :
	first_statistics( 0 ),
	m_name( group_name )
{
	m_next							= statistics::ref().first_group;
	statistics::ref().first_group	= this;
}

statistics_group::~statistics_group( )
{
}

void statistics_group::start( )
{
	for (statistics_base* it = first_statistics; it != 0; it = it->m_next)
	{
		it->start();
	}
}

static void push_point(
	vectora< ui::vertex >& out_vertices,
	u32 index,
	float x,
	float y,
	float z,
	u32 c,
	float u,
	float v
)
{
	vostok::render::ui::vertex& vertex_item	= out_vertices[index];
	vertex_item.m_position.set				(x, y, z, 1);
	vertex_item.m_uv.set					(u, v);
	vertex_item.m_color						= c;
}

void make_ui_vertices(
	vectora< ui::vertex >& out_vertices,
	pcstr in_text,
	vostok::ui::font const& in_font,
	float2 const& in_position,
	math::color const& in_color,
	math::color const& in_selection_color,
	u32 max_line_width,
	bool is_multiline,
	u32 start_selection_index,
	u32 end_selection_index
)
{
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

statistics_base::statistics_base( statistics_group* group, pcstr name ) :
	m_group( group )
{
	if ( group )
	{
		m_name					= name;
		m_next					= group->first_statistics;
		group->first_statistics = this;
	}
}

statistics_base::~statistics_base( )
{
}

void statistics_float::print( fs_new::virtual_path_string& out_result )
{
	out_result.assignf( "%s: %f (%f..%f)", m_name.c_str( ), value, min_value, max_value );
}

static u32 get_num_digits( u32 v )
{
	math::clamp( v, u32( 0 ), u32( 10000000 ) );

	u32 result = 0;
	for ( u32 divider = 1; v / divider && divider < 1000000; divider *= 10 )
		++result;

	return result;
}

fixed_string< 260 > u32_to_string( u32 v )
{
	fixed_string< 260 > result;
	result.assignf( "%d", v );
	return result;
}

void statistics_int::print( fs_new::virtual_path_string& out_result )
{
	value_num_max_digits = u8( math::max( 1u, math::max( u32( value_num_max_digits ), get_num_digits( value ) ) ) );
	max_value_num_max_digits = u8( math::max( 1u, math::max( u32( max_value_num_max_digits ), get_num_digits( max_value ) ) ) );
	min_value_num_max_digits = u8( math::max( 1u, math::max( u32( min_value_num_max_digits ), get_num_digits( min_value ) ) ) );

	fixed_string< 260 > rule( "%s: " );
	rule += "%";
	rule += u32_to_string( value_num_max_digits );
	rule += "d (";
	rule += "%";
	rule += u32_to_string( min_value_num_max_digits );
	rule += "d..";
	rule += "%";
	rule += u32_to_string( min_value_num_max_digits );
	rule += "d)";

	out_result.assignf( rule.c_str( ), m_name.c_str( ), value, min_value, max_value );
}

void statistics_cpu_gpu::print( fs_new::virtual_path_string& out_result )
{
	out_result.assignf( "%s: CPU:%.4f(%.4f..%.4f), GPU:%.4f", m_name.c_str( ), cpu_time.value, cpu_time.min_value, cpu_time.max_value, gpu_time.value );
}

void statistics_cpu_gpu::start( )
{
	cpu_time.start();
	gpu_time.start();
}

gbuffer_statistics_group::gbuffer_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	pre_pass_execute_time( this, "pre pass execute time" ),
	material_pass_execute_time( this, "material pass execute time" )
{
}

visibility_statistics_group::visibility_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	portal_culling_time( this, "portals culling time" ),
	frustums_count( this, "frustums count" ),
	culling_time( this, "culling time" ),
	models_updating_time( this, "models updating time" ),
	num_draw_calls( this, "draw calls" ),
	num_triangles( this, "visible triangles" ),
	num_total_rendered_triangles( this, "total rendered triangles" ),
	num_total_rendered_points( this, "total rendered points" ),
	num_surfaces( this, "surfaces" ),
	num_lights( this, "lights" ),
	num_particle_instances( this, "particle instances" ),
	num_speedtree_instances( this, "total rendered speedtree instances" ),
	num_environment_probes( this, "environment probes" ),
	num_ambient_volumes( this, "ambient volumes" ),
	num_occlusion_culled_surfaces( this, "occlusion culled surfaces" ),
	num_occlusion_culled_lights( this, "occlusion culled lights" ),
	num_occlusion_culled_grass_patches( this, "occlusion culled grass patches" ),
	num_occlusion_culled_particle_instances( this, "occlusion culled particle instances" ),
	num_occlusion_culled_decals( this, "occlusion culled decals" ),
	num_occlusion_culled_env_probes( this, "occlusion culled environment probes" ),
	num_occlusion_culled_portals( this, "occlusion culled portals" ),
	num_occlusion_culled_ambient_volumes( this, "occlusion culled ambient volumes" )
{
}

lights_statistics_group::lights_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	accumulate_lighting_time( this, "accumulate lighting time" ),
	forward_lighting_time( this, "forward lighting time" ),
	shadow_map_time( this, "shadow map time" )
{
}

particles_statistics_group::particles_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	execute_time( this, "execute time" ),
	sprites_execute_time( this, "sprites execute time" ),
	beamtrails_execute_time( this, "beams and trails execute time" ),
	meshes_execute_time( this, "meshes execute time" ),
	num_total_instances( this, "total instances" )
{
}

cascaded_sun_shadow_statistics_group::cascaded_sun_shadow_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	execute_time_cascade_1( this, "execute time cascade # 1" ),
	execute_time_cascade_2( this, "execute time cascade # 2" ),
	execute_time_cascade_3( this, "execute time cascade # 3" ),
	execute_time_cascade_4( this, "execute time cascade # 4" ),
	num_dips_cascade_1( this, "dips in cascade # 1" ),
	num_dips_cascade_2( this, "dips in cascade # 2" ),
	num_dips_cascade_3( this, "dips in cascade # 3" ),
	num_dips_cascade_4( this, "dips in cascade # 4" ),
	num_dips( this, "dips" ),
	num_clipped_dips( this, "clipped dips" ),
	num_triangles( this, "triangles" )
{
}

postprocess_statistics_group::postprocess_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	execute_time( this, "execute time" )
{
}

forward_stage_statistics_group::forward_stage_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	execute_time( this, "execute time" )
{
}

general_statistics_group::general_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	render_frame_time( this, "render frame time" ),
	cpu_render_frame_time( this, "cpu render frame time" ),
	num_setted_shader_constants( this, "setted shader constants" ),
	fps( this, "FPS" ),
	cpu_fps( this, "FPS (CPU)" ),
	render_only_time( this, "render only time" )
{
}

speedtree_statistics_group::speedtree_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	render_time( this, "render time" ),
	culling_time( this, "culling time" ),
	num_instances( this, "instances" )
{
}

forward_decals_statistics_group::forward_decals_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	execute_time( this, "execute time" ),
	num_decals( this, "decals" ),
	num_decal_draw_calls( this, "decal draw calls" )
{
}

deferred_decals_statistics_group::deferred_decals_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	execute_time( this, "execute time" ),
	num_decals( this, "decals" ),
	num_decal_draw_calls( this, "decal draw calls" )
{
}

distortion_pass_statistics_group::distortion_pass_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	accumulate_time( this, "accumulate time" ),
	apply_time( this, "apply time" )
{
}

ssao_statistics_group::ssao_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	ssao_accumulate_time( this, "ssao accumulate time" ),
	ssao_blurring_time( this, "ssao blurring time" )
{
}

sky_statistics_group::sky_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	execute_time( this, "execute time" )
{
}

lpv_statistics_group::lpv_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	lpv_lookup_time( this, "LPV lookup time" ),
	propagation_time( this, "propagation time" ),
	gv_injection_time( this, "GV injection time" ),
	vpl_injection_time( this, "VPL injection time" ),
	rsm_downsample_time( this, "RSM downsample time" ),
	rsm_rendering_time( this, "RSM rendering time" ),
	num_dips( this, "dips" ),
	num_dips_in_cascade_0( this, "dips in cascade #1" ),
	num_dips_in_cascade_1( this, "dips in cascade #2" ),
	num_dips_in_cascade_2( this, "dips in cascade #3" ),
	num_clipped_dips( this, "clipped dips" )
{
}

grass_statistics_group::grass_statistics_group( pcstr group_name ) :
	statistics_group( group_name ),
	num_total_patches( this, "total patches" ),
	num_rendered_patches( this, "rendered patches" ),
	num_visible_patches( this, "visible patches" )
{
}

debug_statistics_group::debug_statistics_group( pcstr group_name ) :
	statistics_group( "debug statistics" ),
	avaliable_video_memory( this, "avaliable video memory(Mb)" ),
	texture_video_memory( this, "texture video memory(Mb)" ),
	render_tergets_video_memory( this, "other rt video memory(Mb)" ),
	gbuffer_video_memory( this, "G-Buffer video memory(Mb)" ),
	num_dips_in_lpv( this, "dips in lpv" ),
	num_vertex_shader_changes( this, "vertex shader changes" ),
	num_pixel_shader_changes( this, "pixel shader changes" ),
	num_vs_textures_changes( this, "vs textures changes" ),
	num_vs_constants_changes( this, "vs constants changes" ),
	num_vs_samplers_changes( this, "vs samplers changes" ),
	num_ps_textures_changes( this, "ps textures changes" ),
	num_ps_constants_changes( this, "ps constants changes" ),
	num_ps_samplers_changes( this, "ps samplers changes" ),
	num_input_layout_changes( this, "input layout changes" ),
	textures_compression_duration( this, "GPU compressor time" ),
	dxt_rt_tex_creation_duration( this, "GPU compressor RT create time" ),
	cpu_textures_compression_duration( this, "CPU compressor time" ),
	gpu_num_compressed_textures( this, "GPU num compressed textures" ),
	cpu_num_compressed_textures( this, "CPU num compressed textures" )
{
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
	m_max_string_width( 0 )
{
}

void statistics::start( )
{
	for (statistics_group* it = first_group; it != 0; it = it->m_next)
	{
		it->start();
	}

	backend::ref().num_total_rendered_triangles		= 0;
	backend::ref().num_total_rendered_points		= 0;
	backend::ref().num_setted_shader_constants		= 0;
	backend::ref().num_draw_calls					= 0;
}

statistics::~statistics( )
{
}

} // namespace render
} // namespace vostok

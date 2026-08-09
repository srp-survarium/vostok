#ifndef VOSTOK_RENDER_ENGINE_STATISTICS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STATISTICS_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/fixed_string.h>
#include <vostok/render/core/quasi_singleton.h>
#include <vostok/timing_timer.h>

namespace vostok {

namespace fs_new {
class virtual_path_string;
}

namespace ui {
struct font;
struct world;
}

namespace render {

struct statistics_base;

struct statistics_group : public boost::noncopyable {
	explicit statistics_group( pcstr group_name );
	~statistics_group( );

	u32 render( vostok::ui::world&, u32, u32 ) { return 0; }
	void start( );
	void clear( ) { }
	void calc_sizes( vostok::ui::font const*, u32&, u32& ) { }
	u32 get_num_text_lines( ) const { return 0; }

	statistics_base*	first_statistics;

private:
	friend class statistics;

	fixed_string< 128 >	m_name;
	statistics_group*	m_next;
};

STATIC_SIZE_ASSERT( statistics_group, 0x94 );

struct statistics_base : public boost::noncopyable {
	statistics_base( statistics_group* group, pcstr name );
	~statistics_base( );

	virtual void print( fs_new::virtual_path_string& out_result ) = 0;
	virtual void start( ) = 0;
	virtual void clear( ) { }

	u32 calc_max_string_width( vostok::ui::font const* ) { return 0; }

protected:
	friend struct statistics_group;

	fixed_string< 128 >	m_name;
	statistics_base*	m_next;
	statistics_group*	m_group;
};

STATIC_SIZE_ASSERT( statistics_base, 0x98 );

template < typename T >
struct statistics_value : public statistics_base {
	statistics_value( statistics_group* group, pcstr name );

	virtual void start( ) override;
	virtual void print( fs_new::virtual_path_string& out_result ) override = 0;
	virtual void clear( ) override;

	T average( );
	~statistics_value( );

	T	value;
	T	min_value;
	T	max_value;
	u8	value_num_max_digits;
	u8	min_value_num_max_digits;
	u8	max_value_num_max_digits;
	T	min_value_temp;
	T	max_value_temp;

protected:
	T	history[1];
	u32	history_index;
	u32	min_max_frame_index;
};

STATIC_SIZE_ASSERT( statistics_value< double >, 0xD8 );
STATIC_SIZE_ASSERT( statistics_value< int >, 0xBC );

struct statistics_float : public statistics_value< double > {
	statistics_float( statistics_group* group, pcstr name ) :
		statistics_value< double >( group, name )
	{
	}

	virtual void print( fs_new::virtual_path_string& out_result ) override;
	~statistics_float( ) { }
};

STATIC_SIZE_ASSERT( statistics_float, 0xD8 );

struct statistics_int : public statistics_value< int > {
	statistics_int( statistics_group* group, pcstr name ) :
		statistics_value< int >( group, name )
	{
	}

	virtual void print( fs_new::virtual_path_string& out_result ) override;
	~statistics_int( ) { }
};

STATIC_SIZE_ASSERT( statistics_int, 0xBC );

struct statistics_cpu_gpu : public statistics_base {
	statistics_cpu_gpu( statistics_group* group, pcstr name ) :
		statistics_base( group, name ),
		cpu_time( 0, 0 ),
		gpu_time( 0, 0 )
	{
	}

	virtual void start( ) override;
	virtual void print( fs_new::virtual_path_string& out_result ) override;
	~statistics_cpu_gpu( ) { }

	statistics_float	cpu_time;
	statistics_float	gpu_time;
};

STATIC_SIZE_ASSERT( statistics_cpu_gpu, 0x248 );

struct cpu_gpu_timer_scope : public boost::noncopyable {
	explicit cpu_gpu_timer_scope( statistics_cpu_gpu& counter ) :
		m_counter( counter )
	{
		// claude@NOTE: no legacy ancestor - legacy statistics.h consumed; its scope ctor used the retired USE_GPU_TIMERS gpu_timer member (recoverable from git); matcher-phase work.
		// STATE[STUB]
	}

	~cpu_gpu_timer_scope( )
	{
		// claude@NOTE: no legacy ancestor - legacy statistics.h consumed; retired gpu_timer layout; matcher-phase work.
		// STATE[STUB]
	}

	statistics_cpu_gpu&	m_counter;
	timing::timer		m_cpu_tumer;
};

STATIC_SIZE_ASSERT( cpu_gpu_timer_scope, 0x20 );

struct timer_scope : public boost::noncopyable {
	explicit timer_scope( statistics_float& counter ) :
		m_counter( counter )
	{
		// claude@NOTE: no legacy ancestor - legacy statistics.h consumed; retired layout; matcher-phase work.
		// STATE[STUB]
	}

	~timer_scope( )
	{
		// claude@NOTE: no legacy ancestor - legacy statistics.h consumed; retired layout; matcher-phase work.
		// STATE[STUB]
	}

	statistics_float&	m_counter;
	timing::timer		m_cpu_tumer;
};

STATIC_SIZE_ASSERT( timer_scope, 0x20 );

struct gbuffer_statistics_group : public statistics_group {
	explicit gbuffer_statistics_group( pcstr group_name );
	~gbuffer_statistics_group( ) { }

	statistics_cpu_gpu	pre_pass_execute_time;
	statistics_cpu_gpu	material_pass_execute_time;
};

STATIC_SIZE_ASSERT( gbuffer_statistics_group, 0x528 );

struct visibility_statistics_group : public statistics_group {
	explicit visibility_statistics_group( pcstr group_name );
	~visibility_statistics_group( ) { }

	statistics_float	portal_culling_time;
	statistics_int		frustums_count;
	statistics_float	culling_time;
	statistics_float	models_updating_time;
	statistics_int		num_draw_calls;
	statistics_int		num_triangles;
	statistics_int		num_total_rendered_triangles;
	statistics_int		num_total_rendered_points;
	statistics_int		num_surfaces;
	statistics_int		num_lights;
	statistics_int		num_particle_instances;
	statistics_int		num_speedtree_instances;
	statistics_int		num_environment_probes;
	statistics_int		num_ambient_volumes;
	statistics_int		num_occlusion_culled_surfaces;
	statistics_int		num_occlusion_culled_lights;
	statistics_int		num_occlusion_culled_grass_patches;
	statistics_int		num_occlusion_culled_particle_instances;
	statistics_int		num_occlusion_culled_decals;
	statistics_int		num_occlusion_culled_env_probes;
	statistics_int		num_occlusion_culled_portals;
	statistics_int		num_occlusion_culled_ambient_volumes;
};

STATIC_SIZE_ASSERT( visibility_statistics_group, 0x1118 );

struct lights_statistics_group : public statistics_group {
	explicit lights_statistics_group( pcstr group_name );
	~lights_statistics_group( ) { }

	statistics_cpu_gpu	accumulate_lighting_time;
	statistics_cpu_gpu	forward_lighting_time;
	statistics_cpu_gpu	shadow_map_time;
};

STATIC_SIZE_ASSERT( lights_statistics_group, 0x770 );

struct particles_statistics_group : public statistics_group {
	explicit particles_statistics_group( pcstr group_name );
	~particles_statistics_group( )
	{
	}

	statistics_cpu_gpu	execute_time;
	statistics_cpu_gpu	sprites_execute_time;
	statistics_cpu_gpu	beamtrails_execute_time;
	statistics_cpu_gpu	meshes_execute_time;
	statistics_int		num_total_instances;
};

STATIC_SIZE_ASSERT( particles_statistics_group, 0xA78 );

struct cascaded_sun_shadow_statistics_group : public statistics_group {
	explicit cascaded_sun_shadow_statistics_group( pcstr group_name );
	~cascaded_sun_shadow_statistics_group( ) { }

	statistics_cpu_gpu	execute_time_cascade_1;
	statistics_cpu_gpu	execute_time_cascade_2;
	statistics_cpu_gpu	execute_time_cascade_3;
	statistics_cpu_gpu	execute_time_cascade_4;
	statistics_int		num_dips_cascade_1;
	statistics_int		num_dips_cascade_2;
	statistics_int		num_dips_cascade_3;
	statistics_int		num_dips_cascade_4;
	statistics_int		num_dips;
	statistics_int		num_clipped_dips;
	statistics_int		num_triangles;
};

STATIC_SIZE_ASSERT( cascaded_sun_shadow_statistics_group, 0xEE0 );

struct postprocess_statistics_group : public statistics_group {
	explicit postprocess_statistics_group( pcstr group_name );
	~postprocess_statistics_group( ) { }

	statistics_cpu_gpu	execute_time;
};

STATIC_SIZE_ASSERT( postprocess_statistics_group, 0x2E0 );

struct forward_stage_statistics_group : public statistics_group {
	explicit forward_stage_statistics_group( pcstr group_name );
	~forward_stage_statistics_group( ) { }

	statistics_cpu_gpu	execute_time;
};

STATIC_SIZE_ASSERT( forward_stage_statistics_group, 0x2E0 );

struct general_statistics_group : public statistics_group {
	explicit general_statistics_group( pcstr group_name );
	~general_statistics_group( ) { }

	statistics_cpu_gpu	render_frame_time;
	statistics_float	cpu_render_frame_time;
	statistics_int		num_setted_shader_constants;
	statistics_int		fps;
	statistics_int		cpu_fps;
	statistics_float	render_only_time;
};

STATIC_SIZE_ASSERT( general_statistics_group, 0x6C8 );

struct speedtree_statistics_group : public statistics_group {
	explicit speedtree_statistics_group( pcstr group_name );
	~speedtree_statistics_group( ) { }

	statistics_cpu_gpu	render_time;
	statistics_float	culling_time;
	statistics_int		num_instances;
};

STATIC_SIZE_ASSERT( speedtree_statistics_group, 0x478 );

struct forward_decals_statistics_group : public statistics_group {
	explicit forward_decals_statistics_group( pcstr group_name );
	~forward_decals_statistics_group( ) { }

	statistics_float	execute_time;
	statistics_int		num_decals;
	statistics_int		num_decal_draw_calls;
};

STATIC_SIZE_ASSERT( forward_decals_statistics_group, 0x2E8 );

struct deferred_decals_statistics_group : public statistics_group {
	explicit deferred_decals_statistics_group( pcstr group_name );
	~deferred_decals_statistics_group( ) { }

	statistics_float	execute_time;
	statistics_int		num_decals;
	statistics_int		num_decal_draw_calls;
};

STATIC_SIZE_ASSERT( deferred_decals_statistics_group, 0x2E8 );

struct distortion_pass_statistics_group : public statistics_group {
	explicit distortion_pass_statistics_group( pcstr group_name );
	~distortion_pass_statistics_group( ) { }

	statistics_cpu_gpu	accumulate_time;
	statistics_cpu_gpu	apply_time;
};

STATIC_SIZE_ASSERT( distortion_pass_statistics_group, 0x528 );

struct ssao_statistics_group : public statistics_group {
	explicit ssao_statistics_group( pcstr group_name );
	~ssao_statistics_group( ) { }

	statistics_cpu_gpu	ssao_accumulate_time;
	statistics_cpu_gpu	ssao_blurring_time;
};

STATIC_SIZE_ASSERT( ssao_statistics_group, 0x528 );

struct sky_statistics_group : public statistics_group {
	explicit sky_statistics_group( pcstr group_name );
	~sky_statistics_group( ) { }

	statistics_cpu_gpu	execute_time;
};

STATIC_SIZE_ASSERT( sky_statistics_group, 0x2E0 );

struct lpv_statistics_group : public statistics_group {
	explicit lpv_statistics_group( pcstr group_name );
	~lpv_statistics_group( ) { }

	statistics_cpu_gpu	lpv_lookup_time;
	statistics_cpu_gpu	propagation_time;
	statistics_cpu_gpu	gv_injection_time;
	statistics_cpu_gpu	vpl_injection_time;
	statistics_cpu_gpu	rsm_downsample_time;
	statistics_cpu_gpu	rsm_rendering_time;
	statistics_int		num_dips;
	statistics_int		num_dips_in_cascade_0;
	statistics_int		num_dips_in_cascade_1;
	statistics_int		num_dips_in_cascade_2;
	statistics_int		num_clipped_dips;
};

STATIC_SIZE_ASSERT( lpv_statistics_group, 0x11F8 );

struct grass_statistics_group : public statistics_group {
	explicit grass_statistics_group( pcstr group_name );
	~grass_statistics_group( ) { }

	statistics_int	num_total_patches;
	statistics_int	num_rendered_patches;
	statistics_int	num_visible_patches;
};

STATIC_SIZE_ASSERT( grass_statistics_group, 0x2C8 );

struct debug_statistics_group : public statistics_group {
	explicit debug_statistics_group( pcstr group_name );
	~debug_statistics_group( ) { }

	statistics_int		avaliable_video_memory;
	statistics_int		texture_video_memory;
	statistics_int		render_tergets_video_memory;
	statistics_int		gbuffer_video_memory;
	statistics_int		num_dips_in_lpv;
	statistics_int		num_vertex_shader_changes;
	statistics_int		num_pixel_shader_changes;
	statistics_int		num_vs_textures_changes;
	statistics_int		num_vs_constants_changes;
	statistics_int		num_vs_samplers_changes;
	statistics_int		num_ps_textures_changes;
	statistics_int		num_ps_constants_changes;
	statistics_int		num_ps_samplers_changes;
	statistics_int		num_input_layout_changes;
	statistics_float	textures_compression_duration;
	statistics_float	dxt_rt_tex_creation_duration;
	statistics_float	cpu_textures_compression_duration;
	statistics_int		gpu_num_compressed_textures;
	statistics_int		cpu_num_compressed_textures;
};

STATIC_SIZE_ASSERT( debug_statistics_group, 0xEE0 );

class statistics : public quasi_singleton< statistics > {
public:
	statistics( );
	~statistics( );

	void start( );
	void render( vostok::ui::world&, u32, u32 ) { }
	void clear( ) { }
	u32 get_num_text_lines( ) const { return 0; }

	statistics_group*					first_group;
	postprocess_statistics_group			postprocess_stat_group;
	ssao_statistics_group				ssao_stat_group;
	distortion_pass_statistics_group		distortion_pass_stat_group;
	particles_statistics_group			particles_stat_group;
	sky_statistics_group				sky_stat_group;
	speedtree_statistics_group			speedtree_stat_group;
	forward_stage_statistics_group			forward_stage_stat_group;
	cascaded_sun_shadow_statistics_group	cascaded_sun_shadow_stat_group;
	grass_statistics_group				grass_stat_group;
	gbuffer_statistics_group			gbuffer_stat_group;
	forward_decals_statistics_group		forward_decals_stat_group;
	lpv_statistics_group				lpv_stat_group;
	deferred_decals_statistics_group		deferred_decals_stat_group;
	lights_statistics_group				lights_stat_group;
	visibility_statistics_group			visibility_stat_group;
	general_statistics_group			general_stat_group;
	debug_statistics_group				debug_stat_group;

private:
	u32	m_max_string_width;
	u32	m_max_string_height;
};

STATIC_SIZE_ASSERT( statistics, 0x7EB8 );

#define USE_RENDER_STATISTICS 1

#if USE_RENDER_STATISTICS
#	define BEGIN_CPUGPU_TIMER(c) { cpu_gpu_timer_scope local_cpu_gpu_timer_scope(c);
#	define END_CPUGPU_TIMER }
#else
#	define BEGIN_CPUGPU_TIMER(c)
#	define END_CPUGPU_TIMER
#endif

#define BEGIN_TIMER(c) { timer_scope local_timer_scope(c);
#define END_TIMER }

} // namespace render
} // namespace vostok

#include <vostok/render/engine/sources/statistics_inline.h>

#endif // #ifndef VOSTOK_RENDER_ENGINE_STATISTICS_H_INCLUDED

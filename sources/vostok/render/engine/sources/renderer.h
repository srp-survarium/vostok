#ifndef VOSTOK_RENDER_ENGINE_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDERER_H_INCLUDED

#include <vostok/fixed_vector.h>
#include <vostok/intrusive_list.h>
#include <vostok/render/core/effect_descriptor.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_texture_list.h>
#include <vostok/render/engine/base_classes.h>
#include <vostok/render/facade/render_stage_types.h>
#include <vostok/timing_timer.h>

#include "cloud_interp_textures.h"
#include "cloud_simulation.h"
#include "material_effects.h"
#include "render_target.h"

namespace vostok {

namespace ui {
struct font;
struct world;
} // namespace ui

namespace render {

class custom_config_value;
class effect_compiler;
class environment_probe_generate_parameters;
class renderer_context;
class renderer_context_targets;
struct render_surface_instance;
class scene;
class scene_view;
class shader_constant_host;
class sky_ambient_occlusion_map_generate_parameters;
class stage;
class stage_debug;
class stage_screen_image;
class stage_view_mode;
class stage_visibility;

struct event_query;
struct hw_hiz_occlusion_manager;

namespace engine {
class world;
} // namespace engine

namespace ui {
struct vertex;
} // namespace ui

struct frame_histogram_info {
	float time;
	float mem;
	u32 dips;
	frame_histogram_info* next;
};

typedef intrusive_list<
	frame_histogram_info,
	frame_histogram_info*,
	&frame_histogram_info::next,
	threading::mutex,
	size_policy,
	no_debug_policy
> frame_histogram_info_list;

class effect_pick_light_luminance : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler& compiler,
		custom_config_value const& config
	) override;
};

struct sort_by_vs_predicate {
	sort_by_vs_predicate( enum_render_stage_type stage_type, u32 const tech_index ) :
		m_stage_type( stage_type ),
		m_tech_index( tech_index )
	{
	}

	bool operator()(
		render_surface_instance const* left,
		render_surface_instance const* right
	) const;

	enum_render_stage_type m_stage_type;
	u32 m_tech_index;
};

struct sort_by_distance_predicate {
	sort_by_distance_predicate(
		float3 const& eye_position,
		bool from_near_to_far
	) :
		m_eye_position( eye_position ),
		m_from_near_to_far( from_near_to_far )
	{
	}

	bool operator()(
		render_surface_instance const* left,
		render_surface_instance const* right
	) const;

public:
	float3 m_eye_position;
	bool m_from_near_to_far;
};

struct sort_by_texture_predicate {
	sort_by_texture_predicate( enum_render_stage_type stage_type, u32 const tech_index ) :
		m_stage_type( stage_type ),
		m_tech_index( tech_index )
	{
	}

	bool operator()(
		render_surface_instance const* left,
		render_surface_instance const* right
	) const;

	enum_render_stage_type m_stage_type;
	u32 m_tech_index;
};

struct sort_by_textures_predicate {
	bool operator()(
		render_surface_instance const*,
		render_surface_instance const*
	) const
	{
		// STATE[STUB]
		return false;
	}
};

class renderer : public boost::noncopyable {
	typedef fixed_vector< stage*, 29 > stages_type;
	typedef frame_histogram_info_list fps_historgram_type;

	// world::clear_resources accesses the renderer context directly.
	friend class engine::world;

public:
	explicit renderer( renderer_context* renderer_context );
	~renderer( );

	void render(
		base_scene_ptr const& in_scene,
		base_scene_view_ptr const& in_view,
		base_output_window_ptr const& output_window,
		math::rectangle< float2 > const& viewport,
		boost::function< void( bool ) > const& on_draw_scene,
		bool draw_debug_terrain,
		vostok::ui::font const* default_font
	);

	void generate_sky_ao_map(
		base_scene_ptr const& scene,
		base_scene_view_ptr const& view,
		base_output_window_ptr const& output_window,
		sky_ambient_occlusion_map_generate_parameters const& parameters
	);

	void generate_environment_probe(
		base_scene_ptr const& scene,
		base_scene_view_ptr const& view,
		base_output_window_ptr const& output_window,
		environment_probe_generate_parameters const& parameters
	);

	void generate_environment_probe_face(
		base_scene_ptr const& scene,
		base_scene_view_ptr const& view,
		base_output_window_ptr const& output_window,
		u32 const face,
		float3 const& position,
		u32 const resolution,
		float4& result,
		float const near_plane
	);

	void recreate_stage( enum_render_stage_type stage_type );
	void pick_lighting_luminance( u32 const x, u32 const y );
	void set_picking_lighting_luminance_mode( bool value );
	bool m_picking_lighting_luminance_mode;

	void present(
		base_output_window_ptr in_output_window,
		math::rectangle< float2 > const& viewport
	);

	u32 frame_id( ) const { return m_frame_id; }

	void clear_resources( );
	void fill_opaque_models( );
	void sort_models(
		vector< render_surface_instance* >& instances,
		enum_render_stage_type stage_type,
		u32 const tech_index
	);
	void sort_models_by_distance(
		vector< render_surface_instance* >& instances,
		bool from_near_to_far
	);

	void setup_render_output_window(
		base_output_window_ptr in_output_window,
		math::rectangle< float2 > const& viewport
	);

	void toggle_render_stage( enum_render_stage_type stage_type, bool toggle );

	void set_view_to_rain_shadow( float4x4 const& matrix )
	{
		m_view_to_rain_shadow = matrix;
	}

	float4x4 const& get_view_to_rain_shadow( ) const
	{
		return m_view_to_rain_shadow;
	}

	void draw_top_dip_models_list(
		vostok::ui::world& world,
		u32 const x,
		u32 const y,
		u32 const count
	) const;

	void gather_statistics( );
	res_effect_ptr get_grass_trample_effect( )
	{
		return m_grass_trample_effect;
	}

	void set_target_context(
		renderer_context_targets const* targets_context,
		bool force_set
	);

	void draw_debug(
		scene* scene,
		scene_view* view,
		float const frame_time,
		vostok::ui::font const* default_font
	);

private:
	void test_perlin( );
	void draw_luminance_picker_info( vostok::ui::font const* default_font );
	void draw_frame_histogram( ) const;
	void draw_stages_stats( vostok::ui::font const* default_font );
	void execute_stages( );
	bool is_effects_ready( ) const;

	float4x4 m_view_to_rain_shadow;
	timing::timer m_timing_timer;
	event_query* m_timing_event;
	event_query* m_frame_sync_event;
	render_target_ptr m_debug_1x1_gpu_data_rt[4];
	res_texture_ptr m_debug_1x1_gpu_data[4];
	res_texture_ptr m_debug_1x1_cpu_data[4];
	float4 m_debug_readed_data[4];
	stages_type m_stages;
	cloud_interp_textures m_cloud_interp_textures;
	hw_hiz_occlusion_manager* m_occlusion_manager;
	renderer_context* m_renderer_context;
	stage_screen_image* m_present_stage;
	stage_view_mode* m_view_mode_stage;
	stage_debug* m_stage_debug;
	stage_visibility* m_visibility_stage;
	timing::timer m_timer;
	timing::timer m_fps_timer;
	shader_constant_host* m_gbuffer_to_screen_type;
	shader_constant_host* m_c_eye_ray_corner;
	shader_constant_host* m_c_probe_position;
	shader_constant_host* m_c_pick_lighting_luminance_position;
	res_effect_ptr m_gbuffer_to_screen_shader;
	res_effect_ptr m_fill_environment_probe_face;
	res_effect_ptr m_editor_apply_wireframe_shader;
	res_effect_ptr m_fill_sky_ao_map_effect;
	res_effect_ptr m_pick_light_luminance_effect;
	res_effect_ptr m_grass_trample_effect;
	float m_last_frame_time;
	float m_current_time;
	u32 m_frame_id;
	cloud_simulation m_simulation;
	static u32 const m_num_fps_history_values = 512;
	fps_historgram_type m_fps_history;
	float4 m_selected_lighting_luminanace_in_screen;
};

STATIC_SIZE_ASSERT( frame_histogram_info, 0x10 );
STATIC_SIZE_ASSERT( effect_pick_light_luminance, 0x4 );
STATIC_SIZE_ASSERT( sort_by_vs_predicate, 0x8 );
STATIC_SIZE_ASSERT( sort_by_distance_predicate, 0x10 );
STATIC_SIZE_ASSERT( sort_by_texture_predicate, 0x8 );
STATIC_SIZE_ASSERT( sort_by_textures_predicate, 0x1 );
STATIC_SIZE_ASSERT( renderer, 0x280 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDERER_H_INCLUDED

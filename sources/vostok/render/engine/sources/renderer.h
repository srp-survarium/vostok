#ifndef VOSTOK_RENDER_ENGINE_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDERER_H_INCLUDED

#include <vostok/render/engine/base_classes.h>
#include <vostok/render/core/effect_descriptor.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/facade/render_stage_types.h>

namespace vostok {

namespace ui {
struct font;
}

namespace render {

class custom_config_value;
class effect_compiler;
class renderer_context;
class renderer_context_targets;
class render_surface_instance;
class scene;
class scene_view;

enum enum_render_stage_type;

namespace ui {
struct vertex;
}

class effect_pick_light_luminance : public effect_descriptor {
public:
	effect_pick_light_luminance( ) { }
	virtual ~effect_pick_light_luminance( ) { }

	virtual void compile( effect_compiler& compiler, custom_config_value const& config ) override;
};

class sort_by_vs_predicate {
public:
	sort_by_vs_predicate( enum_render_stage_type stage_type, u32 tech_index ) :
		m_stage_type( stage_type ),
		m_tech_index( tech_index )
	{
	}

	bool operator()( render_surface_instance const* left, render_surface_instance const* right ) const;

private:
	enum_render_stage_type	m_stage_type;
	u32						m_tech_index;
};

class sort_by_distance_predicate {
public:
	sort_by_distance_predicate( float3 const& eye_position, bool from_near_to_far ) :
		m_eye_position( eye_position ),
		m_from_near_to_far( from_near_to_far )
	{
	}

	bool operator()( render_surface_instance const* left, render_surface_instance const* right ) const;

private:
	float3	m_eye_position;
	bool	m_from_near_to_far;
};

class sort_by_texture_predicate {
public:
	sort_by_texture_predicate( enum_render_stage_type stage_type, u32 tech_index ) :
		m_stage_type( stage_type ),
		m_tech_index( tech_index )
	{
	}

	bool operator()( render_surface_instance const* left, render_surface_instance const* right ) const;

private:
	enum_render_stage_type	m_stage_type;
	u32						m_tech_index;
};

class sort_by_textures_predicate {
public:
	bool operator()( render_surface_instance const*, render_surface_instance const* ) const
	{
		// STATE[STUB]
		return false;
	}
};

class renderer : public boost::noncopyable {
public:
	explicit renderer( renderer_context* renderer_context );
	~renderer( );

	void recreate_stage( enum_render_stage_type stage_type );
	void set_target_context( renderer_context_targets const* targets_context, bool force_set );
	void setup_render_output_window(
		base_output_window_ptr				in_output_window,
		math::rectangle< float2 > const&	viewport
	);
	void toggle_render_stage( enum_render_stage_type stage_type, bool toggle );
	void clear_resources( );
	void fill_opaque_models( );
	void sort_models_by_distance( vector< render_surface_instance* >& instances, bool from_near_to_far );
	void sort_models(
		vector< render_surface_instance* >&		instances,
		enum_render_stage_type					stage_type,
		u32										tech_index
	);
	void render(
		base_scene_ptr const&					in_scene,
		base_scene_view_ptr const&				in_view,
		base_output_window_ptr const&			output_window,
		math::rectangle< float2 > const&		viewport,
		boost::function< void( bool ) > const&	on_draw_scene,
		bool									draw_debug_terrain,
		vostok::ui::font const*					default_font
	);
	void draw_debug(
		scene*					scene,
		scene_view*				view,
		float					frame_time,
		vostok::ui::font const*	default_font
	);
	void present(
		base_output_window_ptr				in_output_window,
		math::rectangle< float2 > const&	viewport
	);

private:
	void execute_stages( );
	bool is_effects_ready( ) const;
	void draw_luminance_picker_info( vostok::ui::font const* default_font );
	void draw_stages_stats( vostok::ui::font const* default_font );
	void draw_frame_histogram( ) const;
};

STATIC_SIZE_ASSERT( effect_pick_light_luminance, 0x4 );
STATIC_SIZE_ASSERT( sort_by_vs_predicate, 0x8 );
STATIC_SIZE_ASSERT( sort_by_distance_predicate, 0x10 );
STATIC_SIZE_ASSERT( sort_by_texture_predicate, 0x8 );
STATIC_SIZE_ASSERT( sort_by_textures_predicate, 0x1 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDERER_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright ( C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/engine/world.h>
#include <vostok/render/facade/particles.h>
#include <vostok/render/facade/scene_view_mode.h>
#include <vostok/render/facade/common_types.h>
#include <vostok/render/facade/render_stage_types.h>
#include <vostok/render/facade/scene_view_mode.h>
#include <vostok/render/facade/particles.h>
#include <vostok/render/engine/vertex_colored.h>
#include <vostok/render/facade/ui_renderer.h>

using vostok::math::float4x4;
using vostok::math::int2_pod;
using vostok::math::rectangle;

#if VOSTOK_PLATFORM_WINDOWS_32
#	if VOSTOK_STATIC_LIBRARIES
#		pragma comment( lib, "SpeedTreeCore_v5.2_VC90MT_Static.lib" )
#		pragma comment( lib, "SpeedTreeForest_v5.2_VC90MT_Static.lib" )
#	else // #endif // #if VOSTOK_STATIC_LIBRARIES
#		pragma comment( lib, "SpeedTreeCore_v5.2_VC90MTDLL_Static.lib" )
#		pragma comment( lib, "SpeedTreeForest_v5.2_VC90MTDLL_Static.lib" )
#	endif // #if VOSTOK_STATIC_LIBRARIES
#else // #if VOSTOK_PLATFORM_WINDOWS_32
#	if VOSTOK_STATIC_LIBRARIES
#		pragma comment( lib, "SpeedTreeCore_v5.2_VC90MT64_Static.lib" )
#		pragma comment( lib, "SpeedTreeForest_v5.2_VC90MT64_Static.lib" )
#	else // #endif // #if VOSTOK_STATIC_LIBRARIES
#		pragma comment( lib, "SpeedTreeCore_v5.2_VC90MTDLL64_Static.lib" )
#		pragma comment( lib, "SpeedTreeForest_v5.2_VC90MTDLL64_Static.lib" )
#	endif // #if VOSTOK_STATIC_LIBRARIES
#endif // #if VOSTOK_PLATFORM_WINDOWS_32

vostok::render::render_allocator_type* vostok::render::g_allocator = 0;

namespace vostok {
namespace render {
namespace engine {

static vostok::uninitialized_reference< engine::world > s_world;

engine::world* create_world							( )
{
	R_ASSERT						( !s_world.initialized() );
	VOSTOK_CONSTRUCT_REFERENCE		( s_world, world ) ( );
	return							s_world.c_ptr( );
}

void destroy										( engine::world*& world )
{
	R_ASSERT						( s_world.initialized() );
	R_ASSERT						( s_world.c_ptr() == world );
	VOSTOK_DESTROY_REFERENCE			( s_world );
	world							= 0;
}

engine::world::world								( ) :
	m_frame_id						( 0 )
{
}

void engine::world::initialize						( )
{
}

engine::world::~world								( )
{
}

void engine::world::clear_resources					( )
{
}

void engine::world::draw_lines						( scene_ptr const& scene, debug_vertices_type const& vertices, debug_indices_type const& indices )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &scene, &vertices, &indices );
	NOT_IMPLEMENTED					( );
}

void engine::world::draw_triangles					( scene_ptr const& scene, debug_vertices_type const& vertices, debug_indices_type const& indices )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &scene, &vertices, &indices );
	NOT_IMPLEMENTED					( );
}

void engine::world::draw_text						(
		vostok::vectora< ui::vertex >& output,
		pcstr const& text,
		vostok::ui::font const& font,
		vostok::float2 const& position,
		vostok::math::color const& text_color,
		vostok::math::color const& selection_color,
		u32 max_line_width,
		bool is_multiline,
		u32 start_selection,
		u32 end_selection
	)
{
	VOSTOK_UNREFERENCED_PARAMETERS	(
		& output,
		& text,
		& font,
		& position,
		& text_color,
		& selection_color,
		max_line_width,
		is_multiline,
		start_selection,
		end_selection
	);
}

void engine::world::set_view_matrix					( scene_view_ptr const& scene_view, float4x4 const& view )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &scene_view, &view );
	NOT_IMPLEMENTED					( );
}

#ifndef MASTER_GOLD
void engine::world::set_view_matrix_only	( scene_view_ptr const& scene_view, float4x4 const& view_matrix )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &scene_view, &view_matrix );
	NOT_IMPLEMENTED					( );
}
#endif // #ifndef MASTER_GOLD

void engine::world::set_projection_matrix			( scene_view_ptr const& scene_view, float4x4 const& projection )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &scene_view, &projection );
	NOT_IMPLEMENTED					( );
}

math::uint2 engine::world::window_client_size		( render::render_output_window_ptr const& render_output_window )
{
	VOSTOK_UNREFERENCED_PARAMETER		( render_output_window );
	NOT_IMPLEMENTED					( return math::uint2(0,0) );
}

void engine::world::draw_debug_lines				( vector< vertex_colored > const& vertices, vector< u16 > const& indices )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &vertices, &indices );
	NOT_IMPLEMENTED					( );
}

void engine::world::draw_debug_triangles			( vector< vertex_colored > const& vertices, vector< u16 > const& indices )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &vertices, &indices );
	NOT_IMPLEMENTED					( );
}

void engine::world::draw_ui_vertices				( ui::vertex const * vertices, u32 const & count, int prim_type, int point_type )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &vertices, &count, &prim_type, &point_type );
	NOT_IMPLEMENTED					( );
}

void engine::world::clear_zbuffer				( float z_value )
{
	VOSTOK_UNREFERENCED_PARAMETER		( z_value );
	NOT_IMPLEMENTED					( );
}

void engine::world::draw_text						( pcstr text, vostok::math::float2 const& position, vostok::ui::font* const in_font, vostok::math::color const& in_color )
{
	VOSTOK_UNREFERENCED_PARAMETERS(
		&text,
		&position,
		&in_font,
		&in_color
	);
	NOT_IMPLEMENTED					( );
}

void engine::world::end_frame						( )
{
	NOT_IMPLEMENTED					( );
}

void engine::world::draw_scene					(
		scene_ptr const& scene,
		scene_view_ptr const& view,
		render_output_window_ptr const& output_window,
		viewport_type const& viewport,
		boost::function< void ( bool ) > const& on_draw_scene
	)
{
	VOSTOK_UNREFERENCED_PARAMETERS	(
		&scene,
		&view,
		&output_window,
		&viewport,
		&on_draw_scene
	);
	NOT_IMPLEMENTED					( );
}

u32 engine::world::frame_id						( )
{
	return							m_frame_id;
}

void engine::world::reload_shaders				( )
{
	NOT_IMPLEMENTED					( );
}

void engine::world::reload_modified_textures	( )
{
	NOT_IMPLEMENTED					( );
}

void engine::world::add_model					( scene_ptr const& in_scene, render::render_model_instance_ptr const& v, float4x4 const& transform, bool apply_transform )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, &v, &transform, apply_transform );
	NOT_IMPLEMENTED					( );
}

void engine::world::update_model				( scene_ptr const& in_scene, render::render_model_instance_ptr const& v, vostok::math::float4x4 const& transform )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, &v, &transform );
	NOT_IMPLEMENTED					( );
}

void engine::world::update_system_model	( render::render_model_instance_ptr const& v, vostok::math::float4x4 const& transform)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &v, &transform );
	NOT_IMPLEMENTED					( );
}

void engine::world::remove_model		( scene_ptr const& in_scene, render::render_model_instance_ptr const& v )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, &v );
	NOT_IMPLEMENTED					( );
}

void engine::world::set_model_material	( render::render_model_instance_ptr const& v, fs_new::virtual_path_string const& subsurface_name, resources::unmanaged_resource_ptr mtl_ptr)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &v, &subsurface_name, &mtl_ptr );
	NOT_IMPLEMENTED					( );
}

void engine::world::set_model_visible	( render::render_model_instance_ptr const& v, fs_new::virtual_path_string const& subsurface_name, bool value )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &v, &subsurface_name, value );
	NOT_IMPLEMENTED					( );
}

void engine::world::set_model_ghost_mode( render::render_model_instance_ptr const& v, bool value)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &v, value );
	NOT_IMPLEMENTED					( );
}

void engine::world::update_model_vertex_buffer	( render::render_model_instance_ptr const& v, vectora<buffer_fragment> const& fragments )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &v, &fragments );
	NOT_IMPLEMENTED					( );
}

void engine::world::update_model_index_buffer	( render::render_model_instance_ptr const& v, vectora<buffer_fragment> const& fragments )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &v, &fragments );
	NOT_IMPLEMENTED					( );
}

void engine::world::add_light					( scene_ptr const& in_scene, u32 id, light_props const& props )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, &id, &props );
	NOT_IMPLEMENTED					( );
}

void engine::world::update_light				( scene_ptr const& in_scene, u32 id, light_props const& props )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, &id, &props );
	NOT_IMPLEMENTED					( );
}

void engine::world::remove_light				( scene_ptr const& in_scene, u32 id )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, &id);
	NOT_IMPLEMENTED					( );
}

void engine::world::add_decal					( scene_ptr const& scene, u32 id, render::decal_properties const& properties )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &scene, id, &properties);
	NOT_IMPLEMENTED					( );
}

void engine::world::update_decal				( scene_ptr const& scene, u32 id, render::decal_properties const& properties )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &scene, id, &properties);
	NOT_IMPLEMENTED					( );
}

void engine::world::remove_decal				( scene_ptr const& scene, u32 id )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &scene, &id);
	NOT_IMPLEMENTED					( );
}

void engine::world::terrain_add_cell			( scene_ptr const& in_scene, render::render_model_instance_ptr const& v )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, &v );
	NOT_IMPLEMENTED					( );
}

void engine::world::terrain_remove_cell			( scene_ptr const& in_scene, render::render_model_instance_ptr const& v )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, &v );
	NOT_IMPLEMENTED					( );
}

void engine::world::terrain_update_cell_buffer	( scene_ptr const& in_scene, render::render_model_instance_ptr const& v, vostok::vectora<terrain_buffer_fragment> const& fragments, float4x4 const& transform)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, &v, &fragments, &transform );
	NOT_IMPLEMENTED					( );
}

void engine::world::terrain_update_cell_aabb	( scene_ptr const& in_scene, render::render_model_instance_ptr const& v, math::aabb const & aabb)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, &v, &aabb );
	NOT_IMPLEMENTED					( );
}

void engine::world::update_skeleton( render::render_model_instance_ptr const& v, math::float4x4* matrices, u32 count )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &v, matrices, count );
	NOT_IMPLEMENTED					( );
}

void engine::world::terrain_add_cell_texture	( scene_ptr const& scene, render::render_model_instance_ptr const& v, fs_new::virtual_path_string const & texture, u32 user_tex_id)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &scene, &v, &texture, user_tex_id );
	NOT_IMPLEMENTED					( );
}

void engine::world::terrain_remove_cell_texture	( scene_ptr const& in_scene, render::render_model_instance_ptr const& v, u32 user_tex_id)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, &v, user_tex_id );
	NOT_IMPLEMENTED					( );
}

void engine::world::terrain_exchange_texture	( scene_ptr const& scene, fs_new::virtual_path_string const & old_texture, fs_new::virtual_path_string const & new_texture)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &scene, &old_texture, &new_texture );
	NOT_IMPLEMENTED					( );
}

void engine::world::setup_view_and_output		( scene_view_ptr const& view, render_output_window_ptr const& output_window, viewport_type const& viewport)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &view, output_window, viewport);
	NOT_IMPLEMENTED					( );
}

void engine::world::play_particle_system			( scene_ptr const& in_scene, particle::particle_system_instance_ptr in_instance, bool use_transform, bool always_looping, math::float4x4 const& transform )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, in_instance, use_transform, always_looping, &transform );
	NOT_IMPLEMENTED					( );
}

void engine::world::stop_particle_system			( scene_ptr const& in_scene, particle::particle_system_instance_ptr in_instance )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_instance, &in_scene);
	NOT_IMPLEMENTED					( );
}

void engine::world::remove_particle_system_instance	( particle::particle_system_instance_ptr in_instance, scene_ptr const& in_scene )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_instance, &in_scene);
	NOT_IMPLEMENTED					( );
}

void engine::world::update_particle_system_instance	( particle::particle_system_instance_ptr instance, scene_ptr const& scene, math::float4x4 const& transform, bool visible, bool paused)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &instance, &transform, visible, paused, &scene);
	NOT_IMPLEMENTED					( );
}

#ifndef MASTER_GOLD

void engine::world::update_preview_particle_system	( vostok::particle::particle_system** preview_ps, scene_ptr const& in_scene, vostok::configs::lua_config_value config_value )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( preview_ps, &config_value, &in_scene);
	NOT_IMPLEMENTED					( );
}

void engine::world::update_preview_particle_system_transform	( vostok::particle::particle_system** preview_ps, scene_ptr const& in_scene, math::float4x4 const& transform )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( preview_ps, &transform, &in_scene);
	NOT_IMPLEMENTED					( );
}

void engine::world::add_preview_particle_system		( vostok::particle::particle_system** preview_ps, scene_ptr const& in_scene, vostok::configs::lua_config_value init_values, math::float4x4 const& transform )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( preview_ps, &init_values, &transform, &in_scene);
	NOT_IMPLEMENTED					( );
}

void engine::world::remove_preview_particle_system	( vostok::particle::particle_system** preview_ps, scene_ptr const& in_scene )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( preview_ps, &in_scene);
	NOT_IMPLEMENTED					( );
}

void engine::world::restart_preview_particle_system	( vostok::particle::particle_system** preview_ps, scene_ptr const& in_scene )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( preview_ps, &in_scene );
	NOT_IMPLEMENTED					( );
}

void engine::world::show_preview_particle_system	( particle::particle_system** preview_ps, scene_ptr const& scene )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( preview_ps, &scene );
	NOT_IMPLEMENTED					( );
}

void engine::world::set_looping_preview_particle_system		( vostok::particle::particle_system** preview_ps, scene_ptr const& in_scene, bool looping )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( preview_ps, &in_scene, looping );
	NOT_IMPLEMENTED					( );
}

void engine::world::draw_terrain_debug				( )
{
	NOT_IMPLEMENTED					( );
}

u32 engine::world::get_num_preview_particle_system_emitters		( vostok::particle::particle_system** in_ps_out_ptr, scene_ptr const& in_scene ) const
{
	VOSTOK_UNREFERENCED_PARAMETERS	( in_ps_out_ptr, &in_scene );
	NOT_IMPLEMENTED					( return 0 );
}

void world::gather_preview_particle_system_statistics	( vostok::particle::particle_system** in_ps_out_ptr, scene_ptr const& in_scene, vostok::particle::preview_particle_emitter_info* out_info )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( in_ps_out_ptr, out_info, &in_scene);
	NOT_IMPLEMENTED					( );
}

#endif // #ifndef MASTER_GOLD

void engine::world::set_selection_parameters			( math::float4 const& selection_color, float selection_rate )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &selection_color, selection_rate );
	NOT_IMPLEMENTED					( );
}

void engine::world::set_slomo( vostok::render::scene_ptr const& in_scene, float time_multiplier )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, time_multiplier );
	NOT_IMPLEMENTED					( );
}

void engine::world::toggle_render_stage				( enum_render_stage_type stage_type, bool toggle )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( stage_type, toggle );
	NOT_IMPLEMENTED					( );
}

void engine::world::setup_rotation_control_modes	( bool color_write )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( color_write );
	NOT_IMPLEMENTED					( );
}

void engine::world::set_view_mode					( scene_view_ptr view_ptr, scene_view_mode view_mode )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &view_ptr, view_mode );
	NOT_IMPLEMENTED					( );
}

void engine::world::set_particles_render_mode		( scene_view_ptr view_ptr, vostok::particle::enum_particle_render_mode render_mode )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &view_ptr, render_mode );
	NOT_IMPLEMENTED					( );
}

void engine::world::enable_post_process				( scene_view_ptr view_ptr, bool enable )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &view_ptr, enable );
	NOT_IMPLEMENTED					( );
}

void engine::world::set_post_process				( scene_view_ptr view_ptr, resources::unmanaged_resource_ptr post_process_resource)
{
	NOT_IMPLEMENTED					( );
}

void engine::world::draw_screen_lines				( scene_ptr const& scene, float2 const* points, u32 count, math::color const& color, float width, u32 pattern )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( points, count, &color, pattern, width, &scene );
	NOT_IMPLEMENTED					( );
}

void engine::world::draw_3D_screen_lines			( scene_ptr const& scene, float3 const* points, u32 count, math::color const& color, float width, u32 pattern, bool use_depth )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( points, count, &color, width, pattern, use_depth, &scene );
	NOT_IMPLEMENTED					( );
}

void engine::world::draw_3D_screen_point			( scene_ptr const& scene, float3 const& position, math::color color, float width, bool use_depth )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( position, &color, width, use_depth, &scene );
	NOT_IMPLEMENTED					( );
}

void engine::world::setup_grid_render_mode			( u32 grid_density )
{
	VOSTOK_UNREFERENCED_PARAMETER		( grid_density );
	NOT_IMPLEMENTED					( );
}

void engine::world::remove_grid_render_mode			( )
{
	NOT_IMPLEMENTED					( );
}

void engine::world::set_renderer_configuration		(fs_new::virtual_path_string const& config_name, bool async_effects )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &config_name, &async_effects );
	NOT_IMPLEMENTED					( );
}

void engine::world::apply_render_options_changes	( )
{
	NOT_IMPLEMENTED					( );
}

#ifndef MASTER_GOLD
void engine::world::select_particle_system_instance	( scene_ptr const& scene, particle::particle_system_instance_ptr const& instance, bool selected )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &scene, &instance, selected );
	NOT_IMPLEMENTED					( );
}

void engine::world::select_speedtree_instance		( scene_ptr const& scene, render::speedtree_instance_ptr const& instance, bool is_selected )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &scene, &instance, is_selected );
	NOT_IMPLEMENTED					( );
}

void engine::world::select_model					( scene_ptr const& scene, render_model_instance_ptr const& v, bool is_selected )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &scene, &v, is_selected );
	NOT_IMPLEMENTED					( );
}

void engine::world::enable_terrain_debug_mode		( bool is_enabled )
{
	VOSTOK_UNREFERENCED_PARAMETER		( is_enabled );
	NOT_IMPLEMENTED					( );
}

void engine::world::draw_render_statistics			( vostok::ui::world* ui_world )
{
	VOSTOK_UNREFERENCED_PARAMETER		( ui_world );
	NOT_IMPLEMENTED					( );
}

void engine::world::apply_material_changes			( fs_new::virtual_path_string const& material_name, vostok::configs::lua_config_value const& config_value )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &material_name, &config_value );
	NOT_IMPLEMENTED					( );
}
#endif // #ifndef MASTER_GOLD

void engine::world::add_speedtree_instance			( scene_ptr const& in_scene, render::speedtree_instance_ptr const& v, float4x4 const& transform, bool populate_forest )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, &v, &transform, populate_forest );
	NOT_IMPLEMENTED					( );
}

void engine::world::remove_speedtree_instance		( scene_ptr const& in_scene, render::speedtree_instance_ptr const& v, bool populate_forest )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, &v, populate_forest );
	NOT_IMPLEMENTED					( );
}

void engine::world::update_speedtree_instance		( scene_ptr const& in_scene, render::speedtree_instance_ptr const& v, float4x4 const& transform, bool populate_forest )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &in_scene, &v, &transform, populate_forest );
	NOT_IMPLEMENTED					( );
}

void engine::world::populate_speedtree_forest		( scene_ptr const& in_scene )
{
	VOSTOK_UNREFERENCED_PARAMETER		( in_scene );
	NOT_IMPLEMENTED					( );
}

void engine::world::set_speedtree_instance_material ( render::speedtree_instance_ptr const& v, fs_new::virtual_path_string const& subsurface_name, resources::unmanaged_resource_ptr mtl_ptr)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &v, &subsurface_name, mtl_ptr );
	NOT_IMPLEMENTED					( );
}

particle::world& engine::world::particle_world		( scene_ptr const& scene )
{
	VOSTOK_UNREFERENCED_PARAMETER		( scene );
	NOT_IMPLEMENTED					( return *(particle::world*)0 );
}

void engine::world::set_sky_material				( scene_ptr const& scene, resources::unmanaged_resource_ptr mtl_ptr)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &scene, &mtl_ptr );
	NOT_IMPLEMENTED					( );
}

} // namespace engine
} // namespace render
} // namespace vostok
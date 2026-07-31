////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright ( C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

// HARVEST REMAINDER: every direct-map body was ported into
// sources/vostok/render/engine/sources/render_engine_world_pc_dx11.cpp.
// What is left below could NOT be ported:
// - header-inline targets ( /* no source */ in canonical world_pc.h; header is read-only )
// - members dropped from the shipped class ( console-commands config loading moved to game.cpp )
// - shipped-enum divergence ( enum_vertex_input_type lost the speedtree_* values )
// - shipped-signature divergence ( set_model_visible bool -> u32 flags )
// - removed subsystems ( terrain_* )

#include "pch.h"
#include <vostok/render/engine/world.h>
#include <vostok/render/facade/ambient_volume_properties.h>
#include <vostok/render/core/world.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/particle/api.h>
#include "system_renderer.h"
#include "terrain.h"
#include "material_manager.h"
#include "environment.h"
#include "stage_particles.h"
#include "engine_options.h"
#include "scene_manager.h"
#include "scene.h"
#include "scene_view.h"
#include "renderer.h"
#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include "renderer_context.h"
#include "render_model_user.h"
#include "render_model_skeleton.h"
#include <vostok/render/core/options.h>
#include "speedtree.h"
#include "speedtree_cook.h"
#include "speedtree_forest.h"
#include <vostok/render/facade/material_effects_instance_cook_data.h>
#include "material_effects_instance_cook.h"
#include "render_output_window.h"

#include <vostok/console_command.h>
#include <vostok/console_command_processor.h>

namespace vostok {
namespace render {

void engine::world::set_renderer_configuration( fs_new::virtual_path_string const& config_name, bool async_effects )
{
	fs_new::virtual_path_string						path;
	path.assignf						("resources/render/%s.cfg", config_name.c_str());

	query_resource(
		path.c_str(),
		resources::binary_config_class,
		boost::bind( &engine::world::on_renderer_config_loaded, this, async_effects, _1),
		g_allocator
	);
}

void engine::world::on_console_commands_config_loaded( bool load_renderer_options, resources::queries_result& data )
{
	if( !data.is_successful( ) )
	{
		LOG_ERROR("config file loading FAILED");
		if(load_renderer_options)
			set_renderer_configuration	( "", true );
		return;
	}

	resources::pinned_ptr_const<u8> pinned_data	(data[ 0 ].get_managed_resource( ));
	memory::reader				F( pinned_data.c_ptr( ), pinned_data.size( ) );
	vostok::console_commands::load		( F, vostok::console_commands::execution_filter_all );

	if (load_renderer_options)
		set_renderer_configuration	( options::ref().get_current_configuration(), true );
}

void engine::world::load_console_commands_config_query( pcstr cfg_name, bool load_renderer_options )
{
	resources::query_resource	(
		cfg_name,
		vostok::resources::raw_data_class,
		boost::bind(&engine::world::on_console_commands_config_loaded, this, load_renderer_options, _1),
		g_allocator,
		NULL,
		NULL,
		assert_on_fail_false
	);
}

// legacy initialize() tail dropped on port: the cfg_load cc_delegates moved to
// game.cpp in the shipped exe (target statics cfg_load_cc / cfg_load_level)
//	static vostok::console_commands::cc_delegate cfg_load_cc_0( "cfg_load", boost::bind(&engine::world::load_console_commands_config_query, this, _1, false ), true );
//	static vostok::console_commands::cc_delegate cfg_load_cc_1( "cfg_load", boost::bind(&engine::world::load_console_commands_config_query, this, _1, true ), true );
//
//	cfg_load_cc_0.execute				("resources/startup.cfg");
//	cfg_load_cc_1.execute				("user_data/user.cfg");

void engine::world::clear_resources						()
{
	// Not sure if we need this
	//model_manager::ref().clear_resources		( );
}

#ifndef MASTER_GOLD
void engine::world::set_view_matrix_only			( scene_view_ptr const& scene_view, float4x4 const& view_matrix )
{
	static_cast_checked< render::scene_view* >( scene_view.c_ptr() )->camera_set_view_only	( view_matrix );
}
#endif // #ifndef MASTER_GOLD

void engine::world::draw_text( pcstr text, vostok::math::float2 const& position, vostok::ui::font* const in_font, vostok::math::color const& in_color )
{
	VOSTOK_UNREFERENCED_PARAMETERS(text, &position, in_font, &in_font, &in_color);
	//NOT_IMPLEMENTED();
}

void engine::world::draw_debug_lines			( colored_vertices_type  const& vertices, colored_indices_type  const& indices)
{
	system_renderer::ref().set_w( float4x4().identity());
	render::system_renderer::ref().draw_lines	(
		&*vertices.begin(),
		&*vertices.end(),
		&*indices.begin(),
		&*indices.end()
	);
}

void engine::world::draw_debug_triangles		( colored_vertices_type const& vertices, colored_indices_type  const& indices)
{
	system_renderer::ref().set_w( float4x4().identity());
	render::system_renderer::ref().draw_triangles(
		&*vertices.begin(),
		&*vertices.end(),
		&*indices.begin(),
		&*indices.end()
	);
}

pcstr engine::world::type			()
{
	return				( "dx11");
}

void engine::world::setup_view_and_output		( scene_view_ptr const& view_ptr, render_output_window_ptr const& output_window, viewport_type const& viewport)
{
	render::system_renderer::ref().setup_scene_view( view_ptr );
	render::system_renderer::ref().setup_render_output_window( output_window, viewport );
}

void engine::world::clear_zbuffer		( float z_value)
{
	backend::ref().clear_depth_stencil( D3D_CLEAR_DEPTH /*| D3D11_CLEAR_STENCIL */, z_value, 0);
}

void engine::world::draw_editor_lines		( colored_vertices_type const& vertices, colored_indices_type  const& indices)
{
	system_renderer::ref().set_w( float4x4().identity());
	render::system_renderer::ref().draw_lines(
		&*vertices.begin(),
		&*vertices.end(),
		&*indices.begin(),
		&*indices.end()
	);
}

void engine::world::draw_editor_triangles	( colored_vertices_type const& vertices, colored_indices_type  const& indices)
{
	system_renderer::ref().set_w( float4x4().identity());
	render::system_renderer::ref().draw_triangles(
		&*vertices.begin(),
		&*vertices.end(),
		&*indices.begin(),
		&*indices.end()
	);
}

void engine::world::draw_screen_lines		( vostok::render::scene_ptr const& scene, vostok::math::float2 const* points, u32 count, math::color const& color, float width, u32 pattern)
{
	VOSTOK_UNREFERENCED_PARAMETER	( scene );

	float3* const points_3d		= pointer_cast< float3* >( ALLOCA( sizeof(vostok::math::float3) * count ) );

	// normalize coordinates
	for ( u32 i = 0; i < count; ++i)
		points_3d[i]			= float3(
			+	 points[i].elements[0] / (float)vostok::render::backend::ref().target_width()  *2.f - 1.f,
			-(	(points[i].elements[1] / (float)vostok::render::backend::ref().target_height())*2.f - 1.f),
			0.f
		);

	render::system_renderer::ref().draw_screen_lines( points_3d, count, color, width, pattern, false, true);
}

void engine::world::draw_3D_screen_lines	( vostok::render::scene_ptr const& scene, vostok::math::float3 const* points, u32 count, math::color const& color, float width, u32 pattern, bool use_depth)
{
	VOSTOK_UNREFERENCED_PARAMETER	( scene );

	render::system_renderer::ref().draw_screen_lines( points, count, color, width, pattern, use_depth, false);
}

void engine::world::draw_3D_screen_point	( vostok::render::scene_ptr const& scene, float3 const& position, math::color color, float width, bool use_depth)
{
	VOSTOK_UNREFERENCED_PARAMETER	( scene );

	render::system_renderer::ref().draw_3D_point( position, width, color, use_depth);
}

void engine::world::setup_grid_render_mode	( u32 grid_density)
{
	VOSTOK_UNREFERENCED_PARAMETER	( grid_density);
	render::system_renderer::ref().setup_grid_render_mode( grid_density);

}
void engine::world::remove_grid_render_mode	()
{
	render::system_renderer::ref().remove_grid_render_mode();
}

//vostok::math::float4x4 const&	engine::world::get_view_matrix			() const
//{
//	return system_renderer::ref().get_v();
//}
//
//vostok::math::float4x4 const&	engine::world::get_projection_matrix	() const
//{
//	return system_renderer::ref().get_p();
//}
//vostok::render::viewport_type engine::world::get_viewport	() const
//{
//	D3D_VIEWPORT viewport;
//	backend::ref().get_viewport( viewport);
//	return viewport_type( vostok::math::int2( math::floor(viewport.TopLeftX), math::floor(viewport.TopLeftY)), vostok::math::int2( math::floor(viewport.Width), math::floor(viewport.Height) ));
//}

// NOT PORTED: shipped enum_vertex_input_type has no speedtree_* values
// (facade/vertex_input_type.h is canonical and matches the shipped PDB enum);
// the shipped constants must be recovered from asm in the matcher phase.
static enum_vertex_input_type speedtree_subsurface_name_to_vertex_input_type(fs_new::virtual_path_string const& subsurface_name)
{
	if (subsurface_name=="branch")			return speedtree_branch_vertex_input_type;
	else if (subsurface_name=="frond")		return speedtree_frond_vertex_input_type;
	else if (subsurface_name=="leafmesh")	return speedtree_leafmesh_vertex_input_type;
	else if (subsurface_name=="leafcard")	return speedtree_leafcard_vertex_input_type;
	else if (subsurface_name=="billboard")	return speedtree_billboard_vertex_input_type;
	else									return null_vertex_input_type;
}

static speedtree_tree::component_type vertex_input_type_to_speedtree_component_type(enum_vertex_input_type type)
{
	switch (type)
	{
		case speedtree_branch_vertex_input_type:	return speedtree_tree::branch;
		case speedtree_frond_vertex_input_type:		return speedtree_tree::frond;
		case speedtree_leafmesh_vertex_input_type:	return speedtree_tree::leafmesh;
		case speedtree_leafcard_vertex_input_type:	return speedtree_tree::leafcard;
		case speedtree_billboard_vertex_input_type: return speedtree_tree::billboard;
		default:									NODEFAULT(return speedtree_tree::branch);
	};
}

static void on_speedtree_material_effects_instance_ready(resources::queries_result& data, speedtree_tree* tree, enum_vertex_input_type vertex_input_type)
{
	if (data[0].is_successful())
		tree->set_material_effects(
			vostok::static_cast_resource_ptr<material_effects_instance_ptr>(data[0].get_unmanaged_resource()),
			vertex_input_type_to_speedtree_component_type(vertex_input_type),
			data[0].get_requested_path()
		);
}

void engine::world::set_speedtree_instance_material (render::speedtree_instance_ptr const& v,
													 fs_new::virtual_path_string const& subsurface_name,
													 resources::unmanaged_resource_ptr in_mtl_ptr)
{
	speedtree_tree*		tree				= static_cast_checked<speedtree_tree*>(v->m_speedtree_tree_ptr.c_ptr());
	material_ptr		mtl_ptr				= static_cast_resource_ptr<material_ptr>(in_mtl_ptr);

	if (!mtl_ptr)
		return;

	resources::user_data_variant			data_variant;

	data_variant.set						(NEW(material_effects_instance_cook_data)(
		speedtree_subsurface_name_to_vertex_input_type(subsurface_name),
		static_cast_resource_ptr<resources::unmanaged_resource_ptr>(mtl_ptr))
	);

	// Must be query_resource_and_wait!
	resources::query_resource_and_wait		(
		mtl_ptr->get_material_name(),
		resources::material_effects_instance_class,
		boost::bind(
			&on_speedtree_material_effects_instance_ready,
			_1,
			tree,
			speedtree_subsurface_name_to_vertex_input_type(subsurface_name)
		),
		vostok::render::g_allocator,
		&data_variant
	);
}

// NOT PORTED: canonical set_model_visible takes u32 flags (legacy bool value);
// the shipped loop body over the flags is matcher-phase work.
void engine::world::set_model_visible( render::render_model_instance_ptr const& v, fs_new::virtual_path_string const& subsurface_name, bool value )
{
	render_model_instance_impl_ptr model = static_cast_resource_ptr<render_model_instance_impl_ptr>(v);

	render_surface_instances		list;
	model->get_surfaces	( list, false );

	bool check_name = (subsurface_name.length()!=0);

	for (render_surface_instances::iterator it=list.begin(); it!=list.end(); ++it)
	{
		render_surface_instance& inst = *(*it);
		bool apply = !check_name || (inst.m_render_surface->m_render_geometry.shading_group_name == subsurface_name.c_str());

		if( apply )
			inst.m_visible = value;
	}
}

void engine::world::update_system_model( render::render_model_instance_ptr const& v, vostok::math::float4x4 const& transform)
{
	render_model_instance_impl_ptr model = static_cast_resource_ptr<render_model_instance_impl_ptr>(v);
	model->set_transform				( transform );
}

void engine::world::draw_terrain_debug( )
{
	//system_renderer::ref().draw_debug_terrain();
}

void engine::world::update_model_index_buffer( render::render_model_instance_ptr const& v, vostok::vectora<vostok::render::buffer_fragment> const& fragments)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( v,fragments);
	//NOT_IMPLEMENTED();
}

//void engine::world::change_model_shader( render::render_model_instance_ptr const& v, char const* shader, char const* texture)
//{
//	NOT_IMPLEMENTED();
//	//render_model* r_model = static_cast_checked<render_model*>(&(*v));
//	//r_model->set_shader(shader, texture);
//}

void engine::world::terrain_add_cell( vostok::render::scene_ptr const& in_scene,  render::render_model_instance_ptr const& v )
{
	vostok::render::scene* scene			 = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());
	terrain_render_model_instance_ptr t	 = static_cast_resource_ptr<terrain_render_model_instance_ptr>(v);

	ASSERT( scene->terrain());
	if( scene->terrain())
		scene->terrain()->add_cell		( t);
}

void engine::world::terrain_remove_cell( vostok::render::scene_ptr const& in_scene, render::render_model_instance_ptr const& v )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	terrain_render_model_instance_ptr t	= static_cast_resource_ptr<terrain_render_model_instance_ptr>(v);

	ASSERT( scene->terrain());
	if( scene->terrain())
		scene->terrain()->remove_cell		( t);
}

void engine::world::terrain_update_cell_buffer( vostok::render::scene_ptr const& in_scene, render::render_model_instance_ptr const& v, vostok::vectora<vostok::render::terrain_buffer_fragment> const& fragments, float4x4 const& transform)
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	terrain_render_model_instance_ptr t	= static_cast_resource_ptr<terrain_render_model_instance_ptr>(v);

	ASSERT( scene->terrain());
	if( scene->terrain())
		scene->terrain()->update_cell_buffer( t, fragments, transform);
}

void engine::world::terrain_update_cell_aabb( vostok::render::scene_ptr const& in_scene, render::render_model_instance_ptr const& v, math::aabb const & aabb)
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	terrain_render_model_instance_ptr t	= static_cast_resource_ptr<terrain_render_model_instance_ptr>(v);

	ASSERT( scene->terrain());
	if( scene->terrain())
		scene->terrain()->update_cell_aabb	( t, aabb);
}

void engine::world::terrain_add_cell_texture(  vostok::render::scene_ptr const& in_scene, render::render_model_instance_ptr const& v, fs_new::virtual_path_string const & texture, u32 user_tex_id)
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	terrain_render_model_instance_ptr t	= static_cast_resource_ptr<terrain_render_model_instance_ptr>(v);

	ASSERT( scene->terrain());
	if( scene->terrain())
		scene->terrain()->add_cell_texture	( t, texture, user_tex_id);
}

void engine::world::terrain_remove_cell_texture( vostok::render::scene_ptr const& in_scene, render::render_model_instance_ptr const& v, u32 user_tex_id)
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	terrain_render_model_instance_ptr t	= static_cast_resource_ptr<terrain_render_model_instance_ptr>(v);

	ASSERT( scene->terrain());
	if( scene->terrain())
		scene->terrain()->remove_cell_texture	( t, user_tex_id);
}

void engine::world::terrain_exchange_texture( vostok::render::scene_ptr const& in_scene, fs_new::virtual_path_string const & old_texture, fs_new::virtual_path_string const & new_texture)
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	ASSERT( scene->terrain());
	if( scene->terrain())
		scene->terrain()->exchange_texture( old_texture, new_texture );
}

void engine::world::setup_rotation_control_modes( bool color_write )
{
	render::system_renderer::ref().setup_rotation_control_modes( color_write);
}

untyped_buffer * create_quad_ib()
{
	if( g_quad_ib)
		return &*g_quad_ib;

	const u32 quad_count = 4*1024;
	const u32 idx_count  = quad_count*2*3;

	// 	if ( device::ref().get_caps().geometry.software)
	// 	{
	// 		ASSERT( 0);
	// 		usage |= D3DUSAGE_SOFTWAREPROCESSING;
	// 	}

	u16	indices[idx_count];

	int		vertex_id = 0;
	int		idx= 0;
	for ( int i=0; i<quad_count; ++i)
	{
		indices[idx++]=u16( vertex_id+0);
		indices[idx++]=u16( vertex_id+1);
		indices[idx++]=u16( vertex_id+2);

		indices[idx++]=u16( vertex_id+3);
		indices[idx++]=u16( vertex_id+2);
		indices[idx++]=u16( vertex_id+1);

		vertex_id+=4;
	}

	g_quad_ib = resource_manager::ref().create_buffer( idx_count*sizeof(u16), indices, enum_buffer_type_index, false);
	return &*g_quad_ib;
}

#ifndef MASTER_GOLD

void engine::world::update_preview_particle_system	( vostok::particle::particle_system** preview_ps, vostok::render::scene_ptr const& in_scene, vostok::configs::lua_config_value config_value )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->particle_world()->update_preview	( preview_ps, config_value );
}

void engine::world::update_preview_particle_system_transform	( vostok::particle::particle_system** preview_ps, vostok::render::scene_ptr const& in_scene, math::float4x4 const& transform )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->particle_world()->set_transform	( preview_ps, transform );
}

void engine::world::set_looping_preview_particle_system	( vostok::particle::particle_system** preview_ps, vostok::render::scene_ptr const& in_scene, bool is_looped )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->particle_world()->set_looped	( preview_ps, is_looped );
}

void engine::world::add_preview_particle_system		( vostok::particle::particle_system** in_ps_out_ptr, vostok::render::scene_ptr const& in_scene, vostok::configs::lua_config_value init_values, math::float4x4 const& transform )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	*in_ps_out_ptr = scene->particle_world()->add( init_values, transform );
}

void engine::world::remove_preview_particle_system	( vostok::particle::particle_system** in_ps_out_ptr, vostok::render::scene_ptr const& in_scene )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	R_ASSERT						( in_ps_out_ptr );
	scene->particle_world()->remove	( in_ps_out_ptr );
}

void engine::world::restart_preview_particle_system	( vostok::particle::particle_system** in_ps_out_ptr, vostok::render::scene_ptr const& in_scene )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->particle_world()->restart( in_ps_out_ptr );
}

void engine::world::show_preview_particle_system	( vostok::particle::particle_system** in_ps_out_ptr, vostok::render::scene_ptr const& in_scene )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->particle_world()->show	( in_ps_out_ptr );
}

u32 engine::world::get_num_preview_particle_system_emitters(vostok::particle::particle_system** in_ps_out_ptr, vostok::render::scene_ptr const& in_scene ) const
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	return	scene->particle_world()->get_num_preview_particle_system_emitters( in_ps_out_ptr );
}

void engine::world::gather_preview_particle_system_statistics	( vostok::particle::particle_system** in_ps_out_ptr, vostok::render::scene_ptr const& in_scene, vostok::particle::preview_particle_emitter_info* out_info )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	return	scene->particle_world()->gather_stats( in_ps_out_ptr, out_info );
}

void engine::world::draw_render_statistics	( vostok::ui::world* ui_world )
{
	statistics::ref().render(*ui_world, 5, 5);
}

void engine::world::select_particle_system_instance	( scene_ptr const& scene, particle::particle_system_instance_ptr const& instance, bool const is_selected )
{
	static_cast_checked< render::scene& >( *scene.c_ptr() ).select_particle_system_instance( instance, is_selected );
}

void engine::world::select_speedtree_instance		( scene_ptr const& scene, render::speedtree_instance_ptr const& instance, bool const is_selected )
{
	static_cast_checked< render::scene& >( *scene.c_ptr() ).select_speedtree_instance( instance, is_selected );
}

void engine::world::select_model( scene_ptr const& scene, render_model_instance_ptr const& instance, bool const is_selected )
{
	render_model_instance_impl_ptr impl = static_cast_resource_ptr<render_model_instance_impl_ptr>(instance);
	static_cast_checked< render::scene& >( *scene.c_ptr() ).select_model( impl, is_selected );
}

void engine::world::enable_terrain_debug_mode		( bool const is_enabled )
{
	m_enable_terrain_debug_mode						= is_enabled;
}

void engine::world::apply_material_changes( fs_new::virtual_path_string const& material_name, vostok::configs::lua_config_value const& config_value )
{
	material_manager::ref().update_material_from_config(material_name, config_value);
}

#endif // #ifndef MASTER_GOLD

void	engine::world::set_selection_parameters	( vostok::math::float4 const& selection_color, float selection_rate )
{
	system_renderer::ref().set_selection_parameters( selection_color, selection_rate );
}

void engine::world::update_ambient_volume( vostok::render::scene_ptr const& in_scene, u32 id, render::ambient_volume_properties const& in_properties)
{
	// NOT PORTED: legacy is a no-op, but the shipped body has real code
	// ( FUNCTION BODY[0x654cb0] +0x015 ) - matcher-phase work.
	VOSTOK_UNREFERENCED_PARAMETERS( in_scene, id, in_properties );
}

void engine::world::remove_ambient_volume( vostok::render::scene_ptr const& in_scene, u32 id)
{
	// NOT PORTED: legacy is a no-op, but the shipped body has real code
	// ( FUNCTION BODY[0x654a50] +0x010 ) - matcher-phase work.
	VOSTOK_UNREFERENCED_PARAMETERS( in_scene, id );
}

#if 0

static void fix_specular_in_stage( configs::lua_config_value& material_config, pcstr stage_name )
{
	if (!material_config.value_exists(stage_name))
		return;

	configs::lua_config_value& stage_config	= material_config[stage_name]["effect"];

	if (stage_config.value_exists("texture_specular_intensity"))
	{
		if (stage_config.value_exists("texture_specular_power"))
		{
			stage_config["texture_specular_power"].assign_lua_value(stage_config["texture_specular_intensity"]);
		}
	}
}

static void on_material_loaded( pstr request_path, resources::queries_result& data )
{
	if( data.is_successful() )
	{
		configs::lua_config_ptr material_config		= static_cast_checked<configs::lua_config*>(data[0].get_unmanaged_resource().c_ptr());
		fix_specular_in_stage						(material_config->get_root()["material"], "g_stage");
		fix_specular_in_stage						(material_config->get_root()["material"], "forward");
		fix_specular_in_stage						(material_config->get_root()["material"], "lighting");

		fs_new::virtual_path_string			physical_path;
		if (resources::query_result::convert_logical_to_disk_path(&physical_path, request_path))
		{
			material_config->get_root().save_as		(physical_path.c_str());
		}
	}
	FREE(request_path);
}

static void on_fs_iterator_materials_ready( pcstr materials_path, vostok::resources::fs_iterator fs_it )
{
	resources::fs_iterator it				= fs_it.children_begin();
	resources::fs_iterator it_e				= fs_it.children_end();

	for ( ; it!=it_e; ++it )
	{
		pcstr mname							= it.get_name();

		if(it.is_folder())
		{
			fs_new::virtual_path_string new_materials_path;
			new_materials_path.assignf("%s/%s", materials_path, mname);
			on_fs_iterator_materials_ready(new_materials_path.c_str(), it);
			continue;
		}

		if (strstr(mname, ".orig"))
			continue;

		fs_new::virtual_path_string request_path;
		request_path.assignf				("%s/%s", materials_path, mname);

		pstr			request_path_buffer = ALLOC(char, 256);
		memory::zero						(request_path_buffer, 256);
		memory::copy						(request_path_buffer, request_path.length(), request_path.c_str(), request_path.length());

 		resources::query_resource(
 			request_path.c_str(),
			resources::lua_config_class,
 			boost::bind(&on_material_loaded, request_path_buffer, _1),
 			vostok::render::g_allocator
 		);
	}
}

static void fix_materials( pcstr materials_path )
{
	resources::query_fs_iterator(
		materials_path,
		boost::bind(&on_fs_iterator_materials_ready, materials_path, _1),
		g_allocator,
		resources::recursive_true
	);
}
#endif // #if 0

#if 0
static bool support_srgb_format( pcstr name )
{
	return	strings::equal(name, "A8")		||
			strings::equal(name, "DXT1")	||
			strings::equal(name, "DXT1a")	||
			strings::equal(name, "DXT3")	||
			strings::equal(name, "DXT5");
}

static void on_texture_options_loaded( pstr request_path, resources::queries_result& data )
{
	if( data.is_successful() )
	{
		configs::lua_config_ptr options_config			= static_cast_checked<configs::lua_config*>(data[0].get_unmanaged_resource().c_ptr());
		configs::lua_config_value& options_config_value	= options_config->get_root()["options"];

		if (!options_config_value.value_exists("srgb"))
			options_config_value["srgb"]				= (u32)support_srgb_format((pcstr)options_config_value["format"]);

		fs_new::virtual_path_string			physical_path;
		if (resources::query_result::convert_logical_to_disk_path(&physical_path, request_path))
		{
			options_config->get_root().save_as		(physical_path.c_str());
		}
	}
	DEBUG_FREE(request_path);
}

static void on_fs_iterator_texture_options_ready( pcstr options_path, vostok::resources::fs_iterator fs_it )
{
	resources::fs_iterator it				= fs_it.children_begin();
	resources::fs_iterator it_e				= fs_it.children_end();

	for ( ; it!=it_e; ++it )
	{
		pcstr mname							= it.get_name();

		if(it.is_folder())
		{
			fs_new::virtual_path_string new_options_path;
			new_options_path.assignf("%s/%s", options_path, mname);
			on_fs_iterator_texture_options_ready(new_options_path.c_str(), it);
			continue;
		}

		if (strstr(mname, ".orig") || !strstr(mname, ".options"))
			continue;

		fs_new::virtual_path_string request_path;
		request_path.assignf				("%s/%s", options_path, mname);

		pstr			request_path_buffer = DEBUG_ALLOC(char, 256);
		memory::zero						(request_path_buffer, 256);
		memory::copy						(request_path_buffer, request_path.length(), request_path.c_str(), request_path.length());

 		resources::query_resource(
 			request_path.c_str(),
			resources::lua_config_class,
 			boost::bind(&on_texture_options_loaded, request_path_buffer, _1),
 			&vostok::debug::g_mt_allocator
 		);
	}
}

static void fix_texture_options( pcstr path )
{
	resources::query_fs_iterator(
		 path,
		boost::bind(&on_fs_iterator_texture_options_ready,  path, _1),
		&vostok::debug::g_mt_allocator,
		resources::recursive_true
	);
}
#endif // #if 0

math::uint2 engine::world::window_client_size	( render::render_output_window_ptr const& render_output_window )
{
	return	static_cast_checked< render::render_output_window* >( render_output_window.c_ptr() )->get_window_client_size( );
}

} // namespace render
} // namespace vostok

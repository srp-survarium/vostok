#include "pch.h"
#include <vostok/render/facade/material_effects_instance_cook_data.h>
#include <vostok/render/facade/model.h>
#include <vostok/render/engine/model_format.h>
#include "combined_model_cooker.h"
#include "model_manager.h"
#include "render_model.h"
#include "render_model_skeleton.h"
#include "render_surface.h"

namespace vostok {
namespace render {
// 10 target lines are likely retail-compiled-out source.
skeleton_combined_model_cook::skeleton_combined_model_cook( ) : resources::translate_query_cook( resources::skeleton_combined_model_class, reuse_true, use_current_thread_id )
{ resources::register_cook( this );
}


void skeleton_combined_model_cook::translate_query( resources::query_result_for_cook& parent )
{
	skeleton_combined_cook_data* cook_data = 0;
	if ( parent.user_data( ) )
		parent.user_data( )->try_get( cook_data );

	if ( cook_data )
	{ query_resources_by_data( &parent, cook_data );
		return;
	}
	// 6 target lines are likely retail-compiled-out source.
	fs_new::virtual_path_string config_path;
	config_path.assignf( "resources/models/%s.combined_skin", parent.get_requested_path( ) );
	resources::query_resource(
		config_path.c_str( ),
		resources::binary_config_class,
		boost::bind(
			&skeleton_combined_model_cook::on_config_loaded, this, _1, &parent ),
		g_allocator,
		0,
		&parent
	);

}


void build_from_config( configs::binary_config_ptr& cfg, skeleton_combined_cook_data* cook_data )
{ configs::binary_config_value root = cfg->get_root( );
	cook_data->skeleton_name.assignf( "resources/animations/%s", (pcstr)root["skeleton"] );
	cook_data->bind_pose_name.assignf( "resources/models/%s.skinned_model/render/bind_pose", (pcstr)root["bind_pose"] );
	cook_data->models_count = (u8)root["parts"].size( );

	for ( u32 part_idx = 0; part_idx < cook_data->models_count; ++part_idx )
	{
		configs::binary_config_value current = root["parts"][part_idx];
		cook_data->model_defs[part_idx].part_name = (pcstr)current["part_name"];
		cook_data->model_defs[part_idx].base_model_name = (pcstr)current["base_model"];
	}

}
// 3 target lines are likely retail-compiled-out source.
void skeleton_combined_model_cook::on_config_loaded( resources::queries_result& result, resources::query_result_for_cook* parent )
{ if ( !result.is_successful( ) )
	{
		parent->finish_query( result_error );
		return;
	}

	skeleton_combined_cook_data* cook_data = NEW( skeleton_combined_cook_data )( true );
	configs::binary_config_ptr config = static_cast_resource_ptr< configs::binary_config_ptr >(
		result[0].get_unmanaged_resource( ) );
	build_from_config( config, cook_data );
	query_resources_by_data( parent, cook_data );
}
// 3 target lines are likely retail-compiled-out source.
void skeleton_combined_model_cook::query_resources_by_data( resources::query_result_for_cook* parent, skeleton_combined_cook_data* cook_data )
{ u32 request_count = cook_data->models_count * 3 + 2;
	resources::request* requests = ALLOC( resources::request, request_count );


	requests[0].set( cook_data->skeleton_name.c_str( ), resources::skeleton_class );
	requests[1].set( cook_data->bind_pose_name.c_str( ), resources::raw_data_class );

	for ( u32 part_idx = 0; part_idx < cook_data->models_count; ++part_idx )
	{
		STR_JOINA(
			requests[2 + part_idx * 3].path,
			"resources/models/",
			cook_data->model_defs[part_idx].base_model_name.c_str( ),
			".skinned_model/render/",
			cook_data->model_defs[part_idx].part_name.c_str( ),
			"/converted_model"
		);
		requests[2 + part_idx * 3].id = resources::converted_model_class;

		STR_JOINA(
			requests[3 + part_idx * 3].path,
			"resources/models/", cook_data->model_defs[part_idx].base_model_name.c_str( ), ".skinned_model/settings"
		);
		requests[3 + part_idx * 3].id = resources::binary_config_class;
		STR_JOINA(
			requests[4 + part_idx * 3].path,
			"resources/models/",
			cook_data->model_defs[part_idx].base_model_name.c_str( ),
			".skinned_model/render/",
			cook_data->model_defs[part_idx].part_name.c_str( ),
			"/export_properties"
		);
		requests[4 + part_idx * 3].id = resources::binary_config_class;
	}

	resources::query_resources(
		requests, request_count,
		boost::bind( &skeleton_combined_model_cook::on_resources_loaded, this, _1, parent, cook_data ),
		g_allocator, 0,
		parent
	);

	FREE( requests );
}


static enum_vertex_input_type mesh_type_to_vertex_input_type( mesh_type_enum type )
{ switch ( type )
	{
		case mt_static_mesh:
		case mt_static_submesh:
			return static_mesh_vertex_input_type;
		case mt_static_submesh_colored:
			return static_mesh_vertex_colored_input_type;
		case mt_skinned_submesh_1w:
			return skeletal_1_bones_mesh_vertex_input_type;
		case mt_skinned_submesh_2w:
			return skeletal_2_bones_mesh_vertex_input_type;
		case mt_skinned_submesh_3w:
			return skeletal_3_bones_mesh_vertex_input_type;
		case mt_skinned_mesh:
		case mt_skinned_submesh_4w:
			return skeletal_4_bones_mesh_vertex_input_type;
		case mt_user_mesh_wire:
			return wires_vertex_input_type;
		case mt_grass_mesh:
			return grassmesh_vertex_input_type;
		default: NODEFAULT( return unknown_vertex_input_type );
	};
}
// 4 target lines are likely retail-compiled-out source.
void skeleton_combined_model_cook::on_resources_loaded( resources::queries_result& data, resources::query_result_for_cook* parent, skeleton_combined_cook_data* cook_data )
{ if ( !data.is_successful( ) )
	{
		parent->finish_query( result_error );
		return;
	}


	cook_data->skeleton = static_cast_resource_ptr< animation::skeleton_ptr >( data[0].get_unmanaged_resource( ) );
	cook_data->bind_pose = data[1].get_managed_resource( );

	u32 parts_count = cook_data->models_count;
	resources::request* requests = ALLOC( resources::request, parts_count );
	resources::user_data_variant* user_data_variants = ALLOC( resources::user_data_variant, parts_count );
	resources::user_data_variant** user_data_variants_ptrs = ALLOC( resources::user_data_variant*, parts_count );

	for ( u32 i = 0; i < cook_data->models_count; ++i )
	{

		cook_data->model_defs[i].converted_model = data[2 + i * 3].get_managed_resource( );
		cook_data->model_defs[i].owner_model_config = static_cast_resource_ptr< configs::binary_config_ptr >( data[3 + i * 3].get_unmanaged_resource( ) );
		cook_data->model_defs[i].export_properties_config = static_cast_resource_ptr< configs::binary_config_ptr >( data[4 + i * 3].get_unmanaged_resource( ) );

		configs::binary_config_value root = cook_data->model_defs[i].owner_model_config->get_root( );

		if ( cook_data->model_defs[i].material_name.empty( ) )
		{
			cook_data->model_defs[i].material_name = (pcstr)
				root["material_settings"]
					[cook_data->model_defs[i].part_name.c_str( )]
					["material_name"];
		}

		mesh_type_enum model_type = (mesh_type_enum)(u16) cook_data->model_defs[i].export_properties_config->get_root( )["type"];

		requests[i].set( cook_data->model_defs[i].material_name.c_str( ), resources::material_effects_instance_class );
		user_data_variants_ptrs[i] = new ( &user_data_variants[i] ) resources::user_data_variant;
		user_data_variants_ptrs[i]->set(
			NEW(
				material_effects_instance_cook_data
			)(
				mesh_type_to_vertex_input_type(
					model_type
				),
				0
			)

		);
	}
	resources::query_resources( requests, parts_count, boost::bind( &skeleton_combined_model_cook::on_material_effects_loaded, this, _1, parent, cook_data ), g_allocator, (resources::user_data_variant const**)user_data_variants_ptrs, parent );
	FREE( user_data_variants_ptrs );
	FREE( user_data_variants );
	FREE( requests );
}
// 41 target lines are likely retail-compiled-out source.
void skeleton_combined_model_cook::on_material_effects_loaded( resources::queries_result& data, resources::query_result_for_cook* parent, skeleton_combined_cook_data* cook_data )
{ if ( !data.is_successful( ) )

		LOG_ERROR( "skeleton_combined_model_cook::on_material_effects_loaded : data loading failed" );
	// 3 target lines are likely retail-compiled-out source.
	render_model* result_model = model_factory::create_render_model( mt_skinned_mesh );

	resources::pinned_ptr_const< u8 > bind_pose_ptr( cook_data->bind_pose );
	memory::reader bones_reader( bind_pose_ptr.c_ptr( ), bind_pose_ptr.size( ) );
	static_cast< skeleton_render_model* >( result_model )->load_bones( bones_reader );


	u8 parts_count = cook_data->models_count;
	render_surface** surfaces = ALLOC( render_surface*, parts_count );

	for ( u32 i = 0; i < parts_count; ++i )
	{

		mesh_type_enum model_type = (mesh_type_enum)(u32) cook_data->model_defs[i].export_properties_config->get_root( )["type"];
		render_surface* surface = model_factory::create_render_surface( model_type );
		surfaces[i] = surface;

		resources::pinned_ptr_const< u8 > converted_model_ptr( cook_data->model_defs[i].converted_model );
		memory::chunk_reader model_reader( converted_model_ptr.c_ptr( ), converted_model_ptr.size( ), memory::chunk_reader::chunk_type_sequential );
		surface->load( cook_data->model_defs[i].export_properties_config->get_root( ), model_reader );
		cook_data->model_defs[i].material_effects = data[i].get_unmanaged_resource( );
		surface->set_material_effects(
			static_cast_resource_ptr< material_effects_instance_ptr >( cook_data->model_defs[i].material_effects ),
			cook_data->model_defs[i].material_name.c_str( )
		);
	}
	result_model->set_children( surfaces, parts_count, 0 );
	parent->set_unmanaged_resource( result_model, resources::nocache_memory, sizeof( render_model ) );

	parent->finish_query( result_success );
	if ( cook_data->owner_is_cook )
		DELETE( cook_data );
}


void skeleton_combined_model_cook::delete_resource( resources::resource_base* resource )
{ DELETE( resource );
}
// 4 target lines are likely retail-compiled-out source.
skeleton_combined_render_model_instance_cook::skeleton_combined_render_model_instance_cook( ) : resources::translate_query_cook( resources::skeleton_combined_render_model_instance_class, reuse_false, use_current_thread_id )
{ resources::register_cook( this );
}


void skeleton_combined_render_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{ skeleton_combined_cook_data* cook_data = 0; if ( parent.user_data( ) )


		parent.user_data( )->try_get( cook_data );
	// 3 target lines are likely retail-compiled-out source.
	fs_new::virtual_path_string render_path( parent.get_requested_path( ) );
	resources::query_resource(
		render_path.c_str( ), resources::skeleton_combined_model_class,
		boost::bind( &skeleton_combined_render_model_instance_cook::on_resources_loaded, this, _1, &parent ),
		g_allocator,
		parent.user_data( ), &parent
	);
}


void skeleton_combined_render_model_instance_cook::on_resources_loaded( resources::queries_result& data, resources::query_result_for_cook* parent )
{ if ( !data.is_successful( ) )
	{
		parent->finish_query( result_error );
		return;
	}

	skeleton_render_model_ptr model = static_cast_resource_ptr< skeleton_render_model_ptr >( data[0].get_unmanaged_resource( ) );
	skeleton_render_model_instance* created_resource = NEW( skeleton_render_model_instance );
	created_resource->assign_original( model );

	parent->set_unmanaged_resource( created_resource, resources::nocache_memory, sizeof( skeleton_render_model_instance ) );
	parent->finish_query( result_success );
}


void skeleton_combined_render_model_instance_cook::delete_resource( resources::resource_base* resource )
{ DELETE( resource );
}
// 7 target lines are likely retail-compiled-out source.
skeleton_combined_model_instance_cook::skeleton_combined_model_instance_cook( ) : resources::translate_query_cook( resources::skeleton_combined_model_instance_class, reuse_false, use_current_thread_id )
{ resources::register_cook( this );
}


void skeleton_combined_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
	skeleton_combined_cook_data* cook_data = 0;
	if ( parent.user_data( ) )
		parent.user_data( )->try_get( cook_data );

	resources::request r[2] = {
		{ parent.get_requested_path( ), resources::skeleton_combined_render_model_instance_class },
		{
			cook_data ? cook_data->skeleton_name.c_str( ) :
				"resources/animations/skeletons/scavengers_01",
			resources::skeleton_class
		}
	};

	resources::user_data_variant ud;
	ud.set( cook_data );
	resources::user_data_variant const* params[2] = { &ud, 0 };

	resources::query_resources(
		r, 2,
		boost::bind(
			&skeleton_combined_model_instance_cook::on_resources_loaded, this, _1, &parent ),
		g_allocator,
		params, &parent
	);
}


void skeleton_combined_model_instance_cook::delete_resource( resources::resource_base* resource )
{ DELETE( resource );
}


void skeleton_combined_model_instance_cook::on_resources_loaded( resources::queries_result& data, resources::query_result_for_cook* parent_query )
{ if ( !data.is_successful( ) )
	{
		parent_query->finish_query( result_error );
		return;
	}

	skeleton_model_instance* created_resource = NEW( skeleton_model_instance );

	created_resource->m_render_model = static_cast_resource_ptr< render_model_instance_ptr >( data[0].get_unmanaged_resource( ) );
	created_resource->m_skeleton = static_cast_resource_ptr< animation::skeleton_ptr >( data[1].get_unmanaged_resource( ) );


	skeleton_render_model_instance* skel_mesh = static_cast< skeleton_render_model_instance* >( created_resource->m_render_model.c_ptr( ) );

	skel_mesh->m_bones_matrices.resize( skel_mesh->m_original->m_inverted_bones_matrices_in_bind_pose.size( ) );

	for ( u32 i = 0; i < skel_mesh->m_bones_matrices.size( ); ++i )
		skel_mesh->m_bones_matrices[i] = math::float4x4( ).identity( );

	parent_query->set_unmanaged_resource( created_resource, resources::nocache_memory, sizeof( skeleton_model_instance ) );
	parent_query->finish_query( result_success );
}

} // namespace render
} // namespace vostok

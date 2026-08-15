#include "pch.h"
#include "grass_cook.h"
#include "grass_world.h"

namespace vostok {
namespace render {

grass_cook::grass_cook( ) :
	resources::translate_query_cook(
		resources::grass_world_class,
		reuse_false,
		use_current_thread_id
)
{
	resources::register_cook( this );
}

void grass_cook::translate_query( resources::query_result_for_cook& parent )
{
	grass_loading_data* loading_data;
	parent.user_data( )->try_get( loading_data );

	grass_world* result = NEW( grass_world );


	configs::binary_config_value const& config = *loading_data->t_current;
	u8 const layers_count = (u8)config["layers"].size( );

	grass_cook_data* cook_data = NEW( grass_cook_data );
	cook_data->desc = NEW_ARRAY( grass_layer_desc*, layers_count );
	cook_data->data = NEW_ARRAY( grass_layer_data*, layers_count );
	cook_data->parent_query = &parent;
	cook_data->result = result;

	buffer_vector< resources::request > requests( ALLOCA( sizeof( resources::request ) * layers_count ), layers_count );
	requests.resize( layers_count );
	for ( u8 i = 0; i < layers_count; ++i )
	{
		cook_data->desc[i] = NEW( grass_layer_desc );
		cook_data->data[i] = NEW( grass_layer_data );

		grass_layer_desc* layer_desc = cook_data->desc[i];
		grass_layer_data* layer_data = cook_data->data[i];

		configs::binary_config_value const& t = config["layers"][i];
		requests[i].id = resources::raw_data_class;

		if ( t.value_exists( "intermediate_filename" ) )
		{
			fixed_string< 260 > s = t["intermediate_filename"];
			requests[i].path = strings::duplicate( g_allocator, s.c_str( ) + s.find( "user_data" ) );
		}
		else
		{

			fs_new::virtual_path_string filename;
			filename.assignf( "%s/%s", loading_data->project_resources_path.c_str( ), pcstr( t["filename"] ) );
			STR_JOINA( requests[i].path, filename.c_str( ) );
		}

		layer_desc->max_slope_ang = t["max_slope"];
		layer_desc->random_orient = t["random_dir"];
		layer_desc->random_scale = t["random_scale"];
		layer_desc->wind_factor = t["wind_factor"];
		layer_desc->use_face_normal = t["use_face_normal"];

		layer_data->lt_x_m = t["lt_x"];
		layer_data->lt_z_m = t["lt_z"];

		if ( t.value_exists( "models" ) )
		{
			u8 const models_count = (u8)t["models"].size( );
			layer_desc->models_list.resize( models_count );

			for ( u8 model_index = 0;
				  model_index < models_count;
				  ++model_index )
			{
				layer_desc->models_list[model_index].name = pcstr( t["models"][model_index] );
				layer_desc->models_list[model_index].probability_ = 1.f / u32( models_count );
				layer_desc->models_list[model_index].scale = 1.f;
			}
		}
		else
		{
			u8 const models_count = (u8)t["model_def"].size( );
			layer_desc->models_list.resize( models_count );

			for ( u8 model_index = 0;
				  model_index < models_count;
				  ++model_index )
			{
				layer_desc->models_list[model_index].name = pcstr( t["model_def"][model_index]["name"] );
				layer_desc->models_list[model_index].probability_ = t["model_def"][model_index]["probability"];

				if ( t["model_def"][model_index].value_exists( "scale" ) )
					layer_desc->models_list[model_index].scale = float( t["model_def"][model_index]["scale"] );
				else
					layer_desc->models_list[model_index].scale = 1.f;
			}
		}
	}


	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind( &grass_cook::on_layers_loaded, this, _1, cook_data ),
		g_allocator,
		0,
		&parent
	);
}

void grass_cook::on_layers_loaded( resources::queries_result& data, grass_cook_data* cook_data )
{
	u8 const layers_count = (u8)data.size( );
	for ( u8 i = 0; i < layers_count; ++i )
	{
		grass_layer_desc* layer_desc = cook_data->desc[i];
		cook_data->data[i]->layer_data_raw_file = data[i].get_managed_resource( );
		cook_data->result->update_grass_layer( layer_desc, cook_data->data[i], true, true, true );
	}

	cook_data->result->populate( 16.f );
	cook_data->parent_query->set_unmanaged_resource(
		cook_data->result,
		resources::nocache_memory,
		sizeof( grass_world )
	);
	cook_data->parent_query->finish_query( result_success );

	DELETE_ARRAY( cook_data->desc );
	DELETE_ARRAY( cook_data->data );
	DELETE( cook_data );
}

void grass_cook::delete_resource( resources::resource_base* resource )
{
	grass_world* world = static_cast<grass_world*>( resource );
	DELETE( world );
}

} // namespace render
} // namespace vostok

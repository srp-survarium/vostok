////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sound_environment_cook.h"
#include "sound_environment.h"
#include "sound_scene.h"
#include <vostok/configs_binary_config.h>
#include <vostok/render/static_model_instance_user_data.h>

namespace vostok {
namespace sound {

sound_environment_cook::sound_environment_cook	( ) :
	super(
		resources::sound_environment_class,
		reuse_false,
		use_resource_manager_thread_id
	)
{
}

void sound_environment_cook::translate_query	( resources::query_result_for_cook& parent )
{
	render::static_model_instance_user_data model_user_data;
	bool const success				= parent.user_data( )->try_get( model_user_data );
	R_ASSERT						( success );
	pcstr model_name				= (*model_user_data.config)["lib_name"];

	fixed_string<256> path;
	path.assignf					( "resources/models/%s.model/render", model_name );

	resources::query_resource(
		path.c_str( ),
		resources::binary_config_class,
		boost::bind( &sound_environment_cook::on_model_config_loaded, this, _1 ),
		g_allocator,
		0,
		&parent
	);
}

void sound_environment_cook::on_model_config_loaded	( resources::queries_result& data )
{
	configs::binary_config_ptr cfg =
		static_cast_resource_ptr<configs::binary_config_ptr>( data[0].get_unmanaged_resource( ) );
	resources::query_result_for_cook& parent	= *data.get_parent_query( );

	render::static_model_instance_user_data model_user_data;
	bool const success				= parent.user_data( )->try_get( model_user_data );
	R_ASSERT						( success );

	float3 min_aabb					= cfg->get_root( )["bounding_box"]["min"];
	float3 max_aabb					= cfg->get_root( )["bounding_box"]["max"];
	float3 dimension				= max_aabb - min_aabb;
	float3 position					= (*model_user_data.config)["position"];
	float3 rotation					= (*model_user_data.config)["rotation"];

	float4x4* transform				= NEW(float4x4);
	*transform						=
		math::create_scale( dimension / 2.0f ) *
		math::create_rotation( rotation ) *
		math::create_translation( position + float3( 0.0f, dimension.y / 2.0f, 0.0f ) );

	pcstr environment_name			= (*model_user_data.config)["sound_environment"];
	sound_scene& scn				= *static_cast<sound_scene*>( model_user_data.sound_scene.c_ptr( ) );
	u32 env_params_id				= scn.get_environment_params_id( environment_name );
	if ( env_params_id != u32( -1 ) )
	{
		sound_environment* created_resource	= NEW(sound_environment)( env_params_id );
		scn.insert_environment				( *created_resource, *transform );
		DELETE								( transform );
		parent.set_unmanaged_resource		( created_resource, resources::nocache_memory, sizeof( sound_environment ) );
		parent.finish_query					( result_success );
		return;
	}

	fixed_string<256> path;
	path.assignf					( "resources/sounds/environments/%s.environment", environment_name );
	resources::query_resource(
		path.c_str( ),
		resources::binary_config_class,
		boost::bind( &sound_environment_cook::on_environment_options_loaded, this, _1, transform ),
		g_allocator,
		0,
		&parent
	);
}

void sound_environment_cook::on_environment_options_loaded	(
	resources::queries_result& data,
	float4x4* transform
)
{
	resources::query_result_for_cook& parent	= *data.get_parent_query( );
	render::static_model_instance_user_data model_user_data;
	bool const success				= parent.user_data( )->try_get( model_user_data );
	R_ASSERT						( success );

	configs::binary_config_ptr cfg =
		static_cast_resource_ptr<configs::binary_config_ptr>( data[0].get_unmanaged_resource( ) );
	configs::binary_config_value root	= cfg->get_root( )["environment"];

	XAUDIO2FX_REVERB_I3DL2_PARAMETERS* params = NEW(XAUDIO2FX_REVERB_I3DL2_PARAMETERS);
	params->DecayHFRatio				= (float)root["decay_hf_ratio"];
	params->DecayTime					= (float)root["decay_time"];
	params->Density						= (float)root["density"];
	params->Diffusion					= (float)root["diffusion"];
	params->HFReference					= (float)root["hf_reference"];
	params->Reflections					= (s32)root["reflections"];
	params->ReflectionsDelay			= (float)root["reflections_delay"];
	params->Reverb						= (s32)root["reverb"];
	params->ReverbDelay					= (float)root["reverb_delay"];
	params->Room							= (s32)root["room"];
	params->RoomHF						= (s32)root["room_hf"];
	params->RoomRolloffFactor			= (float)root["room_rolloff_factor"];
	params->WetDryMix					= (float)root["wet_dry_mix"];

	sound_scene& scn				= *static_cast<sound_scene*>( model_user_data.sound_scene.c_ptr( ) );
	pcstr environment_name			= (*model_user_data.config)["sound_environment"];
	u32 environment_params_id			= scn.get_environment_params_id( environment_name );
	if ( environment_params_id == u32( -1 ) )
		scn.add_environment_params		( environment_name, params, environment_params_id );

	sound_environment* created_resource	= NEW(sound_environment)( environment_params_id );
	scn.insert_environment				( *created_resource, *transform );
	DELETE								( transform );
	parent.set_unmanaged_resource		( created_resource, resources::nocache_memory, sizeof( sound_environment ) );
	parent.finish_query					( result_success );
}

void sound_environment_cook::delete_resource	( resources::resource_base* resource )
{
	UNMANAGED_DELETE					( resource );
}

} // namespace sound
} // namespace vostok

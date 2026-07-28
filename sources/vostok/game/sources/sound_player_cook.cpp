////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sound_player_cook.h"
#include <vostok/resources.h>
#include <vostok/resources_query_result.h>
#include <vostok/configs_binary_config.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

 sound_player_cook::sound_player_cook( sound::world* world, resources::class_id_enum class_id ) :
	resources::translate_query_cook( class_id, reuse_false, use_current_thread_id ),
	m_world( world )
{
}

void sound_player_cook::translate_query( resources::query_result_for_cook& parent )
{
	resources::query_resource	(
		parent.get_requested_path(),
		resources::binary_config_class,
		boost::bind( &sound_player_cook::on_config_loaded, this, _1 ),
		g_allocator,
		0,
		&parent
	);
}

// claude@NOTE: 53% - the dtor call is a distinct target statement but the free
// path differs: target emits strip_pointer<doug_lea_allocator> + inline
// vostok_mspace_free, base resolves VOSTOK_FREE_IMPL to get_pointer + free_helper
// (g_allocator pass-by-pointer idiom mismatch). Structure (2 stmts) is faithful.
void sound_player_cook::delete_resource( resources::resource_base* resource )
{
	resource->~resource_base			( );
	VOSTOK_FREE_IMPL						( g_allocator, resource );
}

void sound_player_cook::on_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();
	if ( !data.is_successful() )
	{
		R_ASSERT										( data.is_successful(), "couldn't retrieve sound collection options" );
		parent->finish_query							( result_error );
		return;
	}

	configs::binary_config_ptr config					= static_cast_resource_ptr<configs::binary_config_ptr const>( data[0].get_unmanaged_resource() );
	configs::binary_config_value const& sounds_value	= (*config)["sounds"];

	configs::binary_config_value::const_iterator it		= sounds_value.begin();
	configs::binary_config_value::const_iterator it_end	= sounds_value.end();

	buffer_vector< resources::request >	requests		( ALLOCA( sizeof( resources::request ) * sounds_value.size() ), sounds_value.size() );

	for ( ; it != it_end; ++it )
	{
		configs::binary_config_value const& sound_value	= *it;
		resources::request								request;
		request.path									= sound_value["filename"];
		request.id										= resources::sound_collection_class;
		requests.push_back								( request );
	}

	resources::query_resources	(
		&requests.front(),
		requests.size(),
		boost::bind( &sound_player_cook::on_sounds_loaded, this, _1, config ),
		g_allocator,
		0,
		parent
	);

}

// STATE[STUB]
void sound_player_cook::on_sounds_loaded( resources::queries_result& data, configs::binary_config_ptr config )
{
	// LOCALS
	// resources::unmanaged_resource_ptr sound_scene
	// ai_sound_player::sounds_collection_type* const end
	// resources::query_result_for_cook* const parent
	// sound::world_user* 				sound_world_user
	// configs::binary_config_value const* it_end
	// ai_sound_player* const 			player
	// ai::brain_unit_cook_params 		params
	// ******

	// FUNCTION BODY[0x768a90]: 52
	// <0x768a93>|0x003|+0x00b:'87'
	// <0x768a9e>|0x00e|+0x028:'88'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x768ac6>|0x036|+0x016:'95'
	// <0>
	// <0x768adc>|0x04c|+0x004:'97'
	// <0>
	// <1>
	// <2>
	// <0x768ae0>|0x050|+0x00d:'101'
	// <0x768aed>|0x05d|+0x015:'102'
	// <0>
	// <0x768b02>|0x072|+0x006:'104'
	// <0x768b08>|0x078|+0x00c:'105'
	// <0x768b14>|0x084|+0x017:'106'
	// <0>
	// <0x768b2b>|0x09b|+0x004:'108'
	// <0x768b2f>|0x09f|-0x08e:'109'
	// <0x768aa1>|0x011|+0x0ca:'110'
	// <0x768b6b>|0x0db|+0x008:'110'
	// <0>
	// <0x768b73>|0x0e3|+0x01b:'112'
	// <0>
	// <1>
	// <0x768b8e>|0x0fe|+0x004:'115'
	// <0x768b92>|0x102|+0x026:'116'
	// <0x768bb8>|0x128|+0x1c0:'117'
	// <0x768d78>|0x2e8|-0x167:'117'
	// <0>
	// <0x768c11>|0x181|-0x02b:'119'
	// <0x768be6>|0x156|+0x004:'120'
	// <0>
	// <0x768bea>|0x15a|+0x00e:'122'
	// <0x768bf8>|0x168|+0x011:'123'
	// <0>
	// <0x768c09>|0x179|+0x010:'125'
	// <0x768c19>|0x189|+0x008:'125'
	// <0>
	// <1>
	// <0x768c21>|0x191|+0x016:'128'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x768c37>|0x1a7|+0x0a9:'134'
	// <0>
	// <1>
	// <0x768ce0>|0x250|+0x037:'137'
	// <0x768d17>|0x287|+0x00b:'138'
	// ******
}


} // namespace survarium

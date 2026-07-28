////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "human_npc_cook.h"
#include "human_npc.h"
#include "game_world.h"
#include "game.h"
#include "game_memory.h"
#include "animated_model_instance.h"
#include "animation_space_graph.h"
#include <vostok/resources.h>
#include <vostok/resources_query_result.h>
#include <vostok/resources_memory.h>
#include <vostok/configs_binary_config.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/ai/npc.h>
#include <vostok/ai/world.h>
#include <vostok/sound/world.h>
#include <vostok/sound/world_user.h>
#include <vostok/sound/sound_emitter.h>
#include <vostok/physics/world.h>

#include <boost/bind.hpp>

namespace survarium {

 human_npc_cook::human_npc_cook( game_world& world ) :
	translate_query_cook( resources::human_npc_class, reuse_true, use_any_thread_id ),
	m_game_world( world )
{
}

void human_npc_cook::translate_query( resources::query_result_for_cook& parent )
{
	configs::binary_config_value* t_object	= ( configs::binary_config_value* )( parent.creation_data_from_user().c_ptr() );

	if ( t_object != 0 )
	{
		pcstr npc_brain_config_path		= ( *t_object )["brain"];
		resources::query_resource		(
			npc_brain_config_path,
			resources::binary_config_class,
			boost::bind( &human_npc_cook::on_queried_data_received, this, _1 ),
			g_allocator,
			parent.user_data(),
			&parent
		);
		return;
	}

	resources::query_resource			(
		parent.get_requested_path(),
		resources::binary_config_class,
		boost::bind( &human_npc_cook::on_queried_data_received, this, _1 ),
		g_allocator,
		parent.user_data(),
		&parent
	);
}

void human_npc_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL					( g_allocator, resource );
}

void human_npc_cook::on_queried_data_received( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();
	if ( !data.is_successful() )
	{
		R_ASSERT										( data.is_successful(), "couldn't retrieve npc config options" );
		parent->finish_query							( result_error );
		return;
	}

	configs::binary_config_ptr config					= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource() );

	on_npc_options_received								( config->get_root(), *parent );
}

// claude@NOTE: STRUCTURE reconstruction (target 15 stmts / base 14, near-match). Statement
// order + types follow the recorded source lines (70-119). Two residual gaps vs target: the
// L99 model-request slot and the L119 temporary-teardown tail show as TRGT_ONLY (the variant /
// request-array temporaries are destroyed differently under /Od vs the gold inline), and the
// sound statement (L85) is reconstructed as get_logic_world_user().register_receiver - the
// exact world_user/sound_scene spelling is inferred from human_npc::enable, not yet confirmed.
// Residual otherwise is the inline-vs-call wall (variant<32>::set / boost::bind / query_resources
// inlined gold-side) + the variant-storage alignment (and esp).
// sushi@TODO: confirm the L85 sound statement spelling and the L99/L119 temporary teardown.
void human_npc_cook::on_npc_options_received(
	configs::binary_config_value const&		config_value,
	resources::query_result_for_cook&		parent
)
{
	configs::binary_config_value const& attributes		= config_value["attributes"];

	configs::binary_config_value* project_config		= ( configs::binary_config_value* )( parent.creation_data_from_user().c_ptr() );

	human_npc* const human								= NEW( human_npc )( m_game_world );

	pcstr brain_unit_path								= attributes["brain_unit"];

	pcstr model_path									= ( *project_config )["model"];

	pcstr space_graph_path								= attributes["animation_space_graph"];

	m_game_world.get_game().get_sound_world().get_logic_world_user().register_receiver( m_game_world.get_sound_scene(), *human );

	ai::brain_unit_cook_params cook_brain_unit_params;
	cook_brain_unit_params.npc							= human;

	resources::user_data_variant brain_unit_params;
	brain_unit_params.set								( cook_brain_unit_params );

	resources::user_data_variant physics_world;
	physics_world.set									( m_game_world.get_physics_world() );

	resources::user_data_variant const* params[]		=
	{
		&brain_unit_params,
		&physics_world,
		NULL,
		NULL
	};

	resources::request requests[] =
	{
		{ brain_unit_path,											resources::brain_unit_class },
		{ model_path,												resources::game_animated_model_instance_class },
		{ "resources/animations/single/slot_1/walk/on_site_idle",	resources::animation_class },
		{ space_graph_path,											resources::animation_space_graph_class }
	};

	resources::query_resources							(
		requests,
		boost::bind( &human_npc_cook::on_subresources_loaded, this, _1, human ),
		g_allocator,
		params,
		&parent
	);
}

// claude@NOTE: STRUCTURE reconstruction (target 17 stmts / base 15, near-match). The data[i]
// extraction order + set_* calls follow the structure view (resource indices from the
// queries_result member offsets in the asm). Residual structural gaps: target has 2 extra
// statements after the is_successful guard (L127/L129) and the resource set_* / set_unmanaged
// grouping diverges (BASE_ONLY set_model/set_brain_unit/set_unmanaged) - the gold inline fuses
// the intrusive_ptr<>::set + refcount xadd into the data-extraction statements where our /Od
// emits them as separate calls. Otherwise the inline-vs-call wall + the config-copy alignment.
// sushi@TODO: recover the 2 L127/L129 statements + the exact resource set_* statement grouping.
void human_npc_cook::on_subresources_loaded( resources::queries_result& data, human_npc* const human )
{
	configs::binary_config_value human_attributes_config	=
		( *( configs::binary_config_value* )( data.get_parent_query()->creation_data_from_user().c_ptr() ) )["attributes"];

	resources::query_result_for_cook* const parent		= data.get_parent_query();

	if ( !data.is_successful() )
	{
		parent->finish_query							( result_error );
		return;
	}

	resources::unmanaged_resource_ptr brain_unit_ptr	= data[1].get_unmanaged_resource();

	animated_model_instance_ptr model_ptr				= static_cast_resource_ptr< animated_model_instance_ptr >( data[1].get_unmanaged_resource() );

	human->set_model									( model_ptr );

	resources::managed_resource_ptr default_animation	= data[2].get_managed_resource();

	human->set_default_animation						( default_animation );

	animation_space_graph_ptr new_graph					= static_cast_resource_ptr< animation_space_graph_ptr >( data[3].get_unmanaged_resource() );

	human->set_animation_space_graph					( new_graph );

	human->set_brain_unit								( brain_unit_ptr );

	parent->set_unmanaged_resource						( human, resources::nocache_memory, 0x2E0 );

	parent->finish_query								( result_success );

	m_game_world.on_npc_attributes_received				( human_attributes_config, human );
}

} // namespace survarium

// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/player_parameters_cook.h>

#include <vostok/game_core/player_parameters_modifyer.h>
#include <vostok/game_core/player_parameters_modifyer_cook.h>

#include <vostok/game_core/base_player.h>
#include <vostok/game_core/body_part_parameters_modifyer.h>
#include <vostok/game_core/boosters_enum.h>
#include <vostok/game_core/dictionary_item.h>
#include <vostok/game_core/dispersion_calculator.h>
#include <vostok/game_core/items_dictionary.h>
#include <vostok/game_core/player_profile.h>
#include <vostok/game_core/player_stamina.h>
#include <vostok/game_core/profile_slot_enum.h>
#include <vostok/game_core/profile_slots.h>
#include <vostok/game_core/weapon_core.h>

namespace survarium {

struct bodypart_health_regen_scale_predicate {
	inline	explicit	bodypart_health_regen_scale_predicate( float coeff ) : m_coeff( coeff ) { }
	inline	void		operator()	( body_part_parameters* const params ) const
	{
		params->set_parameters( params->get_max_health( ), params->get_regeneration_speed( ) * m_coeff );
	}

public:
	/* 0x0000 */	float		m_coeff;
}; // struct bodypart_health_regen_scale_predicate

void player_parameters_modifyer::apply( base_player* player )
{
	damage_model_ptr damage_model = player->damage_model( );

	std::map< fixed_string<16>, body_part_parameters_modifyer >::const_iterator body_part_it	= this->body_part_parameters_modifyers.begin( );
	std::map< fixed_string<16>, body_part_parameters_modifyer >::const_iterator body_part_end	= this->body_part_parameters_modifyers.end( );

	for ( ; body_part_it != body_part_end ; ++body_part_it )
	{
		fixed_string<16> body_part_name = body_part_it->first;

		body_part_parameters*			current_body_part_parameters	= damage_model->get_body_part( body_part_name.c_str( ) );
		body_part_parameters_modifyer& 	current_body_part_modifyer		= this->body_part_parameters_modifyers[body_part_name]; // sushi@NOTE: Why not just body_part_it->second

		current_body_part_parameters->set_parameters(
			current_body_part_parameters->get_max_health( )			+ current_body_part_modifyer.health,
			current_body_part_parameters->get_regeneration_speed( ) + current_body_part_modifyer.health_regeneration
		);

		std::map< fixed_string<16>, hit_type_parameters_modifyer >::const_iterator hit_type_it		= current_body_part_modifyer.hit_type_modifyers.begin( );
		std::map< fixed_string<16>, hit_type_parameters_modifyer >::const_iterator hit_type_it_e	= current_body_part_modifyer.hit_type_modifyers.end( );

		for ( ; hit_type_it != hit_type_it_e ; ++hit_type_it )
		{
			fixed_string<16>				hit_type_name				= hit_type_it->first;
			hit_type_parameters* 			current_hit_type_parameters = current_body_part_parameters->get_hit_parameters( hit_type_name.c_str( ) );
			hit_type_parameters_modifyer&	current_hit_type_modifyer	= current_body_part_modifyer.hit_type_modifyers[hit_type_name];

			current_hit_type_parameters->set_parameters(
				current_hit_type_modifyer.armor,
				current_hit_type_modifyer.reduce,
				current_hit_type_modifyer.absorption
			);
		}
	}

	inventory& invent = player->cast_to_inventory_holder( )->inventory( );

	for ( u32 i = 0; i < WEAPON_COUNT ; ++i )
	{
		inventory_item_ptr item = invent.item_in_slot( weapon_slots[i] );
		if ( !item )
			continue;

		weapon_core* wc = item->cast_weapon_core( );
		if ( !wc )
		{
			LOG_WARNING( "non-weapon item in weapon slot" );
			continue;
		}

		dispersion_calculator& dc = wc->get_dispersion_calculator( );
		dc.set_shooting_skill_coeff	( 1.0f + this->dispersion_correction_perc / 100.0f );
		dc.set_aiming_speed_coeff	( 1.0f + this->aiming_speed_correction_perc / 100.0f );
	}

	player->set_movement_speed_factor( 1.0f + movement_speed_correction_perc / 100.0f );

	player_stamina& stamn = player->stamina( );
	stamn.set_max_carried_weight		( stamn.get_max_carried_weight( ) + this->additional_max_weight );
	stamn.set_regeneration_speed_factor	(  1.0f + this->stamina_regen_correction_perc / 100.0f );

	body_part_parameters* bp = damage_model->get_body_part( "pain" );
	if ( bp )
	{
		const float health	= ( 1.0f + ( this->pain_healt_correction_perc / 100.0f ) ) * bp->get_max_health( );
		const float regen	= bp->get_regeneration_speed( );
		bp->set_parameters( health, regen );
	}
	else
		LOG_WARNING( "there's no 'pain' bodypart, pain health will not be scaled" );

	bodypart_health_regen_scale_predicate hr_predicate( 1.0f + this->health_regen_correction_perc / 100.0f );
	damage_model->m_body_parts.for_each( hr_predicate );

	player->usable_object_user_data( )->booster_artcont_time_factor = 1.0f + this->artcontainer_time_corr_perc / 100.0f;

	if ( this->anomaly_damage_corr_perc != 0.0f )
	{
		const float anomaly_scale = 1.0f + ( this->anomaly_damage_corr_perc / 100.0f );

		static pcstr anomaly_damage_types[4] = {
			"irradiation",
			"ambustion",
			"intoxication",
			"electric_shock",
		};
		for ( u32 i = 0 ; i != array_size( anomaly_damage_types ) ; ++i )
		{
			// claude@NOTE: target calls intrusive_ptr::operator* out-of-line here (so the loop
			// back-jump is its own statement, line 141); the base inlines operator* (with its
			// ASSERT) regardless of the *-vs--> spelling. inline-vs-call residual, not steerable.
			( *player->damage_model( ) ).add_damage_protector( anomaly_damage_types[i], anomaly_scale, 0.0f );
		}
	}

	player->usable_object_user_data( )->booster_engineer_use_time_factor = 1.0f + this->engineer_use_time_corr_perc / 100.0f;
}

float get_booster_value( boosters_enum booster_id, player_profile const& profile )
{
	for ( u8 i = 0 ; i < 11 ; ++i ) // sushi@TODO: Shouldn't be hardcoded like this. Instead there should be something like boosters_no constant, or something. Do that at some point.
	{
		if ( profile.boosters[i].id == booster_id )
			return profile.boosters[i].value;
	}
	return 0.0f;
}

struct player_parameters_cooker_data {
	/* 0x0000 */	player_profile const*		profile;
	/* 0x0004 */	items_dictionary*			dictionary;
}; // struct player_parameters_cooker_data

STATIC_SIZE_ASSERT(player_parameters_cooker_data, 0x8);

//
// player_parameters_modifyer_cook
//

player_parameters_modifyer_cook::player_parameters_modifyer_cook( ) :
	resources::translate_query_cook( resources::player_parameters_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

// sushi@TODO: Why don't we need to request for data here? Why is everything already in cooker_data?
void player_parameters_modifyer_cook::translate_query( resources::query_result_for_cook& parent )
{
	player_parameters_cooker_data* cooker_data = NULL;
	parent.user_data( )->try_get< player_parameters_cooker_data* >( cooker_data );

	player_parameters_modifyer* cooked_resource = VOSTOK_NEW_IMPL( g_allocator, player_parameters_modifyer );

	cooked_resource->dispersion_correction_perc     = get_booster_value( dispersion_correction_perc_id,     *cooker_data->profile );
	cooked_resource->aiming_speed_correction_perc   = get_booster_value( aiming_speed_correction_perc_id,   *cooker_data->profile );
	cooked_resource->health_regen_correction_perc   = get_booster_value( health_regen_correction_perc_id,   *cooker_data->profile );
	cooked_resource->stamina_regen_correction_perc  = get_booster_value( stamina_regen_correction_perc_id,  *cooker_data->profile );
	cooked_resource->movement_speed_correction_perc = get_booster_value( movement_speed_correction_perc_id, *cooker_data->profile );
	cooked_resource->additional_max_weight          = get_booster_value( additional_max_weight_id,          *cooker_data->profile );
	cooked_resource->pain_healt_correction_perc     = get_booster_value( pain_healt_correction_perc_id,     *cooker_data->profile );
	cooked_resource->artcontainer_time_corr_perc    = get_booster_value( artcontainer_time_corr_perc_id,    *cooker_data->profile );
	cooked_resource->anomaly_damage_corr_perc       = get_booster_value( anomaly_damage_corr_perc_id,       *cooker_data->profile );
	cooked_resource->engineer_use_time_corr_perc    = get_booster_value( engineer_use_time_corr_perc_id,    *cooker_data->profile );
	cooked_resource->engineer_succ_chance_corr_perc = get_booster_value( engineer_succ_chance_corr_perc_id, *cooker_data->profile );

	for ( u32 i = 0 ; i < max_slots_count ; ++i )
	{
		profile_slot_enum current_slot	= (profile_slot_enum)i;
		profile_slot const* slot		= &cooker_data->profile->slots[current_slot];
		if ( slot->item.id )
		{
			dictionary_item curr_item								= cooker_data->dictionary->item_by_id( slot->item.dict_id ); // sushi@MATCH: item_by_id didn't inline in target.
			configs::binary_config_value const& current_item_config	= curr_item.item_cfg->get_root( );

			if ( current_item_config.value_exists( "parameters" ) )
			{
				u32 count = curr_item.is_stack ? slot->item.condition_or_stack : 1;
				cooked_resource->total_items_weight += count * curr_item.weight;
			}

			if ( current_item_config.value_exists( "additional_slots" ) )
			{
				cooked_resource->additional_artefact_slots += (u8)current_item_config["additional_slots"]["artefact_slots"];
				cooked_resource->additional_devices_slots  += (u8)current_item_config["additional_slots"]["device_slots"];

				ASSERT( UNKNOWN_EXPRESSION );
			}

			if ( current_item_config.value_exists( "hit_params" ) )
			{
				configs::binary_config_value const* body_it		= current_item_config["hit_params"].begin( );
				configs::binary_config_value const* body_it_end	= current_item_config["hit_params"].end( );

				for ( ; body_it != body_it_end ; ++body_it )
				{
					body_part_parameters_modifyer		body_part_modifyer_from_cfg;
					configs::binary_config_value const& current_body_part_cfg	= *body_it;
					// sushi@TODO: Not sure what key does
					fixed_string< 16 > 					body_part_name			= current_body_part_cfg.key( );

					body_part_modifyer_from_cfg.health = current_body_part_cfg.value_exists( "health" )
						? (float)current_body_part_cfg["health"]
						: 0.0f;
					body_part_modifyer_from_cfg.health_regeneration = current_body_part_cfg.value_exists( "regeneration_speed" )
						? (float)current_body_part_cfg["regeneration_speed"]
						: 0.0f;

					if ( cooked_resource->body_part_parameters_modifyers.find( body_part_name ) != cooked_resource->body_part_parameters_modifyers.end( ) )
					{
						cooked_resource->body_part_parameters_modifyers[body_part_name].health				+= body_part_modifyer_from_cfg.health;
						cooked_resource->body_part_parameters_modifyers[body_part_name].health_regeneration += body_part_modifyer_from_cfg.health_regeneration;
					}
					else
						cooked_resource->body_part_parameters_modifyers[body_part_name] = body_part_modifyer_from_cfg; // sushi@MATCH: LTCG for map_assign as third member in class.

					body_part_parameters_modifyer& current_body_part_modifyer = cooked_resource->body_part_parameters_modifyers[body_part_name];

					if ( current_body_part_cfg.value_exists( "hit_types" ) )
					{
						configs::binary_config_value const* hit_type_it		= current_body_part_cfg["hit_types"].begin( );
						configs::binary_config_value const* hit_type_it_end	= current_body_part_cfg["hit_types"].end( );

						for ( ; hit_type_it != hit_type_it_end ; ++hit_type_it )
						{
							configs::binary_config_value const& current_hit_type_cfg = *hit_type_it;

							hit_type_parameters_modifyer hit_type_modifyer_from_cfg;

							fixed_string< 16 > hit_type_name = current_hit_type_cfg.key( );

							hit_type_modifyer_from_cfg.armor		= current_hit_type_cfg["armor"];
							hit_type_modifyer_from_cfg.absorption	= current_hit_type_cfg["absorption"];
							hit_type_modifyer_from_cfg.reduce		= current_hit_type_cfg["reduce"];
							// sushi@TODO: They meant hit_type_name lookup, xd
							if ( cooked_resource->body_part_parameters_modifyers.find( body_part_name ) != cooked_resource->body_part_parameters_modifyers.end( ) )
							{
								current_body_part_modifyer.hit_type_modifyers[hit_type_name].armor		+= hit_type_modifyer_from_cfg.armor;
								current_body_part_modifyer.hit_type_modifyers[hit_type_name].absorption	+= hit_type_modifyer_from_cfg.absorption;
								current_body_part_modifyer.hit_type_modifyers[hit_type_name].reduce		+= hit_type_modifyer_from_cfg.reduce;
							}
							else
								current_body_part_modifyer.hit_type_modifyers[hit_type_name]			= hit_type_modifyer_from_cfg;
						}
					}
				}
			}
		}
	}

	parent.set_unmanaged_resource( cooked_resource, resources::memory_usage_type( resources::nocache_memory, sizeof( player_parameters_modifyer ) ) );
	parent.finish_query( result_success, assert_on_fail_true );
}
void player_parameters_modifyer_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );
}

} // namespace survarium

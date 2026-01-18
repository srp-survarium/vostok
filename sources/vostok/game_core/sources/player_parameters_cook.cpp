////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
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

// STATE[84.21%|PARTIAL]
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
			player->damage_model( )->add_damage_protector( anomaly_damage_types[i], anomaly_scale, 0.0f ); // sushi@MATCH: The biggest diff is here. The order of things is different.
		}
	}

	player->usable_object_user_data( )->booster_engineer_use_time_factor = 1.0f + this->engineer_use_time_corr_perc / 100.0f;

	// FUNCTION BODY[0x5abc10]: 109
	// <0x5abc2a>|0x01a|+0x021:'40'	damage_model_ptr damage_model = player->damage_model( );
	// <0>
	// <0x5abc4b>|0x03b|+0x031:'42'	std::map< fixed_string<16>, body_part_parameters_modifyer >::const_iterator body_part_it	= body_part_parameters_modifyers.begin( );
	// <0x5abc7c>|0x06c|+0x025:'43'	std::map< fixed_string<16>, body_part_parameters_modifyer >::const_iterator body_part_end	= body_part_parameters_modifyers.begin( );
	// <0>
	// <0x5abca1>|0x091|+0x027:'45'	for ( ; body_part_it != body_part_end ; ++body_part_it )
	// <0>
	// <0x5abcc8>|0x0b8|+0x00e:'47'		fixed_string<16> body_part_name = body_part_it->first;
	// <0>
	// <0x5abcd6>|0x0c6|+0x01b:'49'		body_part_parameters* current_body_part_parameters =
	// <0>
	// <0x5abcf1>|0x0e1|+0x018:'51'		body_part_parameters_modifyer& 	current_body_part_modifyer		= body_part_parameters_modifyers[body_part_name];
	// <0>
	// <1>
	// <0x5abd09>|0x0f9|+0x059:'54'		current_body_part_parameters->set_parameters(
	// <0>
	// <0x5abd62>|0x152|+0x02b:'56'		std::map< fixed_string<16>,
	// <0x5abd8d>|0x17d|+0x01f:'57'		std::map< fixed_string<16>,
	// <0>
	// <0x5abdac>|0x19c|+0x023:'59'		for ( ; hit_type_it != hit_type_it_e ; ++hit_type_it )
	// <0>
	// <0x5abdcf>|0x1bf|+0x00e:'61'		fixed_string<16> hit_type_name = hit_type_it->first;
	// <0>
	// <0x5abddd>|0x1cd|+0x014:'63'		hit_type_parameters* 			current_hit_type_parameters = current_body_part_parameters->get_hit_parameters( hit_type_name.c_str( ) );
	// <0>
	// <0x5abdf1>|0x1e1|+0x012:'65'		hit_type_parameters_modifyer&	current_hit_type_modifyer	= current_body_part_modifyer.hit_type_modifyers[hit_type_name];
	// <0>
	// <0x5abe03>|0x1f3|+0x025:'67'
	// <0x5abe28>|0x218|+0x002:'68'		}
	// <0x5abe2a>|0x21a|+0x005:'69'	}
	// <0>
	// <1>
	// <0x5abe2f>|0x21f|+0x019:'72'		inventory& invent = player->cast_to_inventory_holder( );
	// <0>
	// <0x5abe48>|0x238|+0x01c|[1]:'74'	for ( u32 i = 0; i < 2 ; ++i )
	// <0>
	// <0x5abe64>|0x254|+0x01c:'76'			inventory_item_ptr item = invent.item_in_slot( weapon_slots[2] );
	// <0>
	// <0x5abe80>|0x270|+0x00f:'78'			if ( !item )
	// <0x5abe8f>|0x27f|+0x00a:'79'				continue;
	// <0>
	// <0x5abe99>|0x289|+0x024:'81'			weapon_core* wc = item->cast_weapon_core( );
	// <0>
	// <0x5abebd>|0x2ad|+0x00a:'83'			if ( !wc )
	// <0>
	// <0x5abec7>|0x2b7|+0x089:'85'
	// <0x5abf50>|0x340|+0x00d:'86'				continue;
	// <0>
	// <1>
	// <0x5abf5d>|0x34d|+0x00b:'89'
	// <0>
	// <0x5abf68>|0x358|+0x02c:'91'		dc.set_shooting_skill_coeff	( 1.0f + dispersion_correction_perc / 100.0f );
	// <0x5abf94>|0x384|+0x02c:'92'		dc.set_aiming_speed_coeff	( 1.0f + aiming_speed_correction_perc / 100.0f );
	// <0x5abfc0>|0x3b0|+0x00d:'93'	}
	// <0>
	// <1>
	// <0x5abfcd>|0x3bd|+0x039:'96'		player->set_movement_speed_factor( 1.0f + movement_speed_correction_perc / 100.0f );
	// <0>
	// <1>
	// <0x5ac006>|0x3f6|+0x010:'99'		player_stamina& stamn = player->stamina( );
	// <0>
	// <1>
	// <0x5ac016>|0x406|+0x03e:'102'	stamn.set_max_carried_weight( stamn.get_max_carried_weight( ) + this->additional_max_weight );
	// <0>
	// <1>
	// <0x5ac054>|0x444|+0x02c:'105'	stamn.set_regeneration_speed_factor	(  1.0f + this->stamina_regen_correction_perc / 100.0f );
	// <0>
	// <1>
	// <0x5ac080>|0x470|+0x017:'108'	body_part_parameters* bp = damage_model->get_body_part["pain"];
	// <0x5ac097>|0x487|+0x006:'109'	if ( bp )
	// <0>								{
	// <0x5ac09d>|0x48d|+0x041:'111'		const float health	= ( 1.0f + ( this->p
	// <0x5ac0de>|0x4ce|+0x010:'112'		const float regen	= bp->get_regeneration_speed( );
	// <0>
	// <0x5ac0ee>|0x4de|+0x019:'114'		bp->set_parameters( health, regen );
	// <0>								}
	// <0x5ac107>|0x4f7|+0x005:'116'	else
	// <0x5ac10c>|0x4fc|+0x089:'117'		LOG_WARNING( "there's no 'pain' bodypart, pain health will not be scaled" );
	// <0>
	// <1>
	// <0x5ac195>|0x585|+0x033:'120'	bodypart_health_regen_scale_predicate	hr_predicate( 1.0f + this->health_regen_correction_perc / 100.0f );
	// <0x5ac1c8>|0x5b8|+0x045:'121'	damage_model->m_body_parts.for_each( hr_predicate );
	// <0>
	// <1>
	// <0x5ac20d>|0x5fd|+0x036:'124'	player->usable_object_user_data( )->booster_artcont_time_factor = 1.0f + this->artcontainer_time_corr_perc / 100.0f );
	// <0>
	// <1>
	// <0x5ac243>|0x633|+0x01f:'127'	if ( this->anomaly_damage_corr_perc != 0.0f )
	// <0>
	// <0x5ac262>|0x652|+0x026:'129'		const float anomaly_scale = 1.0f + ( this->anomaly_damage_corr_perc / 100.0f );
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5ac288>|0x678|+0x028|[3]:'138'	for ( u32 i = 0 ; i != array_size( anomaly_damage_types ) ; ++i )
	// <0>
	// <0x5ac2b0>|0x6a0|+0x037:'140'			player->damage_model( )->add_damage_protector( anomaly_damage_types[i], anomaly_scale, 0.0f );
	// <0x5ac2e7>|0x6d7|+0x002:'141'		}
	// <0>
	// <1>
	// <2>
	// <0x5ac2e9>|0x6d9|+0x036:'145'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[BLOCKED]
float get_booster_value( boosters_enum booster_id, player_profile const& profile )
{
	for ( u8 i = 0 ; i < 11 ; ++i ) // sushi@TODO: Shouldn't be hardcoded like this. Instead there should be something like boosters_no constant, or something. Do that at some point.
	{
		if ( profile.boosters[i].id == booster_id )
			return profile.boosters[i].value;
	}
	return 0.0f;

	// FUNCTION BODY
	// <0x5ab294>|0x004|+0x017|[1]:'153'
	// <0>
	// <0x5ab2ab>|0x01b|+0x011:'155'
	// <0x5ab2bc>|0x02c|+0x00d:'156'
	// <0x5ab2c9>|0x039|+0x002:'157'
	// <0x5ab2cb>|0x03b|+0x002:'158'
	// ******
}

struct player_parameters_cooker_data {
	/* 0x0000 */	player_profile const*		profile;
	/* 0x0004 */	items_dictionary*			dictionary;
}; // struct player_parameters_cooker_data

STATIC_SIZE_ASSERT(player_parameters_cooker_data, 0x8);

//
// player_parameters_modifyer_cook
//

// STATE[66.69%|PARTIAL]
player_parameters_modifyer_cook::player_parameters_modifyer_cook( ) :
	resources::translate_query_cook( resources::player_parameters_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );

	// FUNCTION BODY
	// <0x5ab315>|0x035|+0x00c:'165'
	// ******
}

// STATE[89.62%|PARTIAL] sushi@TODO: Why don't we need to request for data here? Why is everything already in cooker_data?
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

	// FUNCTION BODY[0x5ab360]: 105
	// <0x5ab37b>|0x01b|+0x007:'170'
	// <0x5ab382>|0x022|+0x013:'171'
	// <0>
	// <0x5ab395>|0x035|+0x06f:'173'	player_parameters_modifyer* cooked_resource = VOSTOK_NEW_IMPL( g_allocat
	// <0>
	// <0x5ab404>|0x0a4|+0x019:'175'	cooked_resource->dispersion_correction_perc
	// <0x5ab41d>|0x0bd|+0x019:'176'
	// <0x5ab436>|0x0d6|+0x019:'177'
	// <0x5ab44f>|0x0ef|+0x019:'178'
	// <0x5ab468>|0x108|+0x019:'179'
	// <0x5ab481>|0x121|+0x019:'180'
	// <0x5ab49a>|0x13a|+0x019:'181'
	// <0x5ab4b3>|0x153|+0x019:'182'
	// <0x5ab4cc>|0x16c|+0x019:'183'
	// <0x5ab4e5>|0x185|+0x019:'184'
	// <0x5ab4fe>|0x19e|+0x019:'185'
	// <0>
	// <1>
	// <2>
	// <0x5ab517>|0x1b7|+0x01c|[1]:'189'	for ( u32 i = 0 ; i < 0x13 ; ++i )
	// <0>
	// <0x5ab533>|0x1d3|+0x006:'191'			profile_slot_enum current_slot = (profile_slot_enum)i;
	// <0x5ab539>|0x1d9|+0x015:'192'
	// <0x5ab54e>|0x1ee|+0x00d:'193'			if ( slot->item.id )
	// <0>
	// <0x5ab55b>|0x1fb|+0x01f:'195'				dictionary_item curr_item								= cooker_data->dictionary->item_by_id( slot->item.dict_id );
	// <0x5ab57a>|0x21a|+0x013:'196'				configs::binary_config_value const& current_item_config	= curr_item.item_cfg->get_root( );
	// <0>
	// <0x5ab58d>|0x22d|+0x014:'198'				if ( current_item_config.value_exists( "parameters" ) )
	// <0>
	// <1>
	// <0x5ab5a1>|0x241|+0x02b:'201'					u32 count = curr_item.is_stack
	// <0>
	// <0x5ab5cc>|0x26c|+0x031:'203'					cooked_resource->total_items_weight += count * curr_item.weight;
	// <0>
	// <1>
	// <0x5ab5fd>|0x29d|+0x014:'206'	if ( current_item_config.value_exists( "additional_slots" ) )
	// <0>								{
	// <0x5ab611>|0x2b1|+0x036:'208'		cooked_resource->additional_artefact_slots
	// <0x5ab647>|0x2e7|+0x036:'209'		cooked_resource->additional_devices_slots
	// <0>
	// <0x5ab67d>|0x31d|+0x012:'211'		ASSERT( UNKNOWN_EXPRESSION );
	// <0>								}
	// <0x5ab68f>|0x32f|+0x018:'213'	if ( current_item_config.value_exists( "hit_params" ) )
	// <0>
	// <0x5ab6a7>|0x347|+0x01a:'215'		configs::binary_config_value const* body_it
	// <0x5ab6c1>|0x361|+0x01a:'216'		configs::binary_config_value const* body_it_end
	// <0>
	// <0x5ab6db>|0x37b|+0x023:'218'		for ( ; body_it != body_it_end ; ++body_it )
	// <0>
	// <0x5ab6fe>|0x39e|+0x017:'220'			body_part_parameters_modifyer body_part_modifyer_from_cfg;
	// <0x5ab715>|0x3b5|+0x00c:'221'			configs::binary_config_value const& current_body_part_cfg = *body_it;
	// <0>
	// <0x5ab721>|0x3c1|+0x017:'223'			fixed_string< 16 > 					body_part_name
	// <0>
	// <0x5ab738>|0x3d8|+0x056:'225'		body_part_modifyer_from_cfg.health
	// <0x5ab78e>|0x42e|+0x056:'226'		body_part_modifyer_from_cfg.health_regeneration
	// <0>
	// <0x5ab7e4>|0x484|+0x07f:'228'	if ( cooked_resource->body_part_parameters_
	// <0>
	// <0x5ab863>|0x503|+0x037:'230'
	// <0x5ab89a>|0x53a|+0x03a:'231'
	// <0>
	// <0x5ab8d4>|0x574|+0x002:'233'	else
	// <0x5ab8d6>|0x576|+0x055:'234'		cooked_resource->body_part_parameters_modifyers[body_part_name] = body_part_modifyer_from_cfg;
	// <0>
	// <0x5ab92b>|0x5cb|+0x01b:'236'	body_part_parameters_modifyer& current_body_part_modifyer
	// <0>
	// <0x5ab946>|0x5e6|+0x01b:'238'	if ( current_body_part_cfg.value_exists( "hit_types" ) )
	// <0>
	// <0x5ab961>|0x601|+0x01d:'240'		configs::binary_config_value const* hit_type_it
	// <0x5ab97e>|0x61e|+0x01d:'241'		configs::binary_config_value const* hit_type_it_end
	// <0>
	// <0x5ab99b>|0x63b|+0x023:'243'		for ( ; hit_type_it != hit_type_it_end ; ++hit_type_it )
	// <0>
	// <0x5ab9be>|0x65e|+0x00c:'245'			configs::binary_config_value const& current_hit_type_cfg = *hit_type_it;
	// <0>
	// <1>
	// <2>
	// <0x5ab9ca>|0x66a|+0x017:'249'			fixed_string< 16 > hit_type_name = current_hit_type_cfg.key( );
	// <0>
	// <0x5ab9e1>|0x681|+0x01d:'251'			hit_type_modifyer_from_cfg.armor		= current_hit_type_cfg["armor"];
	// <0x5ab9fe>|0x69e|+0x01d:'252'
	// <0x5aba1b>|0x6bb|+0x01d:'253'			hit_type_modifyer_from_cfg.reduce		= current_hit_type_cfg["reduce"];
	// <0>
	// <0x5aba38>|0x6d8|+0x083:'255'		if ( cooked_resource->body_part_parameters_modifyers.find( body_part_name ) != cooked_resource->body_part_parameters_modifyers.end( ) )
	// <0>
	// <0x5ababb>|0x75b|+0x037:'257'
	// <0x5abaf2>|0x792|+0x03a:'258'
	// <0x5abb2c>|0x7cc|+0x03a:'259'
	// <0>
	// <0x5abb66>|0x806|+0x002:'261'		else
	// <0x5abb68>|0x808|+0x034:'262'			current_body_part_modifyer.hit_type_modifyers["hit_type_name"]				= hit_type_modifyer_from_cfg;
	// <0>
	// <1>
	// <0x5abb9c>|0x83c|+0x010:'265'	}
	// <0>
	// <0x5abbac>|0x84c|+0x00b:'267'	// clears curr_item
	// <0x5abbb7>|0x857|+0x005:'268'	}	for ( u32 i = 0 ; i < max_slots_count ; ++i )
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5abbbc>|0x85c|+0x036:'273'
	// <0x5abbf2>|0x892|+0x00c:'274'	parent.finish_query( result_success, assert_on_fail_true );
	// ******
}
// STATE[31.00%|PARTIAL]
void player_parameters_modifyer_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );

	// FUNCTION BODY
	// <0x5ab339>|0x009|+0x017:'279'
	// ******
}

} // namespace survarium

// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/inventory_cook.h>

#include <vostok/game_core/booby_trap_set_cook_data.h>
#include <vostok/game_core/damage_model_cook.h>
#include <vostok/game_core/dictionary_item.h>
#include <vostok/game_core/inventory.h>
#include <vostok/game_core/inventory_cooker_data.h>
#include <vostok/game_core/items_cook.h>
#include <vostok/game_core/items_dictionary.h>
#include <vostok/game_core/item_types_enum.h>
#include <vostok/game_core/profile_slot_enum.h>
#include <vostok/game_core/profile_slot.h>
#include <vostok/game_core/profile_slots.h>
#include <vostok/game_core/player_profile.h>
#include <vostok/game_core/weapon_ammunition_cook.h>
#include <vostok/game_core/weapon_core.h>

namespace survarium {

inventory_cook::inventory_cook( ) :
	resources::translate_query_cook( resources::inventory_class, reuse_false, use_any_thread_id )
{
	resources::register_cook( this );
	static items_cook				s_items_cook;
	static damage_model_cook 		s_damage_model_cook;
	static weapon_ammunition_cook	s_weapon_ammunition_cook;;
}

void inventory_cook::translate_query( resources::query_result_for_cook& parent )
{

	inventory_cooker_data* cooker_data;
	parent.user_data( )->try_get( cooker_data ); // sushi@TODO
	ASSERT( UNKNOWN_EXPRESSION_T( cooker_data ) );
	buffer_vector< resources::request > requests	( ALLOCA( max_slots_count * sizeof( resources::request ) ), max_slots_count );

	buffer_vector< variant<32> const* > user_data	( ALLOCA( max_slots_count * sizeof( variant<32> const* ) ), max_slots_count );
	variant<32> 						v[max_slots_count];

	pcstr				resource_name;
	profile_slot*		slot;


	for ( u32 i = 0 ; i < WEAPON_COUNT ; ++i )
	{
		profile_slot_enum	current = weapon_slots[i];
		slot						= &cooker_data->profile->slots[current];
		if ( slot->item.id )
		{
			resource_name = cooker_data->dictionary->item_by_id( slot->item.dict_id ).item_cfg_name.c_str( );		// sushi@MATCH:
			requests.push_back( resources::create_request( resource_name, resources::weapon_class ) );	// sushi@MATCH:
			user_data.push_back( NULL );
		}
	}

	for ( u32 i = 0 ; i < AMMO_COUNT ; ++i )
	{
		profile_slot_enum	current = ammunition_slots[i];
		slot						= &cooker_data->profile->slots[current];
		if ( slot->item.id )
		{
			resource_name = cooker_data->dictionary->item_by_id( slot->item.dict_id ).item_cfg_name.c_str( );
			requests.push_back( resources::create_request( resource_name, resources::weapon_ammunition_class ) );
			user_data.push_back( NULL );
		}
	}

	for ( u32 i = 0 ; i < ITEM_COUNT ; ++i )
	{
		profile_slot_enum	current = item_slots[i];
		slot						= &cooker_data->profile->slots[current];
		if ( slot->item.id )
		{
			dictionary_item const&	desc		= cooker_data->dictionary->item_by_id( slot->item.dict_id );
			item_types_enum 		item_type	= (item_types_enum)(u32)desc.item_cfg->get_root( )["data"]["type"];

			resource_name						= cooker_data->dictionary->item_by_id( slot->item.dict_id ).item_cfg_name.c_str( );

			resources::class_id_enum class_id;
			switch ( item_type )
			{
				case item_type_base_trap:
				{
					class_id = resources::booby_trap_set_class;

					ASSERT( UNKNOWN_EXPRESSION );

					booby_trap_set_cook_data	booby_trap_set_data;
					booby_trap_set_data.is_local_player = cooker_data->profile->is_local;
					booby_trap_set_data.stack_size		= (u8)slot->item.condition_or_stack;

					v[current].set( booby_trap_set_data );
					user_data.push_back( &v[current] );
					break;
				}
				case item_type_foo:
				{
					class_id = resources::unknown_data_class;
					break;
				}
				case item_type_rifle_scope:
				{
					class_id = resources::rifle_scope_class;
					break;
				}
				default:
				{
					class_id = resources::item_class;
					break;
				}
			}

			requests.push_back( resources::create_request( resource_name, class_id ) );
			if ( item_type != item_type_base_trap )
				user_data.push_back( NULL );
		}
	}

	if ( requests.empty( ) )
	{
		requests.push_back( resources::create_request( "", resources::unknown_data_class ) );
		user_data.push_back( NULL );
	}

	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind( &inventory_cook::on_subresources_loaded, this, _1, cooker_data ),
		g_allocator,
		user_data.begin( ),
		&parent
	);
}

void inventory_cook::on_subresources_loaded( resources::queries_result& data, inventory_cooker_data* cooker_data )
{
	ASSERT( UNKNOWN_EXPRESSION );
	inventory* result = VOSTOK_NEW_IMPL( g_allocator, inventory );
	u32 result_index = 0;

	profile_slot* slot;

	for ( u32 i = 0 ; i < WEAPON_COUNT ; ++i )
	{
		profile_slot_enum	current = weapon_slots[i];
		slot						= &cooker_data->profile->slots[current];
		if ( slot->item.id )
		{
			ASSERT( UNKNOWN_EXPRESSION );
			weapon_core_ptr wpn = static_cast_resource_ptr< weapon_core_ptr >( data[result_index].get_unmanaged_resource( ) );	// sushi@MATCH:
			++result_index;
			ASSERT( UNKNOWN_EXPRESSION );
			wpn->set_dict_id( slot->item.dict_id ); // sushi@TODO: Check why this isn't set by weapon cooker.
			inventory_item_ptr iitem = static_cast_resource_ptr< inventory_item_ptr >( wpn );							// sushi@MATCH
			iitem->set_amount( (u16)slot->item.condition_or_stack );
			result->set_item( current, iitem );
		}
	}

	for ( u32 i = 0 ; i < AMMO_COUNT ; ++i )
	{
		profile_slot_enum	current = ammunition_slots[i];
		slot						= &cooker_data->profile->slots[current];
		if ( slot->item.id )
		{
			ASSERT( UNKNOWN_EXPRESSION );
			inventory_item_ptr iitem = static_cast_resource_ptr< inventory_item_ptr >( data[result_index].get_unmanaged_resource( ) );
			++result_index;
			iitem->set_dict_id( slot->item.dict_id ); // sushi@TODO: Why no set_amount, like it was set for weapon?
			result->set_item( current, iitem );
		}
	}

	for ( u32 i = 0 ; i < ITEM_COUNT ; ++i )
	{
		profile_slot_enum	current = item_slots[i];
		slot						= &cooker_data->profile->slots[current];
		if ( slot->item.id )
		{
			if ( data[result_index].get_class_id( ) )
			{
				inventory_item_ptr iitem = static_cast_resource_ptr< inventory_item_ptr >( data[result_index].get_unmanaged_resource( ) );
				iitem->set_dict_id( slot->item.dict_id );
				result->set_item( current, iitem );
			}
			++result_index;
		}
	}

	resources::query_result_for_cook* parent = data.get_parent_query( );
	parent->set_unmanaged_resource( result, resources::memory_usage_type( resources::nocache_memory, sizeof( inventory ) ) );
	parent->finish_query( result_success );
}

void inventory_cook::delete_resource( resources::resource_base* resource )
{
	inventory* inventory = static_cast_checked< survarium::inventory* >( resource );
	VOSTOK_DELETE_IMPL( g_allocator, inventory );
}

} // namespace survarium

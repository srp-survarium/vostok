////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

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

// STATE[84.05%|DONE]
inventory_cook::inventory_cook( ) :
	resources::translate_query_cook( resources::inventory_class, reuse_false, use_any_thread_id )
{
	resources::register_cook( this );
	static items_cook				s_items_cook;
	static damage_model_cook 		s_damage_model_cook;
	static weapon_ammunition_cook	s_weapon_ammunition_cook;;

	// FUNCTION BODY
	// <0x764e45>|0x035|+0x00c:'29'
	// <0x764e51>|0x041|+0x02f:'30'
	// <0x764e80>|0x070|+0x031:'31'
	// <0x764eb1>|0x0a1|+0x030:'32'
	// ******
}

// STATE[92.44%|PARTIAL]
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

	// FUNCTION BODY[0x765330]: 100
	// <0>
	// <0x765350>|0x020|+0x013:'38'		parent.user_data( )->try_get( cooker_data );
	// <0x765363>|0x033|+0x00c:'39'		ASSERT( UNKNOWN_EXPRESSION_T( cooker_data ) );
	// <0x76536f>|0x03f|+0x021:'40'		buffer_vector< resources::request > requests	( ALLOCA( max_slots_count * sizeof( resources::request ) ), max_slots_count );
	// <0>
	// <0x765390>|0x060|+0x021:'42'		buffer_vector< variant<32> const* > user_data	( ALLOCA( max_slots_count * sizeof( variant<32> const* ) ), max_slots_count );
	// <0x7653b1>|0x081|+0x015:'43'		variant<32> 						v[max_slots_count];
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7653c6>|0x096|+0x028|[1]:'49'	for ( u32 i = 0 ; i < WEAPON_COUNT ; ++i )
	// <0>
	// <0x7653ee>|0x0be|+0x013|[2]:'51'		profile_slot_enum	current = weapon_slots[i];
	// <0x765401>|0x0d1|+0x01b:'52'			profile_slot*		slot	= cooker_data->profile->slots[current];
	// <0x76541c>|0x0ec|+0x00c:'53'			if ( slot->item.id )
	// <0>									{
	// <0x765428>|0x0f8|+0x023:'55'				resource_name = cooker_data->dictionary->ite
	// <0x76544b>|0x11b|+0x02a:'56'				requests.push_back( resour
	// <0x765475>|0x145|+0x018:'57'				user_data.push_back( NULL );
	// <0>									}
	// <0x76548d>|0x15d|+0x005:'59'		}
	// <0>
	// <0x765492>|0x162|+0x028|[2]:'61'	for ( u32 i = 0 ; i < AMMO_COUNT ; ++i )
	// <0>								{
	// <0x7654ba>|0x18a|+0x013|[3]:'63'		profile_slot_enum	current = ammunition_slots[i];
	// <0x7654cd>|0x19d|+0x01b:'64'			slot						= &cooker_data->profile->slots[current];
	// <0x7654e8>|0x1b8|+0x00c:'65'			if ( slot->item.id )
	// <0>
	// <0x7654f4>|0x1c4|+0x023:'67'
	// <0x765517>|0x1e7|+0x02a:'68'				requests.push_back( resources::create_request( resource_name, resources::weapon_ammunition_class ) );
	// <0x765541>|0x211|+0x018:'69'				user_data.push_back( NULL );
	// <0>
	// <0x765559>|0x229|+0x005:'71'
	// <0>
	// <0x76555e>|0x22e|+0x028|[3]:'73'	for ( u32 i = 0 ; i < ITEM_COUNT ; ++i )
	// <0>
	// <0x765586>|0x256|+0x013|[4]:'75'		profile_slot_enum	current = item_slots[i];
	// <0x765599>|0x269|+0x01b:'76'			slot						= &cooker_data->profile->slots[current];
	// <0x7655b4>|0x284|+0x010:'77'			if ( slot->item.id )
	// <0>									{
	// <0x7655c4>|0x294|+0x01b|[5]:'79'			dictionary_item const&	desc		= cooker_data->dictionary->item_by_id( slot->item.dict_id );
	// <0x7655df>|0x2af|+0x036:'80'				item_types_enum 		item_type	= (item_types_enum)(u32)desc.item_cfg->get_root( )["data"]["type"];
	// <0>
	// <0x765615>|0x2e5|+0x023:'82'				resource_name						= cooker_data->dictionary->item_by_id( slot->item.dict_id ).item_cfg_name.c_str( );
	// <0>
	// <1>
	// <0x765638>|0x308|+0x034:'85'		switch ( item_type )
	// <0>								{
	// <1>									case item_type_base_trap:
	// <2>									{
	// <0x76566c>|0x33c|+0x00a|[6]:'89'			class_id = booby_trap_set_class;
	// <0>
	// <0x765676>|0x346|+0x012:'91'				ASSERT( UNKNOWN_EXPRESSION );
	// <0>
	// <1>
	// <0x765688>|0x358|+0x011:'94'				booby_trap_set_data.is_local_player = cooker_data->profile->is_local;
	// <0x765699>|0x369|+0x00e:'95'				booby_trap_set_data.stack_size		= slot->item.condition_or_stack;
	// <0x7656a7>|0x377|+0x01c:'96'				v[current].set( booby_trap_set_data );
	// <0x7656c3>|0x393|+0x024:'97'				user_data.push_back( &v[current] );
	// <0x7656e7>|0x3b7|+0x002:'98'				break;
	// <0>									}
	// <1>									case item_type_foo:
	// <2>									{
	// <0x7656e9>|0x3b9|+0x00a:'102'			class_id = resources::unknown_data_class;
	// <0x7656f3>|0x3c3|+0x002:'103'			break;
	// <0>									}
	// <1>									case item_type_rifle_scope:
	// <2>									{
	// <0x7656f5>|0x3c5|+0x00a:'107'			class_id = resources::rifle_scope_class;
	// <0x7656ff>|0x3cf|+0x002:'108'			break;
	// <0>									}
	// <1>									default:
	// <2>									{
	// <0x765701>|0x3d1|+0x00a:'112'			class_id = resources::item_class;
	// <0>										break;
	// <1>									}
	// <2>								}
	// <0x76570b>|0x3db|+0x02b:'116'	requests.push_back( resources::
	// <0>
	// <0x765736>|0x406|+0x009:'118'	if ( item_type != item_type_base_trap )
	// <0x76573f>|0x40f|+0x018:'119'		user_data.push_back( NULL );
	// <0>
	// <0x765757>|0x427|+0x005:'121'	}
	// <0>
	// <0x76575c>|0x42c|+0x012:'123'
	// <0>
	// <0x76576e>|0x43e|+0x026:'125'
	// <0x765794>|0x464|+0x018:'126'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x7657ac>|0x47c|+0x0db:'135'
	// <0>
	// ******
}

// STATE[73.38%|PARTIAL]
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

	// FUNCTION BODY[0x764f30]: 62
	// <0x764f40>|0x010|+0x00c:'141'
	// <0x764f4c>|0x01c|+0x05a:'142'
	// <0x764fa6>|0x076|+0x007:'143'		u32 result_index = 0;
	// <0>
	// <1>
	// <2>
	// <0x764fad>|0x07d|+0x01c|[1]:'147'	for ( u32 i = 0 ; i < WEAPON_COUNT ; ++i )
	// <0>
	// <0x764fc9>|0x099|+0x00d|[2]:'149'		profile_slot_enum	current = weapon_slots[i];
	// <0x764fd6>|0x0a6|+0x015:'150'			slot						= &cooker_data->profile->slots[current];
	// <0>
	// <0x764feb>|0x0bb|+0x00d:'152'			if ( slot->item.id )
	// <0>										{
	// <0x764ff8>|0x0c8|+0x00c|[3]:'154'			ASSERT( UNKNOWN_EXPRESSION );
	// <0x765004>|0x0d4|+0x036:'155'				weapon_core_ptr wpn = static_cast_resource_ptr< weapon_core_ptr >( data[result_index].get_unmanaged_resource( ) );
	// <0x76503a>|0x10a|+0x009:'156'				++result_index;
	// <0x765043>|0x113|+0x00c:'157'				ASSERT( UNKNOWN_EXPRESSION );
	// <0x76504f>|0x11f|+0x02b:'158'				wpn->set_dict_id( slot->item.id );
	// <0x76507a>|0x14a|+0x019:'159'				inventory_item_ptr iitem = static_cast_resource_ptr< inventory_item_ptr>( wpn );
	// <0x765093>|0x163|+0x013:'160'				iitem->set_amount( (u16)slot->item.condition_or_stack );
	// <0x7650a6>|0x176|+0x010:'161'				result->set_item( current, iitem );
	// <0x7650b6>|0x186|+0x010:'162'			}
	// <0x7650c6>|0x196|+0x005:'163'		}
	// <0>
	// <1>
	// <0x7650cb>|0x19b|+0x01c|[2]:'166'	for ( u32 i = 0 ; i < AMMO_COUNT ; ++i )
	// <0>
	// <0x7650e7>|0x1b7|+0x00d|[3]:'168'		profile_slot_enum	current = ammunition_slots[i];
	// <0x7650f4>|0x1c4|+0x015:'169'			slot						= &cooker_data->profile->slots[current];
	// <0>
	// <0x765109>|0x1d9|+0x00d:'171'			if ( slot->item.id )
	// <0>
	// <0x765116>|0x1e6|+0x00c|[4]:'173'			ASSERT( UNKNOWN_EXPRESSION );
	// <0x765122>|0x1f2|+0x055:'174'				inventory_item_ptr iitem = static_cast_resource_ptr< inventory_item_ptr >( data[result_index].get_unmanaged_resource( ) );
	// <0x765177>|0x247|+0x009:'175'				++result_index;
	// <0x765180>|0x250|+0x030:'176'				iitem->set_dict_id( slot->item.dict_id );
	// <0x7651b0>|0x280|+0x010:'177'				result->set_item( current, iitem );
	// <0x7651c0>|0x290|+0x008:'178'			}
	// <0x7651c8>|0x298|+0x005:'179'		}
	// <0>
	// <1>
	// <0x7651cd>|0x29d|+0x01c|[3]:'182'	for ( u32 i = 0 ; i < ITEM_COUNT ; ++i )
	// <0>
	// <0x7651e9>|0x2b9|+0x00d|[4]:'184'		profile_slot_enum	current = item_slots[i];
	// <0x7651f6>|0x2c6|+0x015:'185'			slot						= &cooker_data->profile->slots[current];
	// <0>
	// <0x76520b>|0x2db|+0x00d:'187'			if ( slot->item.id )
	// <0>
	// <0x765218>|0x2e8|+0x019:'189'				if ( data[result_index].get_class_id( ) )
	// <0>											{
	// <0x765231>|0x301|+0x055|[5]:'191'				inventory_item_ptr iitem = static_cast_resource_ptr< inventory_item_ptr >( data[result_index].get_unmanaged_resource( ) );
	// <0x765286>|0x356|+0x030:'192'					iitem->set_dict_id( slot->item.dict_id );
	// <0x7652b6>|0x386|+0x010:'193'					result->set_item( current, iitem );
	// <0x7652c6>|0x396|+0x008:'194'				}
	// <0x7652ce>|0x39e|+0x009:'195'				++result_index;
	// <0>										}
	// <0x7652d7>|0x3a7|+0x005:'197'		}
	// <0>
	// <0x7652dc>|0x3ac|+0x00b:'199'		resources::query_result_for_cook* parent = data.get_parent_query( );
	// <0x7652e7>|0x3b7|+0x033:'200'
	// <0x76531a>|0x3ea|+0x00c:'201'
	// <0>
	// ******
}

// STATE[100%|DONE]
void inventory_cook::delete_resource( resources::resource_base* resource )
{
	inventory* inventory = static_cast_checked< survarium::inventory* >( resource );
	VOSTOK_DELETE_IMPL( g_allocator, inventory );

	// FUNCTION BODY[0x764ef0]: 2
	// <0x764ef9>|0x009|+0x006:'207'
	// <0x764eff>|0x00f|+0x026:'208'
	// ******
}

} // namespace survarium

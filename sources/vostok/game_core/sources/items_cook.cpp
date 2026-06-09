////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/items_cook.h>

#include <vostok/game_core/medkit.h>
#include <vostok/game_core/oxygen_tank.h>
#include <vostok/game_core/artefact_lifebone_core.h>

namespace survarium {

// STATE[100%|DONE]
items_cook::items_cook( ) :
	resources::translate_query_cook( resources::item_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

// STATE[99.85%|DONE]: LTCG frame-padding on the boost functor temp; statements aligned 4/4
// STRUCTURE DIFF[target 0x751b20 | base 0x566c10]: target 4 / base 4 stmts
// .. same ..
// ; aligned 4, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole residual is a 0xC frame-size delta (base sub esp,178h vs target 16Ch) on the boost::bind/boost::function temp storage; non-steerable LTCG. trail: translate_query.md
void items_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string config_name;
	config_name.assignf( "resources/%s", parent.get_requested_path( ) );

	resources::query_resource(
		config_name.c_str( ),
		resources::binary_config_class_impl,
		boost::bind( &items_cook::on_config_ready, this, _1, &parent ),
		g_allocator,
		parent.user_data( ),
		&parent
	);
}

// STATE[31%|DONE]: delete_helper arg-passing differs (register vs stack) - call-boundary LTCG
// STRUCTURE DIFF[target 0x7516c0 | base 0x5667c0]: target 1 / base 1 stmts
// 0x009 <0x17> | 0x00a <0x16> | VOSTOK_DELETE_IMPL( g_allocator, resource );   SIZE
// ; aligned 0, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (1/1 stmt) - target passes resource via stack (lea [ebp+8]; push; add esp,8) while base uses edi/register (add esp,4) into delete_helper<doug_lea_allocator,resource_base>; call-boundary arg passing, non-steerable LTCG (shared cook-base wall). trail: delete_resource.md
void items_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );
}

// STATE[93.21%|DONE]: resource_ptr by-value temp materialized+destroyed in target, elided in base
// STRUCTURE DIFF[target 0x751a60 | base 0x566b60]: target 6 / base 5 stmts
// .. same ..
// 0x016 <0x22> | 0x016 <0x1d> | configs::binary_config_ptr config		= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );   SIZE
// .. same ..
// <0>         | --          |    EMPTY only target
// .. same ..
// ; aligned 4, size-diffs 1, quantity-diffs 1
// VERDICT: STRUCTURE MATCH (shape ok) - get_unmanaged_resource() returns resource_ptr by value; target materializes the temp at [ebp-28h] and destroys it (call ::dec) before passing &temp to static_cast_resource_ptr, base elides it (add esp,4). resource_ptr by-value temp, non-steerable LTCG. trail: on_config_ready.md
void items_cook::on_config_ready( resources::queries_result& data, resources::query_result_for_cook* parent )
{
	ASSERT( UNKNOWN_EXPRESSION );
	configs::binary_config_ptr config		= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	configs::binary_config_value current	= config->get_root( )["data"];
	item_types_enum item_type				= (item_types_enum)(u32)current["type"];
	create_item_and_finish_query( item_type, config, parent );
}

// STATE[BLOCKED]
void items_cook::create_item_and_finish_query( item_types_enum item_type, configs::binary_config_ptr config, resources::query_result_for_cook* parent )
{
	result_enum result	= result_undefined;

	u16 item_dict_id	= 0;
	if ( parent->user_data( ) )
		parent->user_data( )->try_get<u16>( item_dict_id );

	switch ( item_type )
	{
		case item_type_base_medkit:
		{
			medkit* resource = VOSTOK_NEW_IMPL( g_allocator, medkit );
			resource->load( config->get_root( )["data"] );
			if ( item_dict_id )
				resource->set_dict_id( item_dict_id );

			parent->set_unmanaged_resource( resource, resources::memory_usage_type( resources::nocache_memory, sizeof( medkit ) ) );
			result = result_success;
			break;
		}

		case item_type_oxygen_tank:
		{
			oxygen_tank* resource = VOSTOK_NEW_IMPL( g_allocator, oxygen_tank );
			resource->load( config->get_root( )["data"] );
			parent->set_unmanaged_resource( resource, resources::memory_usage_type( resources::nocache_memory, sizeof( oxygen_tank ) ) );
			result = result_success;
			break;
		}

		case item_type_base_trap:
		{
			ASSERT( UNKNOWN_EXPRESSION );
			result = result_error;
			break;
		}

		case item_type_artefact_lifebone:
		{
			artefact_lifebone_core* resource = VOSTOK_NEW_IMPL( g_allocator, artefact_lifebone_core );
			resource->load_core( config->get_root( )["data"] );
			if ( item_dict_id )
				resource->set_dict_id( item_dict_id );

			parent->set_unmanaged_resource( resource, resources::memory_usage_type( resources::nocache_memory, sizeof( artefact_lifebone_core ) ) );
			result = result_success;
			break;
		}

		case item_type_foo:
			result = result_error;
		break;

		default: NODEFAULT( );
	}

	parent->finish_query( result );

	// FUNCTION BODY
	// <0x7616fc>|0x00c|+0x007:'54'
	// <0>
	// <0x761703>|0x013|+0x006:'56'
	// <0x761709>|0x019|+0x00c:'57'
	// <0x761715>|0x025|+0x013:'58'
	// <0>
	// <0x761728>|0x038|+0x01a:'60'	switch ( item_type )
	// <0>							{
	// <1>								case item_type_base_medkit:
	// <2>
	// <0x761742>|0x052|+0x051|[1]:'64'		medkit* resource = VOSTOK_NEW_IMPL( g_allocator, medkit );
	// <0x761793>|0x0a3|+0x048:'65'			resource->load( root["data"] );
	// <0x7617db>|0x0eb|+0x008:'66'			if ( item_dict_id )
	// <0x7617e3>|0x0f3|+0x016:'67'				resource->set_dict_id( item_dict_id );
	// <0>
	// <0x7617f9>|0x109|+0x02d:'69'
	// <0x761826>|0x136|+0x007:'70'
	// <0x76182d>|0x13d|+0x005:'71'
	// <0>
	// <1>
	// <2>							{
	// <0x761832>|0x142|+0x05a|[1]:'75'	oxygen_tank* resource = VOSTOK_NEW_IMPL( g_allocator, oxygen_tank );
	// <0x76188c>|0x19c|+0x048:'76'		resource->load( config->get_root( )["data"] );
	// <0x7618d4>|0x1e4|+0x033:'77'		parent->set_unmanaged_resource( resource, re
	// <0x761907>|0x217|+0x007:'78'		result = result_success;
	// <0x76190e>|0x21e|+0x005:'79'		break;
	// <0>							}
	// <1>
	// <2>
	// <0x761913>|0x223|+0x00c:'83'		ASSERT( UNKNOWN_EXPRESSION );
	// <0x76191f>|0x22f|+0x007:'84'
	// <0x761926>|0x236|+0x005:'85'		break;
	// <0>
	// <1>
	// <0x76192b>|0x23b|+0x05a|[1]:'88'	artefact_lifebone_core* resource = VOSTO
	// <0x761985>|0x295|+0x048:'89'		resource->load( config->get_root( )["data"] );
	// <0x7619cd>|0x2dd|+0x008:'90'		if ( item_dict_id )
	// <0x7619d5>|0x2e5|+0x016:'91'			resource->set_dict_id( item_dict_id );
	// <0>
	// <0x7619eb>|0x2fb|+0x033:'93'		parent->set_unmanaged_resource( resource,
	// <0x761a1e>|0x32e|+0x007:'94'		result = result_success;
	// <0x761a25>|0x335|+0x002:'95'		break;
	// <0>
	// <1>
	// <0x761a27>|0x337|+0x007:'98'
	// <0>
	// <1>							}
	// <0x761a2e>|0x33e|+0x00e:'101'
	// <0>
	// ******
}

} // namespace survarium

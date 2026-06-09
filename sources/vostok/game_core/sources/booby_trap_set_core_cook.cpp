////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/booby_trap_set_core_cook.h>

#include <vostok/game_core/booby_trap_set_core.h>
#include <vostok/game_core/booby_trap_core.h>

namespace survarium {

// STATE[96.53%|DONE]: LTCG call-target reloc only
// STRUCTURE DIFF[target 0x752310 | base 0x458220]: target 1 / base 1 stmts
// .. same ..
// ; aligned 1, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole diff is a relocated base-init call target, non-steerable. trail: booby_trap_set_core_cook.md
booby_trap_set_core_cook::booby_trap_set_core_cook( ) :
	resources::translate_query_cook( resources::booby_trap_set_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

// STATE[95.75%|DONE]: LTCG boost::bind closure-temp 8-byte frame inflation, no instr diff
// STRUCTURE DIFF[target 0x752750 | base 0x458660]: target 17 / base 17 stmts
// .. same ..
// ; aligned 17, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - zero instr add/remove; sole diff is sub esp 188h vs 190h (8B larger bind-closure temp), shifting [ebp-XX] slots uniformly, non-steerable. trail: booby_trap_set_core_cook.md
void booby_trap_set_core_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string config_name;
	config_name.assignf( "resources/%s", parent.get_requested_path( ) );

	booby_trap_set_cook_data cook_data;
	if ( parent.user_data( ) )
	{
		if ( !parent.user_data( )->try_get( cook_data ) )
		{
			ASSERT( UNKNOWN_EXPRESSION );
			parent.finish_query( result_error );
			return;
		}
	}
	else
	{
		cook_data.is_local_player = false;
		cook_data.stack_size = 1;
	}

	resources::query_resource(
		config_name.c_str( ),
		resources::binary_config_class_impl,
		boost::bind( &booby_trap_set_core_cook::on_config_ready, this, _1, cook_data ),
		g_allocator,
		NULL,
		&parent
	);
}

// STATE[82.01%|DONE]: LTCG resource_ptr-temp inline & create_request ABI
// STRUCTURE DIFF[target 0x7524c0 | base 0x4583d0]: target 28 / base 28 stmts
// .. same ..
// 0x02c <0x22> | 0x02c <0x1d> | configs::binary_config_ptr config = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );   SIZE
// .. same ..
// 0x0b2 <0xd> | 0x0ad <0x11> | resource->set_amount( cook_data.stack_size );   SIZE
// .. same ..
// 0x14f <0x27> | 0x14e <0x1a> | for ( u8 i = 0 ; i != cook_data.stack_size ; ++i )   SIZE
// .. same ..
// 0x176 <0x29> | 0x168 <0x26> | requests.push_back( resources::create_request( "", resources::booby_trap_class ) );   SIZE
// .. same ..
// 0x1b8 <0xa4> | 0x1a7 <0xb2> | );   SIZE
// ; aligned 23, size-diffs 5, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - all 5 SIZE diffs are LTCG inline-vs-temp choices in header/template code (target passes get_unmanaged_resource()/create_request prvalue directly; base materializes an extra resource_ptr copy-construct, inflating the frame 8B), non-steerable. trail: booby_trap_set_core_cook.md
void booby_trap_set_core_cook::on_config_ready(
	resources::queries_result&	data,
	booby_trap_set_cook_data	cook_data
)
{
	ASSERT( UNKNOWN_EXPRESSION );
	configs::binary_config_ptr config = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	ASSERT( UNKNOWN_EXPRESSION );

	configs::binary_config_value data_root = config->get_root( )["data"];
	ASSERT( UNKNOWN_EXPRESSION );

	booby_trap_set_core* const resource = new_derived_resource( );
	resource->set_amount( cook_data.stack_size );
	resource->load( data_root );

	if ( cook_data.stack_size == 0 )
	{
		finish_query( data.get_parent_query( ), resource );
		return;
	}

	buffer_vector< resources::request >		requests	( ALLOCA( cook_data.stack_size * sizeof( resources::request ) ), cook_data.stack_size );
	buffer_vector< variant< 32 > const* >	user_data	( ALLOCA( cook_data.stack_size * sizeof( variant< 32 > const* ) ), cook_data.stack_size );

	variant< 32 > 							udv;
	udv.set( config );
	for ( u8 i = 0 ; i != cook_data.stack_size ; ++i )
	{
		requests.push_back( resources::create_request( "", resources::booby_trap_class ) );
		user_data.push_back( &udv );
	}

	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind( &booby_trap_set_core_cook::on_subresources_loaded, this, _1, resource, cook_data, config ),
		g_allocator,
		user_data.begin( ),
		data.get_parent_query( )
	);
}

// STATE[89.02%|DONE]: LTCG resource_ptr-temp inline (static_cast_resource_ptr)
// STRUCTURE DIFF[target 0x7523c0 | base 0x458270]: target 11 / base 11 stmts
// .. same ..
// 0x039 <0x44> | 0x039 <0x41> | booby_trap_core_ptr trap = static_cast_resource_ptr< booby_trap_core_ptr >( data[i].get_unmanaged_resource( ) );   SIZE
// .. same ..
// ; aligned 10, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE diff: target builds get_unmanaged_resource() result as a direct stack prvalue into static_cast_resource_ptr; base materializes an extra resource_ptr copy-construct (one more call, +8B frame). Header/template LTCG inline choice, non-steerable. trail: booby_trap_set_core_cook.md
void booby_trap_set_core_cook::on_subresources_loaded(
	resources::queries_result&		data,
	booby_trap_set_core*			resource,
	booby_trap_set_cook_data		cook_data,
	configs::binary_config_ptr		config
)
{

	ASSERT( UNKNOWN_EXPRESSION );

	for ( u8 i = 0 ; i != data.size( ) ; ++i )
	{
		booby_trap_core_ptr trap = static_cast_resource_ptr< booby_trap_core_ptr >( data[i].get_unmanaged_resource( ) );
		ASSERT( UNKNOWN_EXPRESSION );
		resource->traps( ).push_back( trap );
		trap->set_owner( resource );
	}

	query_for_derived_resources( data.get_parent_query( ), resource, cook_data, config );
}

// STATE[100%|DONE]
void booby_trap_set_core_cook::finish_query( resources::query_result_for_cook* parent, booby_trap_set_core* resource )
{
	parent->set_unmanaged_resource(
		resources::unmanaged_resource_ptr( resource ),
		resources::memory_usage_type( resources::nocache_memory, get_derived_resource_size( ) )
	);
	parent->finish_query( result_success );
}

} // namespace survarium
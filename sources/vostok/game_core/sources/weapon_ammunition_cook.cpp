////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_ammunition_cook.h>

#include <vostok/game_core/weapon_ammunition.h>

namespace survarium {

// STATE[100%|DONE]
weapon_ammunition_cook::weapon_ammunition_cook( ) :
	resources::translate_query_cook( resources::weapon_ammunition_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

// STATE[99.83%|DONE]: LTCG frame-size/slot residual (boost::bind functor temp); structure exact.
void weapon_ammunition_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string config_name;
	config_name.assignf( "resources/%s", parent.get_requested_path( ) );

	resources::query_resource(
		config_name.c_str( ),
		resources::binary_config_class_impl,
		boost::bind( &weapon_ammunition_cook::on_config_ready, this, _1, &parent ),
		g_allocator,
		NULL,
		&parent
	);

	// STRUCTURE DIFF[target 0x7506d0 | base 0x45f270]: target 4 / base 4 stmts
	// .. same ..
	// ; aligned 4, size-diffs 0, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole residual is a uniform +0xC frame-size
	// (sub esp,18Ch base vs 180h target) shifting every [ebp-N] slot; instructions are
	// otherwise byte-identical. Non-steerable LTCG slot allocation for the boost::bind
	// functor temp passed to query_resource. trail: translate_query.md
}

// STATE[31.00%|PARTIAL]: shared cook-base wall - delete_helper LTCG arg passing.
void weapon_ammunition_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );

	// STRUCTURE DIFF[target 0x7505a0 | base 0x45f150]: target 1 / base 1 stmts
	// 0x009 <0x17> | 0x00a <0x16> | VOSTOK_DELETE_IMPL( g_allocator, resource );   SIZE
	// ; aligned 0, size-diffs 1, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - both call the same
	// delete_helper<doug_lea_allocator,resource_base> instantiation; target pushes the
	// resource_base*& arg on the stack (add esp,8), base passes it in edi (push/pop edi,
	// add esp,4) via the LTCG custom calling convention. Shared wall across all 5 raw-param
	// cook deleters (items_cook, animation_analysis_result_cook, items_dictionary_cook,
	// player_parameters_modifyer_cook, this), non-steerable LTCG arg passing. trail: delete_resource.md
}

// STATE[94.63%|PARTIAL]: resource_ptr-by-value temp materialization, non-steerable.
void weapon_ammunition_cook::on_config_ready( resources::queries_result& data, resources::query_result_for_cook* parent )
{
	ASSERT( UNKNOWN_EXPRESSION_T( data.size( ) == 1 ) );
	configs::binary_config_ptr	config	= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	weapon_ammunition*			wa		= VOSTOK_NEW_IMPL( g_allocator, weapon_ammunition );
	wa->load( config->get_root( )["data"] );

	parent->set_unmanaged_resource( wa, resources::memory_usage_type( resources::nocache_memory, sizeof( weapon_ammunition ) ) );
	parent->finish_query( result_success );

	// STRUCTURE DIFF[target 0x7505d0 | base 0x45f180]: target 7 / base 7 stmts
	// .. same ..
	// 0x016 <0x22> | 0x016 <0x1d> | configs::binary_config_ptr	config	= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );   SIZE
	// .. same ..
	// ; aligned 6, size-diffs 1, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE on the static_cast_resource_ptr line:
	// target materializes the by-value get_unmanaged_resource() resource_ptr temp into a
	// named [ebp-10h] slot and destroys it with an explicit intrusive_ptr::dec, base builds
	// it inline via push ecx;mov esi,esp + add esp,4 (no dec). Non-steerable LTCG by-value
	// smart-ptr temp materialization; all other 6 statements byte-exact. trail: on_config_loaded.md
}

} // namespace survarium

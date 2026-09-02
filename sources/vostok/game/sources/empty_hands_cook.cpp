// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "empty_hands_cook.h"
#include "empty_hands.h"

namespace survarium {

empty_hands_cook::empty_hands_cook( ) :
	resources::translate_query_cook( resources::empty_hands_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

void empty_hands_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string empty_hands_config_name;
	empty_hands_config_name.assignf( "resources/%s", parent.get_requested_path( ) );

	resources::query_resource(
		empty_hands_config_name.c_str( ),
		resources::binary_config_class_impl,
		boost::bind( &empty_hands_cook::on_empty_hands_config_loaded, this, _1 ),
		g_allocator,
		NULL,
		&parent
	);
}

// claude@NOTE: Source and PDB structure are complete. Target preserves the Boost _1
// load and has_empty_target call; base folds _1 to zero and selects an ICF-equivalent
// helper. The request count, VLA, and per-element construction match the target.

void empty_hands_cook::on_empty_hands_config_loaded( resources::queries_result& data )
{
	configs::binary_config_ptr config					= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );

	configs::binary_config_value animations_node		= config->get_root( )["user_animations"];

	u32 const animations_count = animations_node.size( );
	buffer_vector< resources::request > requests( ALLOCA( animations_count * sizeof( resources::request ) ), animations_count );

	for ( u32 i = 0 ; i < animations_count ; ++i )
		requests.push_back( resources::create_request( (pcstr)animations_node[i], resources::animation_class ) );

	resources::query_resources(
		&requests.front( ),
		requests.size( ),
		boost::bind( &empty_hands_cook::on_empty_hands_animations_loaded, this, _1 ),
		g_allocator,
		NULL,
		data.get_parent_query( )
	);
}

void empty_hands_cook::on_empty_hands_animations_loaded( resources::queries_result& data )
{
	const u32 animations_count	= data.size( );
	const u32 buffer_size		= sizeof( empty_hands ) + animations_count * sizeof( resources::managed_resource_ptr );

	empty_hands* hands			= (empty_hands*)VOSTOK_MALLOC_IMPL( g_allocator, buffer_size, "empty_hands" );

	resources::managed_resource_ptr* animations = (resources::managed_resource_ptr*)( hands + 1 );
	for ( u32 i = 0 ; i < animations_count ; ++i )
		new ( animations + i ) resources::managed_resource_ptr( data[i].get_managed_resource( ) );

	new ( hands ) empty_hands( animations, animations_count );

	resources::query_result_for_cook* const parent = data.get_parent_query( );
	parent->set_unmanaged_resource( hands, resources::memory_usage_type( resources::nocache_memory, buffer_size ) );
	parent->finish_query( result_success );
}

// The trailing animations share hands' allocation and must be destroyed first.
// Target's final free uses the legacy RTTI/get_top_pointer helper unavailable in
// the current allocator header.
void empty_hands_cook::delete_resource( resources::resource_base* resource )
{
	empty_hands* hands = static_cast_checked< empty_hands* >( resource );
	resources::managed_resource_ptr* animations = hands->m_animations;
	u32 const animations_count = hands->m_animations_count;

	hands->~empty_hands( );

	for ( u32 i = 0 ; i < animations_count ; ++i )
		animations[ i ].~resource_ptr( );

	VOSTOK_FREE_IMPL( g_allocator, hands );
}

} // namespace survarium

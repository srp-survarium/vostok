////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for options-menu item symbols that
// are not yet retained by the reconstructed game call graph.
//
// Retire once game_options (the matched owner) reaches these for itself.

#include "pch.h"

#include <vostok/scaleform/sources/flash_value.h>
#include <vostok/scaleform/sources/flash_function_handler_params.h>

#include "options_graphics_quality_selector.h"
#include "options_item_bool.h"
#include "options_item_float.h"
#include "options_item_int.h"
#include "options_monitor_index_selector.h"
#include "options_resolution_selector.h"
#include "options_tab.h"

namespace survarium {

void use_game_options_items( )
{
	static volatile bool s_run = false;
	if( !s_run )
		return;

	// volatile-sourced argument values so LTCG cannot const-propagate the anchor's
	// literals into the carcass bodies (a constant arg folds the match away).
	static pcstr volatile	s_pcstr		= 0;
	static volatile u8		s_u8		= 0;
	static pcstr* volatile	s_pcstr_arr	= 0;
	static volatile float	s_float		= 0.0f;
	const pcstr		any_pcstr	= s_pcstr;
	const u8		any_u8		= s_u8;
	pcstr* const	any_pcstr_arr	= s_pcstr_arr;
	const float		any_float	= s_float;

	options_tab&					tab		= *( options_tab* )NULL;
	flash_value&					value	= *( flash_value* )NULL;
	flash_function_handler_params&	params	= *( flash_function_handler_params* )NULL;
	flash_movie_resource_ptr&		movie	= *( flash_movie_resource_ptr* )NULL;

	// options_item_int: construct and exercise every out-of-line method.
	options_item_int int_item( tab, any_pcstr, any_u8, any_pcstr_arr, any_u8 );
	int_item.initialize( );
	int_item.fill_data( value );
	int_item.fill_value( value );
	int_item.apply( );
	int_item.revert( );
	int_item.call( params );

	// options_item_float
	options_item_float float_item( tab, any_pcstr, any_u8, any_float );
	float_item.initialize( );
	float_item.fill_data( value );
	float_item.fill_value( value );
	float_item.apply( );
	float_item.revert( );
	float_item.call( params );

	// options_item_bool
	options_item_bool bool_item( tab, any_pcstr, any_u8 );
	bool_item.initialize( );
	bool_item.fill_data( value );
	bool_item.fill_value( value );
	bool_item.apply( );
	bool_item.revert( );
	bool_item.call( params );

	// the concrete selectors (each derives an item type)
	options_resolution_selector resolution( tab );
	resolution.initialize( );
	resolution.apply( );
	resolution.fill_resolutions( any_u8 );

	options_monitor_index_selector monitor( tab );
	monitor.call( params );
	monitor.revert( );
	monitor.refill_resolutions_data( );

	options_graphics_quality_selector graphics( tab );
	graphics.call( params );

	// options_tab
	static volatile int s_type = 0;
	options_tab built_tab( *( game* )NULL, movie, ( options_enum )s_type );
	built_tab.apply( movie );
	built_tab.revert( movie );
	built_tab.initialize_data( movie );
}

} // namespace survarium

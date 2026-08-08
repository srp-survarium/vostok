////////////////////////////////////////////////////////////////////////////
//	Created		: 09.11.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/sound/propagator_statistic.h>
#include <vostok/sound/proxy_statistic.h>
#include <vostok/sound/sound_scene_statistic.h>
#include <vostok/ui/text_tree_draw_helper.h>

namespace vostok {
namespace sound {

void propagator_statistic::fill_text_tree	( strings::text_tree_item* item ) const
{
	fixed_string<64> temp;
	strings::text_tree_item* stats		= item->new_child( m_filename.c_str( ) );
	stats								= item->new_child( "length" );
	temp.assignf						( "%d", m_length );
	stats->add_column					( temp.c_str( ) );
	stats								= item->new_child( "current_playing_time" );
	temp.assignf						( "%d", m_current_playing_time );
	stats->add_column					( temp.c_str( ) );
	stats								= item->new_child( "playback_mode" );
	switch ( m_playback_mode )
	{
	case once:		stats->add_column	( "once" );		break;
	case looped:	stats->add_column	( "looped" );	break;
	default:		NODEFAULT			( );
	}
}

void proxy_statistic::fill_text_tree		( strings::text_tree_item* item, bool draw_propagators_stats ) const
{
		fixed_string<64> temp;
		strings::text_tree_item* stats		= item->new_child( "proxy id" );
		temp.assignf						( "%d", m_id );
		stats->add_column					( temp.c_str( ) );
		strings::text_tree_item* detail		= stats->new_child( "emitter type" );
		switch( m_emitter_type )
		{
		case single:		detail->add_column	( "single" );		break;
		case composite:		detail->add_column	( "composite" );	break;
		case collection:	detail->add_column	( "collection" );	break;
		default:			NODEFAULT		( );
		};
		detail								= stats->new_child( "sound type" );
		switch( m_sound_type )
		{
		case point:
			{
				detail->add_column			( "point" );
				detail						= stats->new_child( "position" );
				temp.assignf				( "%.2f, %.2f, %.2f", m_position[0], m_position[1], m_position[2] );
				detail->add_column			( temp.c_str( ) );
			} break;
		case cone:
			{
				detail->add_column			( "cone" );
				detail						= stats->new_child( "position" );
				temp.assignf				( "%.2f, %.2f, %.2f", m_position[0], m_position[1], m_position[2] );
				detail->add_column			( temp.c_str( ) );
				detail						= stats->new_child( "direction" );
				temp.assignf				( "%.2f, %.2f, %.2f", m_direction[0], m_direction[1], m_direction[2] );
				detail->add_column			( temp.c_str( ) );
				detail						= stats->new_child( "cone type" );
				switch ( m_cone_type )
				{
				case human:					detail->add_column( "human" );	break;
				default:					NODEFAULT( );
				};
			} break;
		case volumetric:
			{
				detail->add_column			( "volumetric" );
				detail						= stats->new_child( "position" );
				temp.assignf				( "%.2f, %.2f, %.2f", m_position[0], m_position[1], m_position[2] );
				detail->add_column			( temp.c_str( ) );
			} break;
		case hud:
			{
				detail->add_column			( "hud" );
				detail						= stats->new_child( "position" );
				temp.assignf				( "%.2f, %.2f, %.2f", m_position[0], m_position[1], m_position[2] );
				detail->add_column			( temp.c_str( ) );
			} break;
		default:			NODEFAULT		( );
		};

		if ( !draw_propagators_stats )
			return;

		propagator_statistic* prop			= m_propagator_statistics.front( );
		while ( prop )
		{
			prop->fill_text_tree			( detail );
			prop							= m_propagator_statistics.get_next_of_object( prop );
		}
}

sound_scene_statistic::sound_scene_statistic( )
{
	memory::zero( &values, sizeof( values ) );
}

void sound_scene_statistic::fill_text_tree( strings::text_tree_item* item ) const
{
	fixed_string<64> temp;
	strings::text_tree_item* stats = 0;
	stats = item->new_child( "voices count(mono:st)" );
	temp.assignf( "%d:%d", values.m_active_voices_count[0], values.m_active_voices_count[1] );
	stats->add_column( temp.c_str( ) );
	stats = item->new_child( "proxies count" );
	temp.assignf( "%d", values.m_active_proxies_count );
	stats->add_column( temp.c_str( ) );
	stats = item->new_child( "propagators count" );
	temp.assignf( "%d", values.m_propagators_count );
	stats->add_column( temp.c_str( ) );
	stats = item->new_child( "proxy types" );
	temp.assignf( "point:[%d] hud:[%d]", values.m_sound_types[point], values.m_sound_types[hud] );
	stats->add_column( temp.c_str( ) );
}

} // namespace sound
} // namespace vostok

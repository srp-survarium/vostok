////////////////////////////////////////////////////////////////////////////
//	Created		: 03.03.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "stats_graph.h"
#include <vostok/render/facade/ui_renderer.h>

using survarium::stats_graph;
using vostok::render::world;

// stats_time: canonical renamed this to cumulative_time() (a separate carcass
// stub, not in this batch's port list) - left for manual review.
float stats_graph::stats_time			( ) const
{
	R_ASSERT						( m_count );
	R_ASSERT						( m_current_value );
	R_ASSERT						( m_current_value->next );
	return							m_current_value->time - m_current_value->next->time;
}

// render: NOT HARVESTED. Signature diverged - the canonical render() drops the
// legacy current_frame_id / left_margin / width params (carcass params are
// top_margin / height / arg_4..6), so the legacy body's left_margin/width/
// current_frame_id usage doesn't map. Needs manual review.
void stats_graph::render				( vostok::render::ui::renderer& renderer, vostok::render::scene_view_ptr const& scene_view, u32 const current_frame_id, u32 const left_margin, u32 const top_margin, u32 const width, u32 const height )
{
	VOSTOK_UNREFERENCED_PARAMETER		(current_frame_id);
	if ( !m_current_value )
		return;

#ifdef FIXED_FPS
	float const time_delta			= m_current_value->time - m_last_render_time;
	float const spf					= 1.f/FIXED_FPS;
	if ( time_delta < spf )
		return;

	if ( m_last_commands[0] ) {
		for ( u32 i=0; i<command_count; ++i) {
			m_last_commands[i]->remove_frame_id	= current_frame_id;
			m_last_commands[i]				= 0;
		}
	}

	m_last_render_time				= m_current_value->time;// - time_delta + math::floor( time_delta/spf )*spf;
#endif // #ifdef FIXED_FPS

	float min_value					= 0.f;
	float max_value					= m_important_value1;

	stats_value *i					= m_current_value->next;
	u32 count						= 0;
	do {
		min_value					= math::min( i->value, min_value );
		if ( i->value != m_invalid_value ) {
			max_value				= math::max( i->value, max_value );
			++count;
		}

		i							= i->next;
	} while ( i->previous != m_current_value );

	if ( count < 2 )
		return;

	float const value_range			= max_value - min_value;
	float const value_height		= float(height);

	for ( u32 i=0; i<4; ++i ) {
		float						value;
		u32							color;
		switch ( i ) {
			case 0 : {
				value				= m_important_value0;
				color				= math::color_xrgb( 255, 0, 0 );
				break;
			}
			case 1 : {
				value				= m_important_value1;
				color				= math::color_xrgb( 255, 255, 0 );
				break;
			}
			case 2 : {
				value				= 0.f;
				color				= math::color_xrgb( 0, 0, 0);
				break;
			}
			case 3 : {
				value				= average_value();
				color				= math::color_xrgb( 0, 255, 255 );
				break;
			}
			default :				{
				NODEFAULT();
#ifdef DEBUG
				value				= math::SNaN;
				color				= *(u32*)&value;
#endif // #ifdef DEBUG
			}
		}

//		vostok::render::ui::command* const command = renderer.create_command( 2, 1, 0 );
#ifdef FIXED_FPS
		m_last_commands[i]			= command;
#endif // #ifdef FIXED_FPS
		float y						= float(top_margin + math::floor( (1.f - value/float(value_range))*value_height ));
		vostok::render::ui::vertex const vertices[] = {
			vostok::render::ui::vertex( float(left_margin),				y, 0.f, color, 0, 0 ),
			vostok::render::ui::vertex( float(left_margin + width - 1),	y, 0.f, color, 0, 0 ),
		};
		renderer.draw_vertices		( scene_view, array_begin(vertices), array_end(vertices), 1, 0 );
//		command->push_point			( float(left_margin),				y, 0.f, color, 0, 0 );
//		command->push_point			( float(left_margin + width - 1),	y, 0.f, color, 0, 0 );
#ifdef FIXED_FPS
		command->remove_frame_id	= u32(-1);
#endif // #ifdef FIXED_FPS
//		renderer.push_command		( command );
	}

//	vostok::render::ui::command* const command = renderer.create_command( count, 1, 0 );
#ifdef FIXED_FPS
	m_last_commands[command_count - 1]	= command;
#endif // #ifdef FIXED_FPS

	typedef buffer_vector< vostok::render::ui::vertex >	vertices_type;
	vertices_type vertices( ALLOCA(count*sizeof(vostok::render::ui::vertex)), count );
	float total_time				= math::max	( stats_time(), m_time_interval );
	i								= m_current_value->next;
	float const start_time			= i->time;
	do {
		if ( i->value != m_invalid_value ) {
			u32 const x				= left_margin + math::floor( (i->time - start_time)/total_time*float(width - 1) );
			u32 const y				= top_margin + math::floor( (1.f - i->value/float(value_range))*value_height );
			vertices.push_back		( vostok::render::ui::vertex( float(x), float(y), 0.f, m_color, 0, 0 ) );
		}
		i							= i->next;
	} while ( i != m_current_value );

#ifdef FIXED_FPS
	command->remove_frame_id		= u32(-1);
#endif // #ifdef FIXED_FPS
	renderer.draw_vertices			( scene_view, &*vertices.begin(), &*vertices.end(), 1, 0 );
//	renderer.push_command				( command );
}

// stop_rendering: NOT HARVESTED. Legacy body is entirely under #ifdef FIXED_FPS
// (off), so it compiles to nothing, but the carcass body (10 lines) clearly does
// real work - divergence. Needs manual review.
void stats_graph::stop_rendering	( )
{
#ifdef FIXED_FPS
	if ( !m_last_commands[0] )
		return;

	for ( u32 i=0; i<command_count; ++i ) {
		m_last_commands[i]->remove_frame_id	= 0;
		m_last_commands[i]					= 0;
	}
#endif // #ifdef FIXED_FPS
}
